#include "parser/ExpressionToken.h"
#include "parser/WrongTokenException.h"

namespace Lonely {


ExpressionToken::ExpressionToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType ExpressionToken::type() const {
  return ParseTokenTypes::Expression;
}

int ExpressionToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // Statement SEMICOLON
  token = readTokenOrFail(ParseTokenTypes::Statement,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    throw WrongTokenException(ParseTokenTypes::Statement,
                              type(),
                              lineNum);
  }
  
  advance(str, pos, lineNum);
  // Statement must end with a semicolon
  if (charAtPosIs(str, pos, ';')) {
    ++pos;
  }
  else {
    throw BadTerminalException(str[pos],
                               type(),
                               lineNum);
  }
  
  return pos;
}


};
