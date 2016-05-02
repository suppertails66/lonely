#include "parser/StringLiteralToken.h"
#include "parser/WrongTokenException.h"
#include <cctype>
#include <iostream>

namespace Lonely {


StringLiteralToken::StringLiteralToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType StringLiteralToken::type() const {
  return ParseTokenTypes::StringLiteral;
}

int StringLiteralToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  characters_ = "";
                           
  ParseToken* token = NULL;
  
  if (charAtPosIs(str, pos, '"')) {
    ++pos;
  }
  else {
    return -1;
  }

  // Character
  if (pos >= str.size()) {
    return -1;
  }
  else {
    characters_ += str[pos];
    ++pos;
  }
  
  // AlphNumCharacter StringLiteral
  while ((pos < str.size())
         && (str[pos] != '"')) {
    characters_ += str[pos];
    
    if (str[pos] == '\n') {
      ++lineNum;
    }
    
    ++pos;
  }
  
  // Skip trailing quote
  if (pos < str.size()) {
    ++pos;
  }
  else {
    throw GenericParseException("No terminating quote for string literal",
                                lineNum);
  }
  
  return pos;
}
  
std::string& StringLiteralToken::characters() {
  return characters_;
}

const std::string& StringLiteralToken::characters() const {
  return characters_;
}


};
