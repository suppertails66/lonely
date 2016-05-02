#ifndef NUMBERLITERALTOKEN_H
#define NUMBERLITERALTOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class NumberLiteralToken : public ParseToken {
public:
  NumberLiteralToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  int number() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  bool positive_;
};


};


#endif
