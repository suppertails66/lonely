#ifndef NOTEIDENTIFIERTOKEN_H
#define NOTEIDENTIFIERTOKEN_H


#include "parser/ParseToken.h"
#include "parser/NoteNameValue.h"
#include <string>

namespace Lonely {


class NoteIdentifierToken : public ParseToken {
public:
  NoteIdentifierToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  NoteNameValues::NoteNameValue baseNote() const;
  
  int octave() const;
  
  bool isRest() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
