#include "parser/AlphNumCharactersToken.h"
#include "parser/WrongTokenException.h"
#include <cctype>
#include <iostream>

namespace Lonely {


AlphNumCharactersToken::AlphNumCharactersToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType AlphNumCharactersToken::type() const {
  return ParseTokenTypes::AlphNumCharacters;
}

int AlphNumCharactersToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  characters_ = "";
                           
  ParseToken* token = NULL;

  // AlphNumCharacter
  if ((isalpha(str[pos]))
      || (isdigit(str[pos]))
      || (str[pos] == '_')
      || (str[pos] == '-')) {
    characters_ += str[pos];
    ++pos;
  }
  else {
    return -1;
  }
  
  // AlphNumCharacter AlphNumCharacters
  while ((pos < str.size())
         && ((isalpha(str[pos]))
             || (isdigit(str[pos])))
             || (str[pos] == '_')
             || (str[pos] == '-')) {
    characters_ += str[pos];
    ++pos;
  }
  
  return pos;
}
  
std::string& AlphNumCharactersToken::characters() {
  return characters_;
}

const std::string& AlphNumCharactersToken::characters() const {
  return characters_;
}


};
