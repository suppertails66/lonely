#ifndef CONTROLTRIGGERTOKEN_H
#define CONTROLTRIGGERTOKEN_H


#include "parser/ParseToken.h"
#include "parser/ParseConstants.h"
#include <string>

namespace Lonely {


class FunctionArgsToken;

class ControlTriggerToken : public ParseToken {
public:
  ControlTriggerToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  void doSoundDataAssignment(ParseStateData& parseStateData,
                 int lineNum,
                 ParseConstants::Channel channel);
  
  const std::string& name() const;
  
  FunctionArgsToken& args();
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
