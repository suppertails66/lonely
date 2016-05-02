#ifndef WRONGARGTYPEEXCEPTION_H
#define WRONGARGTYPEEXCEPTION_H


#include <exception>
#include <string>
#include "util/StringConversion.h"
#include "parser/ParseTokenType.h"

namespace Lonely {


class WrongArgTypeException : public std::exception {
public:
  WrongArgTypeException(const std::string& functionName__,
                        int argNum__,
                        const std::string& expectedType__,
                        const std::string& actualType__,
                        int lineNum__);
  
  WrongArgTypeException(const std::string& functionName__,
                        int argNum__,
                        ParseTokenTypes::ParseTokenType expectedType__,
                        ParseTokenTypes::ParseTokenType actualType__,
                        int lineNum__);
                        
  const char* what() const throw();
protected:
  std::string functionName_;
  int argNum_;
  std::string expectedType_;
  std::string actualType_;
  int lineNum_;
};


};


#endif
