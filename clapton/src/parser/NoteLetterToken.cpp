#include "parser/NoteLetterToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


NoteLetterToken::NoteLetterToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType NoteLetterToken::type() const {
  return ParseTokenTypes::NoteLetter;
}

int NoteLetterToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // A, B, C ...
  letter_ = tolower(str[pos]);
  
  switch (letter_) {
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'r':
    ++pos;
    break;
  default:
    return -1;
    break;
  }
  
  return pos;
}
  
char NoteLetterToken::letter() const {
  return letter_;
}


};
