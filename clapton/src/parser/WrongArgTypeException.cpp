#include "parser/WrongArgTypeException.h"
#include "util/StringConversion.h"

namespace Lonely {


WrongArgTypeException::WrongArgTypeException(
                        const std::string& functionName__,
                        int argNum__,
                        const std::string& expectedType__,
                        const std::string& actualType__,
                        int lineNum__)
  : functionName_(functionName__),
    argNum_(argNum__),
    expectedType_(expectedType__),
    actualType_(actualType__),
    lineNum_(lineNum__) { };
  
WrongArgTypeException::WrongArgTypeException(
                      const std::string& functionName__,
                      int argNum__,
                      ParseTokenTypes::ParseTokenType expectedType__,
                      ParseTokenTypes::ParseTokenType actualType__,
                      int lineNum__)
  : functionName_(functionName__),
    argNum_(argNum__),
    expectedType_(ParseTokenTypes::nameOfType(expectedType__)),
    actualType_(ParseTokenTypes::nameOfType(actualType__)),
    lineNum_(lineNum__) { };
                        
const char* WrongArgTypeException::what() const throw() {
  return (std::string("At line ")
            + StringConversion::toString(lineNum_)
            + ": "
            + "Argument "
            + StringConversion::toString(argNum_)
            + " to function "
            + functionName_
            + "has wrong type (expected "
            + expectedType_
            + " , got "
            + actualType_
            + ")").c_str();
}


};
