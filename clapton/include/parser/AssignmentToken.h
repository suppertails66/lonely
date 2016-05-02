#ifndef ASSIGNMENTTOKEN_H
#define ASSIGNMENTTOKEN_H


#include "parser/ParseToken.h"
#include "parser/SoundDataToken.h"
#include "parser/NumberLiteralToken.h"

namespace Lonely {


class AssignmentToken : public ParseToken {
public:
  AssignmentToken();
  
  virtual ParseTokenTypes::ParseTokenType type() const;
protected:
  void doNumberLiteralAssignment(ParseStateData& parseStateData,
                                 int lineNum,
                                 const std::string& name,
                                 NumberLiteralToken& numberLiteralToken);
                             
  void doSoundDataAssignment(ParseStateData& parseStateData,
                             int lineNum,
                             const std::string& name,
                             SoundDataToken& soundDataToken);
  
  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData);
};


};


#endif
