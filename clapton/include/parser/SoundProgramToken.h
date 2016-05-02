#ifndef SOUNDPROGRAMTOKEN_H
#define SOUNDPROGRAMTOKEN_H


#include "parser/ParseToken.h"

namespace Lonely {


class SoundProgramToken : public ParseToken {
public:
  SoundProgramToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
