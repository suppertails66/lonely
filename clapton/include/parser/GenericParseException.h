#ifndef GENERICPARSEEXCEPTION_H
#define GENERICPARSEEXCEPTION_H


#include <exception>
#include <string>
#include "parser/ParseToken.h"
#include "parser/ParseTokenType.h"

namespace Lonely {


class GenericParseException : public std::exception {
public:
  GenericParseException(const std::string& prob__,
                        int lineNum__);
  
  const char* what() const throw();
protected:
  std::string prob_;
  int lineNum_;
};


};


#endif
