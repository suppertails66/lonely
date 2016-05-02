#include "parser/SoundDataToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/SoundCommandsToken.h"
#include "parser/SoundCommandToken.h"
#include "parser/PositionLabelToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


SoundDataToken::SoundDataToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType SoundDataToken::type() const {
  return ParseTokenTypes::SoundData;
}

int SoundDataToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // OPEN_CURLYBRACE SoundCommands CLOSE_CURLYBRACE
  if (str[pos] != '{') {
    return -1;
  }
  else {
    ++pos;
  }
  
  advance(str, pos, lineNum);
  
  token = readTokenOrFail(ParseTokenTypes::SoundCommands,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  advance(str, pos, lineNum);
  
  if (str[pos] != '}') {
    return -1;
  }
  else {
    ++pos;
  }
  
  return pos;
}
  
void SoundDataToken::doSoundDataAssignment(ParseStateData& parseStateData,
                 int lineNum,
                 ParseConstants::Channel channel) {
  dynamic_cast<SoundCommandsToken&>(*(subtokens_[0]))
    .doSoundDataAssignment(parseStateData, lineNum, channel);
}


};
