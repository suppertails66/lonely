#ifndef BADTERMINALEXCEPTION_H
#define BADTERMINALEXCEPTION_H


#include <exception>
#include "parser/ParseToken.h"
#include "parser/ParseTokenType.h"

namespace Lonely {


class BadTerminalException : public std::exception {
public:
  BadTerminalException(char invalidTerminal__,
                      ParseTokenTypes::ParseTokenType sourceTokenType__,
                      int lineNum__);
  
  const char* what() const throw();
protected:
  char invalidTerminal_;
  ParseTokenTypes::ParseTokenType sourceTokenType_;
  int lineNum_;
};


};


#endif
