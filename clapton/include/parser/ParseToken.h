#ifndef PARSETOKEN_H
#define PARSETOKEN_H


#include <string>
#include "parser/ParseStateData.h"
#include "parser/ParseTokenCollection.h"
#include "parser/ParseTokenType.h"
#include "parser/WrongTokenException.h"
#include "parser/BadTerminalException.h"
#include "parser/GenericParseException.h"
#include <iostream>

namespace Lonely {


class ParseToken {
public:
  ParseToken();
  
  virtual ~ParseToken();
  
  // Try to interpret the token data at pos in str as a token of
  // a type determined by the derived class. If this is successful,
  // the return value is true and pos is advanced to the point where
  // token interpretation ended; otherwise, the return value is false,
  // though pos may still be updated if EOF was reached.
  // The other parameters passed by reference are similarly maintained.
  // Note that str may be modified by macro substitution.
  virtual bool fromString(std::string& str,
                          int& pos,
                          int& lineNum,
                          ParseStateData& parseStateData);
  
  virtual ParseTokenTypes::ParseTokenType type() const =0;
  
  ParseTokenCollection& subtokens();
  const ParseTokenCollection& subtokens() const;
protected:
  static int advance(const std::string& str, int& pos, int& lineNum);
  
  static bool charAtPosIs(const std::string& str,
                          int pos,
                          char checkValue);
  
  ParseTokenCollection subtokens_;

  virtual int fromStringStep(std::string& str,
                             int pos,
                             int& lineNum,
                             ParseStateData& parseStateData) =0;
  
  // Try to read a token of the specified tokenType from the given
  // string. On success, return a pointer to the token; on failure,
  // return NULL. Values passed by reference are updated appropriately.
  virtual ParseToken* readTokenOrFail(ParseTokenTypes::ParseTokenType
                                        tokenType,
                                     std::string& str,
                                     int& pos,
                                     int& lineNum,
                                     ParseStateData& parseStateData);
  
};


};


#endif
