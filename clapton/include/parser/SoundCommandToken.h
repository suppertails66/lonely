#ifndef SOUNDCOMMANDTOKEN_H
#define SOUNDCOMMANDTOKEN_H


#include "parser/ParseToken.h"
#include "parser/ParseConstants.h"
#include <string>

namespace Lonely {


class SoundCommandToken : public ParseToken {
public:
  SoundCommandToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  void doSoundDataAssignment(ParseStateData& parseStateData,
                 int lineNum,
                 ParseConstants::Channel channel);
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  bool isLabel() const;
};


};


#endif
