#include "parser/BadTerminalException.h"
#include "util/StringConversion.h"

namespace Lonely {


BadTerminalException::BadTerminalException(
                      char invalidTerminal__,
                      ParseTokenTypes::ParseTokenType sourceTokenType__,
                      int lineNum__)
  : invalidTerminal_(invalidTerminal__),
    sourceTokenType_(sourceTokenType__),
    lineNum_(lineNum__) { };

const char* BadTerminalException::what() const throw() {
  std::string str("At line ");
  str += StringConversion::toString(lineNum_);
  str += ":\n";
  str += "Unexpected terminal '"
    + std::string(&invalidTerminal_, 1)
    + "' while reading token of type  "
    + StringConversion::toString(sourceTokenType_)
    + "\n";

  return str.c_str();
}


};
