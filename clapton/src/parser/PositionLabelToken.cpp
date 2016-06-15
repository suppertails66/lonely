#include "parser/PositionLabelToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


PositionLabelToken::PositionLabelToken()
  : ParseToken(),
    plus(false) { };
  

ParseTokenTypes::ParseTokenType PositionLabelToken::type() const {
  return ParseTokenTypes::PositionLabel;
}

int PositionLabelToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;
  
  // CARAT AlphNumCharacters
  if ((charAtPosIs(str, pos, '^'))) {
    ++pos;
  }
  // PLUS AlphNumCharacters
  // (hack: jump to 
  else if ((charAtPosIs(str, pos, '+'))) {
    ++pos;
    plus = true;
  }
  else {
    return -1;
  }
  
  advance(str, pos, lineNum);
  token = readTokenOrFail(ParseTokenTypes::AlphNumCharacters,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  return pos;
}
  
void PositionLabelToken::doSoundDataAssignment(ParseStateData& parseStateData,
               int lineNum,
               ParseConstants::Channel channel) {
  parseStateData.addPositionLabelAtChannelPos(
      channel,
      dynamic_cast<AlphNumCharactersToken&>(*(subtokens_[0]))
        .characters(),
      plus ? 3 : 0);
}
  
const std::string& PositionLabelToken::label() const {
  return dynamic_cast<AlphNumCharactersToken&>(*(subtokens_[0]))
        .characters();
}


};
