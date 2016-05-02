#include "parser/NumberLiteralToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/NumberDigitSequenceToken.h"

namespace Lonely {


NumberLiteralToken::NumberLiteralToken()
  : ParseToken(),
    positive_(true) { };
  

ParseTokenTypes::ParseTokenType NumberLiteralToken::type() const {
  return ParseTokenTypes::NumberLiteral;
}

int NumberLiteralToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // MINUS NumberDigitSequence
  // PLUS NumberDigitSequence
  if (charAtPosIs(str, pos, '-')) {
    positive_ = false;
    ++pos;
  }
  else if (charAtPosIs(str, pos, '+')) {
    positive_ = true;
    ++pos;
  }
  
  // NumberDigitSequence
  token = readTokenOrFail(ParseTokenTypes::NumberDigitSequence,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  return pos;
}
  
int NumberLiteralToken::number() const {
  int rawNumber = dynamic_cast<NumberDigitSequenceToken&>(*(subtokens_[0]))
    .number();
  
  if (positive_) {
    return +rawNumber;
  }
  else {
    return -rawNumber;
  }
}


};
