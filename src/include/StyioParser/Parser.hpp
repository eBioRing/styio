#ifndef STYIO_PARSER_H_
#define STYIO_PARSER_H_

static int readNextChar() 
{
  int tmpChar = getchar();

  return tmpChar;
}

static void dropAllSpaces (int& nextChar) 
{
  while (isspace(nextChar)) {
    nextChar = readNextChar();
  };
}


static void dropWhiteSpace (int& nextChar) 
{
  while (nextChar == ' ') {
    nextChar = readNextChar();
  };
}

static IdAST* parseId (std::vector<int>& tokenBuffer, int& nextChar) 
{
  std::string idStr = "";
  idStr += nextChar;

  // [a-zA-Z][a-zA-Z0-9_]*
  while (isalnum((nextChar = readNextChar())) || nextChar == '_') 
  {
    idStr += nextChar;
  }

  tokenBuffer.push_back(
    StyioToken::TOK_ID
  );

  IdAST* result = new IdAST(idStr);

  std::cout << result -> toString() << std::endl;

  return result;
}

static StyioAST* parseNum (std::vector<int>& tokenBuffer, int& nextChar)
{
  std::string numStr = "";
  numStr += nextChar;
  nextChar = readNextChar();

  // [0-9]*
  while (isdigit(nextChar))
  {
    numStr += nextChar;
    nextChar = readNextChar();
  };

  if (nextChar == '.') 
  {
    numStr += nextChar;
    nextChar = readNextChar();

    while (isdigit(nextChar))
    {
      numStr += nextChar;
      nextChar = readNextChar();
    };

    // if (!isspace(nextChar)) 
    // {
    //   std::string errmsg = "Float `" + numStr + "` ends with unexpected char `" + char(nextChar) + "`";
    //   throw StyioSyntaxError(errmsg);
    // }

    tokenBuffer.push_back(
      StyioToken::TOK_FLOAT
    );

    FloatAST* result = new FloatAST(std::stod(numStr));

    std::cout << result -> toString() << std::endl;

    return result;
  } 
  else 
  {
    // if (!isspace(nextChar)) 
    // {
    //   std::string errmsg = "Int `" + numStr + "` ends with unexpected char `" + char(nextChar) + "`";
    //   throw StyioSyntaxError(errmsg);
    // }

    tokenBuffer.push_back(
      StyioToken::TOK_INT
    );

    IntAST* result = new IntAST(std::stoi(numStr));

    std::cout << result -> toString() << std::endl;

    return result;
  }
}

static StringAST* parseString (std::vector<int>& tokenBuffer, int& nextChar) 
{
  // eliminate the first(start) double quote
  nextChar = readNextChar();

  std::string textStr = "";
  
  while (nextChar != '\"')
  {
    textStr += nextChar;
    nextChar = readNextChar();
  };

  // eliminate the second(end) double quote
  nextChar = readNextChar();

  tokenBuffer.push_back(
    StyioToken::TOK_STRING
  );

  StringAST* result = new StringAST(textStr);

  std::cout << result -> toString() << std::endl;

  return result;
}

static AssignAST* parseAssign (
  std::vector<int>& tokenBuffer, 
  int& nextChar, 
  IdAST* idAST
) 
{
  if (isalpha(nextChar) || nextChar == '_') 
  {
    StyioAST* value = parseId(tokenBuffer, nextChar);
    
    AssignAST* result = new AssignAST(idAST, value);

    std::cout << result -> toString() << std::endl;

    return result;
  }
  else
  if (isdigit(nextChar))
  {
    StyioAST* value = parseNum(tokenBuffer, nextChar);
    
    AssignAST* result = new AssignAST(idAST, value);

    std::cout << result -> toString() << std::endl;

    return result;
  }
  else
  {
    std::string errmsg = std::string("Unexpected Assign.Value, starts with character `") + char(nextChar) + "`";
    throw StyioSyntaxError(errmsg);
  };

  std::cout << "|NotImplemented| VAR_ASSIGN" << std::endl;
}

static BinOpAST* parseBinOp (
  std::vector<int>& tokenBuffer, 
  int& nextChar, 
  StyioToken signToken,
  StyioAST* lhsAST
) 
{
  dropWhiteSpace(nextChar);

  if (isalpha(nextChar) || nextChar == '_') {
    BinOpAST* result = new BinOpAST(signToken, lhsAST, parseId(tokenBuffer, nextChar));
    std::cout << result -> toString() << std::endl;
    return result;
  }
  else
  if (isdigit(nextChar)) {
    BinOpAST* result = new BinOpAST(signToken, lhsAST, parseNum(tokenBuffer, nextChar));
    std::cout << result -> toString() << std::endl;
    return result;
  }
  else
  {
    std::string errmsg = std::string("Unexpected BinOp.RHS, starts with character `") + char(nextChar) + "`";
    throw StyioSyntaxError(errmsg);
  };
}

static StyioAST* parseExpr (std::vector<int>& tokenBuffer, int& nextChar) 
{
  while (nextChar != '\n')
  {
    dropAllSpaces(nextChar);

    // <ID>
    if (isalpha(nextChar) || nextChar == '_') 
    {
      // parse id
      IdAST* idAST = parseId(tokenBuffer, nextChar);
      
      // ignore white spaces after id
      dropWhiteSpace(nextChar);

      // check next character
      switch (nextChar)
      {
        // <LF>
        case '\n':
          {
            // simply eliminate LF
            nextChar = readNextChar();
            tokenBuffer.push_back(
              StyioToken::TOK_LF
            );
            return idAST;
          };

          // You should NOT reach this line.
          break;
        
        // <ID> := <EXPR>
        case ':':
          {
            nextChar = readNextChar();
            if (nextChar == '=')
            {
              nextChar = readNextChar();

              tokenBuffer.push_back(
                StyioToken::TOK_WALRUS
              );

              dropWhiteSpace(nextChar);
              
              // <ID> := | ->
              AssignAST* assignAST = parseAssign(tokenBuffer, nextChar, idAST);
              
              return assignAST;
            }
            else
            {
              std::string errmsg = std::string("Unexpected `:`");
              throw StyioSyntaxError(errmsg);
            }
          };

          // You should NOT reach this line.
          break;

        // BIN_ADD := <ID> "+" <EXPR>
        case '+':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_ADD);

            // <ID> "+" | -> 
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_ADD, idAST);
            return binOpAST;
          };

          // You should NOT reach this line.
          break;

        // BIN_SUB := <ID> "-" <EXPR>
        case '-':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_SUB);

            // <ID> "-" | ->
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_SUB, idAST);
            return binOpAST;
          };

          // You should NOT reach this line.
          break;

        // BIN_MUL | BIN_POW
        case '*':
          {
            nextChar = readNextChar();
            // BIN_POW := <ID> "**" <EXPR>
            if (nextChar == '*')
            {
              nextChar = readNextChar();
              tokenBuffer.push_back(StyioToken::TOK_POW);

              // <ID> "**" | ->
              BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_POW, idAST);
              return binOpAST;
            } 
            // BIN_MUL := <ID> "*" <EXPR>
            else 
            {
              tokenBuffer.push_back(StyioToken::TOK_MUL);

              // <ID> "*" | ->
              BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_MUL, idAST);
              return binOpAST;
            }
          };
          // You should NOT reach this line.
          break;
          
        // BIN_DIV := <ID> "/" <EXPR>
        case '/':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_DIV);

            // <ID> "/" | -> 
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_DIV, idAST);
            return binOpAST;
          };

          // You should NOT reach this line.
          break;

        // BIN_MOD := <ID> "%" <EXPR> 
        case '%':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_MOD);

            // <ID> "%" | -> 
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_MOD, idAST);
            return binOpAST;
          };

          // You should NOT reach this line.
          break;
        
        default:
          break;
      }
    }

    if (isdigit(nextChar)) {
      StyioAST* numAST = parseNum(tokenBuffer, nextChar);

      dropAllSpaces(nextChar);

      switch (nextChar)
      {
        // <LF>
        case '\n':
          {
            // simply eliminate LF
            nextChar = readNextChar();
            tokenBuffer.push_back(
              StyioToken::TOK_LF
            );
            return new ValExprAST(numAST);
          };

          // You should NOT reach this line.
          break;

        // BIN_ADD := [<Int>|<Float>] "+" <EXPR>
        case '+':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_ADD);

            // [<Int>|<Float>] "+" | ->
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_ADD, numAST);
            return new ValExprAST(binOpAST);
          };

          // You should NOT reach this line.
          break;

        // BIN_SUB := [<Int>|<Float>] "-" <EXPR>
        case '-':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_SUB);

            // [<Int>|<Float>] "-" | ->
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_SUB, numAST);
            return new ValExprAST(binOpAST);
          };

          // You should NOT reach this line.
          break;

        // BIN_MUL | BIN_POW
        case '*':
          nextChar = readNextChar();
          // BIN_POW := [<Int>|<Float>] "**" <EXPR>
          if (nextChar == '*')
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_POW);

            // [<Int>|<Float>] "**" | ->
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_POW, numAST);
            return new ValExprAST(binOpAST);
          } 
          // BIN_MUL := [<Int>|<Float>] "*" <EXPR>
          else 
          {
            tokenBuffer.push_back(StyioToken::TOK_MUL);

            // [<Int>|<Float>] "*" | ->
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_MUL, numAST);
            return new ValExprAST(binOpAST);
          }
          // You should NOT reach this line.
          break;

        // BIN_DIV := [<Int>|<Float>] "/" <EXPR>
        case '/':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_DIV);
            
            // [<Int>|<Float>] "/" | ->
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_DIV, numAST);
            return new ValExprAST(binOpAST);
          };
          // You should NOT reach this line.
          break;

        // BIN_MOD := [<Int>|<Float>] "%" <EXPR>
        case '%':
          {
            nextChar = readNextChar();
            tokenBuffer.push_back(StyioToken::TOK_MOD);
            
            // [<Int>|<Float>] "%" | ->
            BinOpAST* binOpAST = parseBinOp(tokenBuffer, nextChar, StyioToken::TOK_MOD, numAST);
            return new ValExprAST(binOpAST);
          };

          // You should NOT reach this line.
          break;

        default:
          return new ValExprAST(numAST);

          // You should NOT reach this line.
          break;
      }
    }

    switch (nextChar)
    {
      // VAR_DEF := "@" "(" [<ID> ["," <ID>]*]? ")"
      case '@':
        {
          nextChar = readNextChar();

          tokenBuffer.push_back(
            StyioToken::TOK_AT
          );
          
          dropWhiteSpace(nextChar);

          if (nextChar == '(') 
          {
            nextChar = readNextChar();

            tokenBuffer.push_back(
              StyioToken::TOK_LPAREN
            );
          };
          
          std::vector<IdAST*> varBuffer;

          if (isalpha(nextChar) || nextChar == '_') {
            // "@" "(" | ->
            IdAST* idAST = parseId(tokenBuffer, nextChar);
        
            varBuffer.push_back(idAST);
          };

          dropWhiteSpace(nextChar);

          // "@" "(" [<ID> | ->
          while (nextChar == ',')
          {
            nextChar = readNextChar();

            dropWhiteSpace(nextChar);

            if (isalpha(nextChar) || nextChar == '_') {
              tokenBuffer.push_back(
                StyioToken::TOK_COMMA
              );
              
              IdAST* idAST = parseId(tokenBuffer, nextChar);
        
              varBuffer.push_back(idAST);

              dropWhiteSpace(nextChar);
            };
          };
          
          if (nextChar == ')') 
          {
            nextChar = readNextChar();

            tokenBuffer.push_back(
              StyioToken::TOK_RPAREN
            );
          };

          VarDefAST* result = new VarDefAST(varBuffer);

          std::cout << result -> toString() << std::endl;

          return result;
          
          // You should NOT reach this line.
          break;
        };

      case ':':
        nextChar = readNextChar();

        if (nextChar == '=') {
          nextChar = readNextChar();

          tokenBuffer.push_back(
            StyioToken::TOK_WALRUS
          );
        } 
        else
        {
          tokenBuffer.push_back(
            StyioToken::TOK_COLON
          );
        };
        
        break;

      case '-':
        nextChar = readNextChar();

        if (nextChar == '>') {
          nextChar = readNextChar();

          tokenBuffer.push_back(
            StyioToken::TOK_RARROW
          );
        } else {
          tokenBuffer.push_back(
            StyioToken::TOK_MINUS
          );
        };
        
        break;

      case '?':
        nextChar = readNextChar();
        
        if (nextChar == '=') {
          nextChar = readNextChar();

          tokenBuffer.push_back(
            StyioToken::TOK_MATCH
          );
        } 
        else 
        {
          tokenBuffer.push_back(
            StyioToken::TOK_CHECK
          );
        };
        
        break;
      
      case '\"':
        parseString(tokenBuffer, nextChar);
        break;

      case '!':
        tokenBuffer.push_back(
          StyioToken::TOK_EXCLAM
        );
        nextChar = readNextChar();
        break;

      case ',':
        tokenBuffer.push_back(
          StyioToken::TOK_COMMA
        );
        nextChar = readNextChar();
        break;

      case '.':
        tokenBuffer.push_back(
          StyioToken::TOK_DOT
        );
        nextChar = readNextChar();
        break;

      case ';':
        tokenBuffer.push_back(
          StyioToken::TOK_SEMICOLON
        );
        nextChar = readNextChar();
        break;

      case '(':
        tokenBuffer.push_back(
          StyioToken::TOK_LPAREN
        );
        nextChar = readNextChar();
        break;

      case ')':
        tokenBuffer.push_back(
          StyioToken::TOK_RPAREN
        );
        nextChar = readNextChar();
        break;

      case '[':
        tokenBuffer.push_back(
          StyioToken::TOK_LBOXBRAC
        );
        nextChar = readNextChar();

        break;

      case ']':
        tokenBuffer.push_back(
          StyioToken::TOK_RBOXBRAC
        );
        nextChar = readNextChar();
        
        break;

      case '{':
        nextChar = readNextChar();
        
        tokenBuffer.push_back(
          StyioToken::TOK_LCURBRAC
        );
        
        // "{" | ->

        break;

      case '}':
        nextChar = readNextChar();
        
        tokenBuffer.push_back(
          StyioToken::TOK_RCURBRAC
        );

        break;

      case '<':
        nextChar = readNextChar();
        tokenBuffer.push_back(
          StyioToken::TOK_LANGBRAC
        );
        break;

      case '>':
        nextChar = readNextChar();
        tokenBuffer.push_back(
          StyioToken::TOK_RANGBRAC
        );
        break;
        
      default:
        break;
    }

    std::cout << "Next: " << char(nextChar) << std::endl;
  };
}

static std::string parseDependencyItem(std::vector<int>& tokenBuffer, int& nextChar)
{
  std::string itemStr;

  if (nextChar == '\"')
  {
    // eliminate double quote symbol " at the start of dependency item
    nextChar = readNextChar();

    while (nextChar != '\"') 
    {
      if (nextChar == ',') 
      {
        std::string errmsg = std::string("An \" was expected after") + itemStr + "however, a delimeter `,` was detected. ";
        throw StyioSyntaxError(errmsg);
      }

      itemStr += nextChar;

      nextChar = readNextChar();
    };

    // eliminate double quote symbol " at the end of dependency item
    nextChar = readNextChar();

    return itemStr;
  }
  else
  {
    std::string errmsg = std::string("Dependencies should be wrapped with double quote like \"abc/xyz\", rather than starting with the character `") + char(nextChar) + "`";
    throw StyioSyntaxError(errmsg);
  };
}

/*
parseDependency

Dependencies should be written like a list of paths
like this -> ["ab/c", "x/yz"]

// 1. The dependencies should be parsed before any domain (statement/expression). 
// 2. The left square bracket `[` is only eliminated after entering this function (parseDependency)
| -> "[" <PATH>+ "]"

If ? ( "the program starts with a left square bracket `[`" ),
then -> { 
  "parseDependency() starts";
  "eliminate the left square bracket `[`";
  "parse dependency paths, which take comma `,` as delimeter";
  "eliminate the right square bracket `]`";
} 
else :  { 
  "parseDependency() should NOT be invoked in this case";
  "if starts with left curly brace `{`, try parseSpace()";
  "otherwise, try parseScript()";
}

*/
static DependencyAST* parseDependency (std::vector<int>& tokenBuffer, int& nextChar) 
{ 
  // eliminate left square (box) bracket [
  nextChar = readNextChar();
  tokenBuffer.push_back(
    StyioToken::TOK_LBOXBRAC
  );

  std::vector<std::string> dependencies;

  dropAllSpaces(nextChar);

  // add the first dependency path to the list
  dependencies.push_back(parseDependencyItem(tokenBuffer, nextChar));

  std::string pathStr = "";
  
  while (nextChar == ',') {
    // eliminate comma ","
    nextChar = readNextChar();

    // reset pathStr to empty ""
    pathStr = ""; 

    dropAllSpaces(nextChar);
    
    // add the next dependency path to the list
    dependencies.push_back(parseDependencyItem(tokenBuffer, nextChar));
  };

  if (nextChar == ']') {
    // eliminate right square bracket `]` after dependency list
    nextChar = readNextChar();
  };

  DependencyAST* result = new DependencyAST(dependencies);

  std::cout << result -> toString() << std::endl;

  return result;
}

static BlockAST* parseBlock (std::vector<int>& tokenBuffer, int& nextChar) 
{
  // the last expression will be the return expression
  // a block must have a return value
  // either an expression
  // or null

  std::vector<StyioAST*> stmtBuffer;

  StyioAST* exprAST = parseExpr(tokenBuffer, nextChar);
  
  if (nextChar == ';')
  {
    stmtBuffer.push_back(exprAST);

    while (nextChar == ';')
    {
      // eliminate ;
      nextChar = readNextChar();

      StyioAST* exprAST = parseExpr(tokenBuffer, nextChar);
      stmtBuffer.push_back(exprAST);
    }

    BlockAST* result = new BlockAST(stmtBuffer, exprAST);

    std::cout << result -> toString() << std::endl;

    return result;
  }
  else
  {
    BlockAST* result = new BlockAST(exprAST);

    std::cout << result -> toString() << std::endl;

    return result;
  };
}

static void parseSpace (std::vector<int>& tokenBuffer, int& nextChar) 
{
  
}

static void parseScript (std::vector<int>& tokenBuffer, int& nextChar) {
  if (nextChar == '{') 
  {
    // eliminate "{"
    nextChar = readNextChar();

    parseBlock(tokenBuffer, nextChar);

    // eliminate "}"
    if (nextChar == '}')
    {
      nextChar = readNextChar();
    }
    else
    {
      std::string errmsg = std::string("Missing `}` after code block.");
      throw StyioSyntaxError(errmsg);
    };
  }
  else
  {
    parseBlock(tokenBuffer, nextChar);
  };
}

static std::vector<int> parseProgram () 
{
  std::vector<int> tokenBuffer;
  static int nextChar = ' ';

  while (1) 
  {
    fprintf(stderr, "Styio/> ");

    nextChar = readNextChar();

    if (nextChar == '[') 
    {
      parseDependency(tokenBuffer, nextChar);

      parseSpace(tokenBuffer, nextChar);
    }
    else
    {
      parseScript(tokenBuffer, nextChar);
    }
  }; 

  for (int token: tokenBuffer) {
    std::cout << reprToken(token) << ' ';
  };
  std::cout << std::endl;

  return tokenBuffer;
}

#endif