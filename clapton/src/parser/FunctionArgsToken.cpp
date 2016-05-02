#include "parser/FunctionArgsToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


FunctionArgsToken::FunctionArgsToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType FunctionArgsToken::type() const {
  return ParseTokenTypes::FunctionArgs;
}

int FunctionArgsToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;
  
  // FunctionArg
  token = readTokenOrFail(ParseTokenTypes::FunctionArg,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  // Functions can have zero arguments
  if (token == NULL) {
    return pos;
  }
  
  // FunctionArg FunctionArgs
  while (token != NULL) {
    advance(str, pos, lineNum);
    if (!(charAtPosIs(str, pos, ','))) {
      break;
    }
    else {
      ++pos;
    }
    
    advance(str, pos, lineNum);
    token = readTokenOrFail(ParseTokenTypes::FunctionArg,
                            str,
                            pos,
                            lineNum,
                            parseStateData);
  }
  
  return pos;
}
  
ParseToken& FunctionArgsToken::arg(int index) {
  return *(subtokens_[index]->subtokens()[0]);
}


};
