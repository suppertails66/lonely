#ifndef WRONGNUMARGSEXCEPTION_H
#define WRONGNUMARGSEXCEPTION_H


#include <exception>
#include <string>
#include "util/StringConversion.h"

namespace Lonely {


class WrongNumArgsException : public std::exception {
public:
  WrongNumArgsException(const std::string& functionName__,
                        int expectedArgs__,
                        int actualArgs__,
                        int lineNum__);
                        
  const char* what() const throw();
protected:
  std::string functionName_;
  int expectedArgs_;
  int actualArgs_;
  int lineNum_;
};


};


#endif
