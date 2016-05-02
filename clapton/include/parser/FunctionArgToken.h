#ifndef FUNCTIONARGTOKEN_H
#define FUNCTIONARGTOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class FunctionArgToken : public ParseToken {
public:
  FunctionArgToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  char letter_;
};


};


#endif
