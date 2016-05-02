#ifndef NUMBERDIGITSEQUENCETOKEN_H
#define NUMBERDIGITSEQUENCETOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class NumberDigitSequenceToken : public ParseToken {
public:
  NumberDigitSequenceToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  int number() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  int number_;
};


};


#endif
