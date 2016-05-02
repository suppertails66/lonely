#ifndef STRINGLITERALTOKEN_H
#define STRINGLITERALTOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class StringLiteralToken : public ParseToken {
public:
  StringLiteralToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  std::string& characters();
  const std::string& characters() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  std::string characters_;
};


};


#endif
