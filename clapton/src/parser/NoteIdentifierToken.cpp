#include "parser/NoteIdentifierToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/NumberLiteralToken.h"
#include "parser/NoteNameToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


NoteIdentifierToken::NoteIdentifierToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType NoteIdentifierToken::type() const {
  return ParseTokenTypes::NoteIdentifier;
}

int NoteIdentifierToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // NoteName NumberLiteral
  
  token = readTokenOrFail(ParseTokenTypes::NoteName,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  if (dynamic_cast<NoteNameToken&>(*token).isRest()) {
    return pos;
  }
  
  advance(str, pos, lineNum);
  token = readTokenOrFail(ParseTokenTypes::NumberLiteral,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  return pos;
}
  
NoteNameValues::NoteNameValue NoteIdentifierToken::baseNote() const {
  const NoteNameToken& noteNameToken
    = dynamic_cast<const NoteNameToken&>(*(subtokens_[0]));
  
  return noteNameToken.noteNameValue();
}

int NoteIdentifierToken::octave() const {
  const NumberLiteralToken& numberLiteralToken
    = dynamic_cast<const NumberLiteralToken&>(*(subtokens_[1]));
  
  return numberLiteralToken.number();
}
  
bool NoteIdentifierToken::isRest() const {
  const NoteNameToken& noteNameToken
    = dynamic_cast<const NoteNameToken&>(*(subtokens_[0]));
  
  return noteNameToken.isRest();
}


};
