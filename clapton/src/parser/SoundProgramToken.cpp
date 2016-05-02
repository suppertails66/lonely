#include "parser/SoundProgramToken.h"

namespace Lonely {


SoundProgramToken::SoundProgramToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType SoundProgramToken::type() const {
  return ParseTokenTypes::SoundProgram;
}

int SoundProgramToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  // EOF
  if (pos >= str.size()) {
    return pos;
  }
  
  // Expressions EOF
  ParseToken* token = NULL;
  token = readTokenOrFail(ParseTokenTypes::Expressions,
                                      str,
                                      pos,
                                      lineNum,
                                      parseStateData);
  
  if (token == NULL) {
    throw WrongTokenException(ParseTokenTypes::Expressions,
                              type(),
                              lineNum);
  }
  
  return pos;
}


};
