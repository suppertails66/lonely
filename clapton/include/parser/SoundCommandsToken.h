#ifndef SOUNDCOMMANDSTOKEN_H
#define SOUNDCOMMANDSTOKEN_H


#include "parser/ParseToken.h"
#include "parser/ParseConstants.h"
#include <string>

namespace Lonely {


class SoundCommandsToken : public ParseToken {
public:
  SoundCommandsToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  void doSoundDataAssignment(ParseStateData& parseStateData,
                 int lineNum,
                 ParseConstants::Channel channel);
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
