#ifndef ALPHNUMCHARACTERSTOKEN_H
#define ALPHNUMCHARACTERSTOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class AlphNumCharactersToken : public ParseToken {
public:
  AlphNumCharactersToken();
  
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
