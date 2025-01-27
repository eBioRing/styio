#pragma once
#ifndef STYIO_EXCEPTION_H_
#define STYIO_EXCEPTION_H_

class StyioBaseException : public std::exception
{
  private:
    std::string message;

  public:
    StyioBaseException() : message("Styio.BaseException: Undefined."){}

    StyioBaseException(std::string msg) : message("Styio.BaseException: " + msg) {}

    ~StyioBaseException() throw () {}

    virtual const char* what() const throw () {
      return message.c_str();
    }
};

class StyioSyntaxError : public StyioBaseException 
{
  private:
    std::string message;

  public:
    StyioSyntaxError() : message("\nStyio.SyntaxError: Undefined."){}

    StyioSyntaxError(std::string msg) : message("\nStyio.SyntaxError: " + msg) {}

    ~StyioSyntaxError() throw () {}

    virtual const char* what() const throw () {
      return message.c_str();
    }
};

class StyioParseError : public StyioBaseException 
{
  private:
    std::string message;

  public:
    StyioParseError() : message("\nStyio.ParseError: Undefined."){}

    StyioParseError(std::string msg) : message("\nStyio.ParseError: " + msg) {}

    ~StyioParseError() throw () {}

    virtual const char* what() const throw () {
      return message.c_str();
    }
};

class StyioNotImplemented : public StyioBaseException 
{
  private:
    std::string message;

  public:
    StyioNotImplemented() : message("\nStyio.NotImplemented: Undefined."){}

    StyioNotImplemented(std::string msg) : message("\nStyio.NotImplemented: " + msg) {}

    ~StyioNotImplemented() throw () {}

    virtual const char* what() const throw () {
      return message.c_str();
    }
};

#endif