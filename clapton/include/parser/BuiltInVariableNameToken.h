#ifndef BUILTINVARIABLENAMETOKEN_H
#define BUILTINVARIABLENAMETOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class BuiltInVariableNameToken : public ParseToken {
public:
  BuiltInVariableNameToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  std::string variableName() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
