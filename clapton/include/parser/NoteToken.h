#ifndef NOTETOKEN_H
#define NOTETOKEN_H


#include "parser/ParseToken.h"
#include "parser/ParseConstants.h"
#include "structs/Tbyte.h"
#include <string>

namespace Lonely {


class NoteToken : public ParseToken {
public:
  NoteToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
  
  void doSoundDataAssignment(ParseStateData& parseStateData,
                 int lineNum,
                 ParseConstants::Channel channel);
  
  Tbyte laylaNoteIdentifier() const;
  
  Tbyte laylaDurationIdentifier() const;
protected:
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
