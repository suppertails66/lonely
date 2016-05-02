#include "parser/ExpressionsToken.h"
#include "parser/WrongTokenException.h"

namespace Lonely {


ExpressionsToken::ExpressionsToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType ExpressionsToken::type() const {
  return ParseTokenTypes::Expressions;
}

int ExpressionsToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // Expression
  token = readTokenOrFail(ParseTokenTypes::Expression,
                                      str,
                                      pos,
                                      lineNum,
                                      parseStateData);
  
  if (token == NULL) {
    throw WrongTokenException(ParseTokenTypes::Expression,
                              type(),
                              lineNum);
  }
  
  // Expression Expressions
  token = readTokenOrFail(ParseTokenTypes::Expressions,
                                      str,
                                      pos,
                                      lineNum,
                                      parseStateData);
  
  return pos;
}


};
