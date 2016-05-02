#ifndef WRONGTOKENEXCEPTION_H
#define WRONGTOKENEXCEPTION_H


#include <exception>
#include "parser/ParseToken.h"
#include "parser/ParseTokenType.h"

namespace Lonely {


class WrongTokenException : public std::exception {
public:
  WrongTokenException(ParseTokenTypes::ParseTokenType expectedTokenType__,
                      ParseTokenTypes::ParseTokenType sourceTokenType__,
                      int lineNum__);
  
  const char* what() const throw();
protected:
  ParseTokenTypes::ParseTokenType expectedTokenType_;
  ParseTokenTypes::ParseTokenType sourceTokenType_;
  int lineNum_;
};


};


#endif
