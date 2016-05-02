#include "parser/WrongNumArgsException.h"
#include "util/StringConversion.h"

namespace Lonely {


WrongNumArgsException::WrongNumArgsException(
                        const std::string& functionName__,
                        int expectedArgs__,
                        int actualArgs__,
                        int lineNum__)
  : functionName_(functionName__),
    expectedArgs_(expectedArgs__),
    actualArgs_(actualArgs__),
    lineNum_(lineNum__) { };
                        
const char* WrongNumArgsException::what() const throw() {
  return (std::string("At line ")
            + StringConversion::toString(lineNum_)
            + ": "
            + "Wrong number of arguments to function "
            + functionName_
            + " (expected "
            + StringConversion::toString(expectedArgs_)
            + " , got "
            + StringConversion::toString(actualArgs_)
            + ")").c_str();
}


};
