#include "parser/WrongTokenException.h"
#include "util/StringConversion.h"

namespace Lonely {


WrongTokenException::WrongTokenException(
                      ParseTokenTypes::ParseTokenType expectedTokenType__,
                      ParseTokenTypes::ParseTokenType sourceTokenType__,
                      int lineNum__)
  : expectedTokenType_(expectedTokenType__),
    sourceTokenType_(sourceTokenType__),
    lineNum_(lineNum__) { };

const char* WrongTokenException::what() const throw() {
  std::string str("At line ");
  str += StringConversion::toString(lineNum_);
  str += ": ";
  str += "While reading token of type "
    + ParseTokenTypes::nameOfType(sourceTokenType_)
    + ", could not read token of type "
    + ParseTokenTypes::nameOfType(expectedTokenType_);

  return str.c_str();
}


};
