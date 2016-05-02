#include "parser/ParseToken.h"
#include "parser/ParseTokenCollection.h"
#include "parser/SoundProgramToken.h"
#include "parser/ExpressionsToken.h"
#include "parser/ExpressionToken.h"
#include "parser/StatementToken.h"
#include "parser/AssignmentToken.h"
#include "parser/MacroNameToken.h"
#include "parser/BuiltInVariableNameToken.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/StringLiteralToken.h"
#include "parser/NumberLiteralToken.h"
#include "parser/NumberDigitSequenceToken.h"
#include "parser/SoundDataToken.h"
#include "parser/SoundCommandsToken.h"
#include "parser/SoundCommandToken.h"
#include "parser/NoteToken.h"
#include "parser/NoteIdentifierToken.h"
#include "parser/NoteNameToken.h"
#include "parser/NoteLetterToken.h"
#include "parser/ControlTriggerToken.h"
#include "parser/FunctionArgsToken.h"
#include "parser/FunctionArgToken.h"
#include "parser/PositionLabelToken.h"
#include <cstdlib>
#include <cctype>
#include <iostream>

namespace Lonely {


ParseToken::ParseToken() { };
  
ParseToken::~ParseToken() {
  for (int i = 0; i < subtokens_.size(); i++) {
    delete subtokens_[i];
  }
  subtokens_.clear();
}
  
ParseTokenCollection& ParseToken::subtokens() {
  return subtokens_;
}

const ParseTokenCollection& ParseToken::subtokens() const {
  return subtokens_;
}
  
bool ParseToken::fromString(std::string& str,
                int& pos,
                int& lineNum,
                ParseStateData& parseStateData) {
  for (int i = 0; i < subtokens_.size(); i++) {
    delete subtokens_[i];
  }
  subtokens_.clear();
  
  int localLineNum = lineNum;
  
  // Eat irrelevant input
  int nextTokenPos = advance(str, pos, localLineNum);
  
  // Conversion failed if we hit end of input
  if (nextTokenPos >= str.size()) {
    pos = nextTokenPos;
    return false;
  }
                
  // Check if we need to perform macro substitution.
  // Don't do this if this call is already reading a macro name,
  // since that would lead to an infinite recursion.
//  std::cout << lineNum << str.substr(pos, str.size() - pos) << std::endl;
  if (type() != ParseTokenTypes::MacroName
      && (charAtPosIs(str, pos, '$'))) {
    int macroCheckPos = pos;
    int macroCheckLineNum = lineNum;
    MacroNameToken macroName;
    macroName.fromString(str,
                         macroCheckPos,
                         macroCheckLineNum,
                         parseStateData);
    
//    std::cout << macroName.macroName() << std::endl;

    // If macro exists, substitute
    if (parseStateData.macroExists(macroName.macroName())) {
      const std::string& macroValue
        = parseStateData.macroValue(macroName.macroName());
      std::string remainder = str.substr(macroCheckPos,
                                         str.size() - macroCheckPos);
      str = str.substr(0, pos);
      str += macroValue;
      str += remainder;
      
//      std::cout << macroValue << std::endl;
//      std::cout << str << std::endl;
      
      // Subtract off newlines in the macro from the line count.
      // Problems in multiline macros will have misleading (too low)
      // line numbers reported, but this will at least keep the rest
      // of the file consistent
      for (int i = 0; i < macroValue.size(); i++) {
        if (str[pos + i] == '\n') {
          --localLineNum;
        }
      }
      
      pos = macroCheckPos;
      
      advance(str, pos, macroCheckLineNum);
    }
  }
  
  // Try to let the derived class do the conversion
  int resultPos = fromStringStep(str, nextTokenPos, localLineNum, parseStateData);
  
  // The conversion failed if result is -1)
  if ((resultPos < 0)) {
    return false;
  }
  else {
    pos = resultPos;
    lineNum = localLineNum;
    return true;
  }
}

int ParseToken::advance(const std::string& str, int& pos, int& lineNum) {
//  std::cout << "ADVANCE" << std::endl;
//  std::cout << pos << " " << str.size() << std::endl;
  while (pos < str.size()) {
    // // Comment
    if ((pos <= str.size() - 2)
        && (str[pos] == '/')
        && (str[pos + 1] == '/')) {
      pos += 2;
      while ((pos < str.size())
             && (str[pos] != '\n')) {
        ++pos;
      }
      
      // Skip the newline character itself
      if (pos < str.size()) {
        ++pos;
        ++lineNum;
      }
    }
    // /* Comment
    else if ((pos <= str.size() - 2)
        && (str[pos] == '/')
        && (str[pos + 1] == '*')) {
      pos += 2;
      while (pos < str.size() - 1) {
        if ((str[pos] == '*')
             && (str[pos + 1] == '/')) {
          pos += 2;
          break;
        }
        
        if (str[pos] == '\n') {
          ++lineNum;
        }
        
        ++pos;
      }
    }
    // Whitespace
    else if (isspace(str[pos])) {
      if (str[pos] == '\n') {
        ++lineNum;
      }
    
      ++pos;
    }
    // Start of next token
    else {
      break;
    }
  }
  
  return pos;
}
  
bool ParseToken::charAtPosIs(const std::string& str,
                        int pos,
                        char checkValue) {
  if (pos >= str.size()) {
    return false;
  }
  
  if (str[pos] == checkValue) {
    return true;
  }
  else {
    return false;
  }
}

ParseToken* ParseToken::readTokenOrFail(ParseTokenTypes::ParseTokenType
                                        tokenType,
                                     std::string& str,
                                     int& pos,
                                     int& lineNum,
                                     ParseStateData& parseStateData) {
//  std::cout << tokenType << std::endl;
  ParseToken* newToken = NULL;
  
  switch (tokenType) {
  case ParseTokenTypes::SoundProgram:
    newToken = new SoundProgramToken();
    break;
  case ParseTokenTypes::Expressions:
    newToken = new ExpressionsToken();
    break;
  case ParseTokenTypes::Expression:
    newToken = new ExpressionToken();
    break;
  case ParseTokenTypes::Statement:
    newToken = new StatementToken();
    break;
  case ParseTokenTypes::Assignment:
    newToken = new AssignmentToken();
    break;
  case ParseTokenTypes::MacroName:
    newToken = new MacroNameToken();
    break;
  case ParseTokenTypes::AlphNumCharacters:
    newToken = new AlphNumCharactersToken();
    break;
  case ParseTokenTypes::StringLiteral:
    newToken = new StringLiteralToken();
    break;
  case ParseTokenTypes::BuiltInVariableName:
    newToken = new BuiltInVariableNameToken();
    break;
  case ParseTokenTypes::NumberLiteral:
    newToken = new NumberLiteralToken();
    break;
  case ParseTokenTypes::NumberDigitSequence:
    newToken = new NumberDigitSequenceToken();
    break;
  case ParseTokenTypes::SoundData:
    newToken = new SoundDataToken();
    break;
  case ParseTokenTypes::SoundCommands:
    newToken = new SoundCommandsToken();
    break;
  case ParseTokenTypes::SoundCommand:
    newToken = new SoundCommandToken();
    break;
  case ParseTokenTypes::Note:
    newToken = new NoteToken();
    break;
  case ParseTokenTypes::NoteIdentifier:
    newToken = new NoteIdentifierToken();
    break;
  case ParseTokenTypes::NoteName:
    newToken = new NoteNameToken();
    break;
  case ParseTokenTypes::NoteLetter:
    newToken = new NoteLetterToken();
    break;
  case ParseTokenTypes::ControlTrigger:
    newToken = new ControlTriggerToken();
    break;
  case ParseTokenTypes::FunctionArgs:
    newToken = new FunctionArgsToken();
    break;
  case ParseTokenTypes::FunctionArg:
    newToken = new FunctionArgToken();
    break;
  case ParseTokenTypes::PositionLabel:
    newToken = new PositionLabelToken();
    break;
  default:
    // Unknown token type
    return NULL;
    break;
  }
  
  bool result = newToken->fromString(str, pos, lineNum, parseStateData);
  
  // If conversion succeeded, return the new token
  if (result) {
    subtokens_.push_back(newToken);
    return newToken;
  }
  // If conversion failed, deallocate the token and return NULL
  else {
    delete newToken;
    return NULL;
  }
}


};
