#include "parser/SoundCommandsToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/SoundCommandsToken.h"
#include "parser/SoundCommandToken.h"
#include "parser/PositionLabelToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


SoundCommandsToken::SoundCommandsToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType SoundCommandsToken::type() const {
  return ParseTokenTypes::SoundCommands;
}

int SoundCommandsToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // SoundCommand
  token = readTokenOrFail(ParseTokenTypes::SoundCommand,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  // SoundCommand SoundCommands
  while (token != NULL) {
    advance(str, pos, lineNum);
    
    token = readTokenOrFail(ParseTokenTypes::SoundCommand,
                            str,
                            pos,
                            lineNum,
                            parseStateData);
  }
  
  return pos;
}
  
void SoundCommandsToken
  ::doSoundDataAssignment(ParseStateData& parseStateData,
               int lineNum,
               ParseConstants::Channel channel) {

  for (int i = 0; i < subtokens_.size(); i++) {
    dynamic_cast<SoundCommandToken&>(
            *(subtokens_[i])).doSoundDataAssignment(
                parseStateData, lineNum, channel);
  }
  
/*  for (int i = 0; i < subtokens_.size(); i++) {
//    std::cout << subtokens_[i]->type() << std::endl;
    if (dynamic_cast<SoundCommandToken&>(
            *(subtokens_[i])).isLabel()) {
      PositionLabelToken& labelToken
        = dynamic_cast<PositionLabelToken&>(*(subtokens_[i]->subtokens()[0]));
      
//      labelToken.
    }
  } */
}


};
