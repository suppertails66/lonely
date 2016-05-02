#include "parser/NumberDigitSequenceToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


NumberDigitSequenceToken::NumberDigitSequenceToken()
  : ParseToken(),
    number_(0) { };
  

ParseTokenTypes::ParseTokenType NumberDigitSequenceToken::type() const {
  return ParseTokenTypes::NumberDigitSequence;
}

int NumberDigitSequenceToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;
  
  std::string numString;

  // ZERO CHAR_X Digits
  bool hexfmt = false;
  if ((pos < str.size() - 2)
      && (str[pos] == '0')
      && ((str[pos + 1] == 'x'
          || str[pos + 1] == 'X'))) {
    hexfmt = true;
    numString += "0x";
    pos += 2;
  }
  
  // Digit
  // (Hexadecimal strings can contain alphabetic characters)
  if ((pos >= str.size())
      || (!((isdigit(str[pos]))
             || (hexfmt && isalpha(str[pos]))))) {
    return -1;
  }
  
  // Digit Digits
  while ((pos < str.size())
         && ((isdigit(str[pos])
             || (hexfmt && isalpha(str[pos]))))) {
    numString += str[pos];
    ++pos;
  }
  
  number_ = StringConversion::stringToInt(numString);
  
  return pos;
}
  
int NumberDigitSequenceToken::number() const {
//  return dynamic_cast<AlphNumCharactersToken&>(*(subtokens_[0]))
//    .characters();
  return number_;
}


};
