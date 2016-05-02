#include "parser/SoundCommandToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/NoteToken.h"
#include "parser/ControlTriggerToken.h"
#include "parser/PositionLabelToken.h"
#include "util/StringConversion.h"
#include <cctype>
#include <iostream>

namespace Lonely {


SoundCommandToken::SoundCommandToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType SoundCommandToken::type() const {
  return ParseTokenTypes::SoundCommand;
}

int SoundCommandToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // Note
  token = readTokenOrFail(ParseTokenTypes::Note,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token != NULL) {
    return pos;
  }
  
  // ControlTrigger
  token = readTokenOrFail(ParseTokenTypes::ControlTrigger,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token != NULL) {
    return pos;
  }
  
  // PositionLabel
  token = readTokenOrFail(ParseTokenTypes::PositionLabel,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token != NULL) {
    return pos;
  }
  
  return -1;
}
  
void SoundCommandToken::doSoundDataAssignment(ParseStateData& parseStateData,
               int lineNum,
               ParseConstants::Channel channel) {
  switch (subtokens_[0]->type()) {
  case ParseTokenTypes::Note:
    dynamic_cast<NoteToken&>(*(subtokens_[0]))
      .doSoundDataAssignment(
                parseStateData, lineNum, channel);
    break;
  case ParseTokenTypes::ControlTrigger:
    dynamic_cast<ControlTriggerToken&>(*(subtokens_[0]))
      .doSoundDataAssignment(
                parseStateData, lineNum, channel);
    break;
  case ParseTokenTypes::PositionLabel:
    dynamic_cast<PositionLabelToken&>(*(subtokens_[0]))
      .doSoundDataAssignment(
                parseStateData, lineNum, channel);
    break;
  default:
    break;
  }
}
  
bool SoundCommandToken::isLabel() const {
  if (subtokens_[0]->type() == ParseTokenTypes::PositionLabel) {
    return true;
  }
  
  return false;
}


};
