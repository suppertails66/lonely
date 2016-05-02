#include "parser/NoteNameToken.h"
#include "parser/NoteNameValue.h"
#include "parser/NoteLetterToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


NoteNameToken::NoteNameToken()
  : ParseToken(),
    sharp_(false),
    flat_(false) { };
  

ParseTokenTypes::ParseTokenType NoteNameToken::type() const {
  return ParseTokenTypes::NoteName;
}

int NoteNameToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // NoteLetter
  // NoteLetter HASH
  // NoteLetter CHAR_b
  // NoteLetter CHAR_B
  
  token = readTokenOrFail(ParseTokenTypes::NoteLetter,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  // Rests aren't sharp or flat
  if ((charAtPosIs(str, pos, 'r'))) {
    ++pos;
  }
  else if ((charAtPosIs(str, pos, '#'))
       || (charAtPosIs(str, pos, '+'))) {
    sharp_ = true;
    ++pos;
  }
  else if ((charAtPosIs(str, pos, 'b'))
           || (charAtPosIs(str, pos, 'B'))
           || (charAtPosIs(str, pos, '-'))) {
    flat_ = true;
    ++pos;
  }
  
  return pos;
}
  
NoteNameValues::NoteNameValue NoteNameToken::noteNameValue() const {
  const NoteLetterToken& noteLetterToken
    = dynamic_cast<const NoteLetterToken&>(*(subtokens_[0]));
  
  return NoteNameValues::baseValue(noteLetterToken.letter(),
                                   sharp_,
                                   flat_);
}
  
bool NoteNameToken::isRest() const {
  const NoteLetterToken& noteLetterToken
    = dynamic_cast<const NoteLetterToken&>(*(subtokens_[0]));
  
  if (tolower(noteLetterToken.letter()) == 'r') {
    return true;
  }
  
  return false;
}


};
