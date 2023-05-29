#ifndef STYIO_AST_H_
#define STYIO_AST_H_

/*
StyioAST
*/
class StyioAST {
  public:
    virtual ~StyioAST() {}

    virtual std::string toString() {
      return "Styio.Base {}";
    }

    virtual std::string toStringInline() {
      return "Styio.Base {}";
    }
};

/*
IdAST
*/
class IdAST : public StyioAST {
  std::string Id;

  public:
    IdAST(const std::string &id) : Id(id) {}

    std::string toString() {
      return std::string("ID { ") + Id + " }";
    }

    std::string toStringInline() {
      return "<ID: " + Id + ">";
    }
};

/*
IntAST
*/
class IntAST : public StyioAST {
  int Value;

  public:
    IntAST(int val) : Value(val) {}

    std::string toString() {
      return "Int { " + std::to_string(Value) + " }";
    }

    std::string toStringInline() {
      return "<Int: " + std::to_string(Value) + ">";
    }
};

/*
FloatAST
*/
class FloatAST : public StyioAST {
  double Value;

  public:
    FloatAST(double val) : Value(val) {}

    std::string toString() {
      return "Float { " + std::to_string(Value) + " }";
    }

    std::string toStringInline() {
      return "<Float: " + std::to_string(Value) + ">";
    }
};

/*
StringAST
*/
class StringAST : public StyioAST {
  std::string Value;

  public:
    StringAST(std::string val) : Value(val) {}

    std::string toString() {
      return "String { \"" + Value + "\" }";
    }

    std::string toStringInline() {
      return "<String: \"" + Value + "\">";
    }
};

/*
AssignAST
*/
class AssignAST : public StyioAST {
  IdAST* varId;
  StyioAST* valExpr;

  public:
    AssignAST(IdAST* var, StyioAST* val) : varId(var), valExpr(val) {}

    std::string toString(int indent = 2) {
      return std::string("Assign {\n") 
        + std::string(indent, ' ') + "| Var: " 
        + varId -> toString() 
        + "\n"
        + std::string(indent, ' ') + "| Op:  " 
        + ":="
        + "\n"
        + std::string(indent, ' ') + "| Val: " 
        + valExpr -> toString() 
        + "\n"
        + "}";
    }
};

/*
BinOpAST
*/
class BinOpAST : public StyioAST {
  StyioToken Op;
  StyioAST *LHS;
  StyioAST *RHS;

  public:
    BinOpAST(StyioToken op, StyioAST* lhs, StyioAST* rhs): Op(op), LHS(lhs), RHS(rhs) {}

    std::string toString(int indent = 2) {
      return std::string("BinOp {\n") 
        + std::string(indent, ' ') + "| LHS: "
        + LHS -> toString() 
        + "\n"
        + std::string(indent, ' ') + "| Op:  "
        + reprToken(Op)
        + "\n"
        + std::string(indent, ' ') + "| RHS: "
        + RHS -> toString()  
        + "\n} ";
    }
};

/*
VarDefAST
*/
class VarDefAST : public StyioAST {
  std::vector<IdAST*> Vars;

  public:
    VarDefAST(std::vector<IdAST*> vars): Vars(vars) {}

    std::string toString(int indent = 2) {
      std::string varStr;

      for (std::vector<IdAST*>::iterator it = Vars.begin(); 
        it != Vars.end(); 
        ++it
      ) {
        varStr += std::string(indent, ' ') + "| ";
        varStr += (*it) -> toStringInline();
        varStr += "\n";
      };

      return std::string("Variable Definition {\n")
        + varStr
        + "\n} ";
    }
};

/*
ValExprAST
*/
class ValExprAST : public StyioAST {
  StyioAST* Value;

  public:
    ValExprAST(StyioAST* value): Value(value) {}

    std::string toString(int indent = 2) {
      return std::string("ValExpr { ") + Value -> toString() + " } ";
    }
};

/*
BlockAST
*/
class BlockAST : public StyioAST {
  std::vector<StyioAST*> Stmts;
  StyioAST* Expr;

  public:
    BlockAST() {}

    BlockAST(StyioAST* expr): Expr(expr) {}

    BlockAST(std::vector<StyioAST*> stmts, StyioAST* expr): Stmts(stmts), Expr(expr) {}

    std::string toString(int indent = 2) {
      std::string stmtStr;

      for (std::vector<StyioAST*>::iterator it = Stmts.begin(); 
        it != Stmts.end(); 
        ++it
      ) {
        stmtStr += (*it) -> toStringInline();
        stmtStr += "\n";
      };

      return std::string("Block {\n")
        
        + std::string(indent, ' ') + "| Stmts: "
        + stmtStr
        + "\n"
        + std::string(indent, ' ') + "| Expr:  "
        + Expr -> toString()  
        + "\n} ";
    }
};

/*
DependencyAST
*/
class DependencyAST : public StyioAST {
  std::vector<std::string> DependencyPaths;

  public:
    DependencyAST(std::vector<std::string> paths): DependencyPaths(paths) {}

    std::string toString(int indent = 2) {
      std::string dependencyPathsStr;

      for(int i=0; i < DependencyPaths.size(); i++) {
        dependencyPathsStr += std::string(indent, ' ') + "| ";
        dependencyPathsStr += DependencyPaths[i];
        dependencyPathsStr += "\n";
      };

      return std::string("Dependencies {\n")
        + dependencyPathsStr
        + "\n} ";
    }
};

#endif