#ifndef MACRONAMETOKEN_H
#define MACRONAMETOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class MacroNameToken : public ParseToken {
public:
  MacroNameToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  std::string macroName() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
