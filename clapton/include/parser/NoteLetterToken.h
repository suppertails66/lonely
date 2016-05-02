#ifndef NOTELETTERTOKEN_H
#define NOTELETTERTOKEN_H


#include "parser/ParseToken.h"
#include <string>

namespace Lonely {


class NoteLetterToken : public ParseToken {
public:
  NoteLetterToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  char letter() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  char letter_;
};


};


#endif
