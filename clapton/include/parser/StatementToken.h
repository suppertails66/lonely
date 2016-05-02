#ifndef STATEMENTTOKEN_H
#define STATEMENTTOKEN_H


#include "parser/ParseToken.h"

namespace Lonely {


class ControlTriggerToken;

class StatementToken : public ParseToken {
public:
  StatementToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  void callFunction(
              ParseStateData& parseStateData,
              ControlTriggerToken& controlTriggerToken,
              int lineNum);
              
};


};


#endif
