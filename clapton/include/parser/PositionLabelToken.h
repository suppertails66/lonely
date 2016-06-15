#ifndef POSITIONLABELTOKEN_H
#define POSITIONLABELTOKEN_H


#include "parser/ParseToken.h"
#include "parser/ParseConstants.h"
#include <string>

namespace Lonely {


class PositionLabelToken : public ParseToken {
public:
  PositionLabelToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  void doSoundDataAssignment(ParseStateData& parseStateData,
                 int lineNum,
                 ParseConstants::Channel channel);
  
  const std::string& label() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  bool plus;
};


};


#endif
