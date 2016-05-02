#ifndef EXPRESSIONSTOKEN_H
#define EXPRESSIONSTOKEN_H


#include "parser/ParseToken.h"

namespace Lonely {


class ExpressionsToken : public ParseToken {
public:
  ExpressionsToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
