#ifndef EXPRESSIONTOKEN_H
#define EXPRESSIONTOKEN_H


#include "parser/ParseToken.h"

namespace Lonely {


class ExpressionToken : public ParseToken {
public:
  ExpressionToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
