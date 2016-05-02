#include "parser/MacroNameToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"

namespace Lonely {


MacroNameToken::MacroNameToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType MacroNameToken::type() const {
  return ParseTokenTypes::MacroName;
}

int MacroNameToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // DOLLAR_SIGN AlphNumCharacters
  
  if (charAtPosIs(str, pos, '$')) {
    ++pos;
  }
  else {
    return -1;
//    throw BadTerminalException(str[pos],
//                               type(),
//                               lineNum);
  }
  
  advance(str, pos, lineNum);
  token = readTokenOrFail(ParseTokenTypes::AlphNumCharacters,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    throw GenericParseException("Expected macro name",
                              lineNum);
  }
  
  return pos;
}
  
std::string MacroNameToken::macroName() const {
  return dynamic_cast<AlphNumCharactersToken&>(*(subtokens_[0]))
    .characters();
}


};
