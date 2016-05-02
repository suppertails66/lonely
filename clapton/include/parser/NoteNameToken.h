#ifndef NOTENAMETOKEN_H
#define NOTENAMETOKEN_H


#include "parser/ParseToken.h"
#include "parser/NoteNameValue.h"
#include <string>

namespace Lonely {


class NoteNameToken : public ParseToken {
public:
  NoteNameToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  NoteNameValues::NoteNameValue noteNameValue() const;
  
  bool isRest() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
  
  bool sharp_;
  bool flat_;
};


};


#endif
