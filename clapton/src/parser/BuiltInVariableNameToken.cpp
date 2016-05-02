#include "parser/BuiltInVariableNameToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"

namespace Lonely {


BuiltInVariableNameToken::BuiltInVariableNameToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType BuiltInVariableNameToken::type() const {
  return ParseTokenTypes::BuiltInVariableName;
}

int BuiltInVariableNameToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // DOLLAR_SIGN AlphNumCharacters
  
  if (charAtPosIs(str, pos, '_')) {
    ++pos;
  }
  else {
    return pos;
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
  
std::string BuiltInVariableNameToken::variableName() const {
  return dynamic_cast<AlphNumCharactersToken&>(*(subtokens_[0]))
    .characters();
}


};
