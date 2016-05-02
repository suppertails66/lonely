#include "parser/FunctionArgToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


FunctionArgToken::FunctionArgToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType FunctionArgToken::type() const {
  return ParseTokenTypes::FunctionArg;
}

int FunctionArgToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;
  
  // NumberLiteral
  token = readTokenOrFail(ParseTokenTypes::NumberLiteral,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token != NULL) {
    return pos;
  }
  
  // PositionLabel
  token = readTokenOrFail(ParseTokenTypes::PositionLabel,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token != NULL) {
    return pos;
  }
  
  return -1;
}


};
