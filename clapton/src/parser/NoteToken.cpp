#include "parser/NoteToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/NoteIdentifierToken.h"
#include "parser/NumberLiteralToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


NoteToken::NoteToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType NoteToken::type() const {
  return ParseTokenTypes::Note;
}

int NoteToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // NoteIdentifier NumberLiteral
  
  token = readTokenOrFail(ParseTokenTypes::NoteIdentifier,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
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
  
void NoteToken::doSoundDataAssignment(ParseStateData& parseStateData,
               int lineNum,
               ParseConstants::Channel channel) {
  Tbyte noteValue = laylaNoteIdentifier();
  Tbyte noteDuration = laylaDurationIdentifier();
  
  parseStateData.appendToChannel(
      channel,
      *(new SoundNoteToken(noteValue, noteDuration)));
}
  
Tbyte NoteToken::laylaNoteIdentifier() const {
  NoteIdentifierToken& noteIdentifier
    = dynamic_cast<NoteIdentifierToken&>(*(subtokens_[0]));
    
  if (noteIdentifier.isRest()) {
    return NoteNameValues::restValue;
  }
  
  NoteNameValues::NoteNameValue baseNote = noteIdentifier.baseNote();
  int octave = noteIdentifier.octave();
  
  // In Layla notation, 0 = C, octave 0, 1 = C#, octave 0, ...
  // The enum is already set up to reflect this, so we need only
  // account for the octave value.
  return (baseNote + (octave * NoteNameValues::numValues));
}

Tbyte NoteToken::laylaDurationIdentifier() const {
  return dynamic_cast<NumberLiteralToken&>(*(subtokens_[1]))
    .number();
}


};
