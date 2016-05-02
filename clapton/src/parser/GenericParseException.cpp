#include "parser/GenericParseException.h"
#include "util/StringConversion.h"

namespace Lonely {


GenericParseException::GenericParseException(const std::string& prob__,
                        int lineNum__)
  : prob_(prob__),
    lineNum_(lineNum__) { };
  
const char* GenericParseException::what() const throw() {
  return (std::string("At line ") 
      + StringConversion::toString(lineNum_)
      + ": "
      + prob_).c_str();
}


};
