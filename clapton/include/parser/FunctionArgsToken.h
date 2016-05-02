#ifndef FUNCTIONARGSTOKEN_H
#define FUNCTIONARGSTOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class FunctionArgsToken : public ParseToken {
public:
  FunctionArgsToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  ParseToken& arg(int index);
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  char letter_;
};


};


#endif
