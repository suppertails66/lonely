#include "parser/ControlTriggerToken.h"
#include "parser/WrongTokenException.h"
#include "parser/AlphNumCharactersToken.h"
#include "parser/FunctionArgsToken.h"
#include "parser/ParseConstants.h"
#include "util/StringConversion.h"
#include <cctype>

namespace Lonely {


ControlTriggerToken::ControlTriggerToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType ControlTriggerToken::type() const {
  return ParseTokenTypes::ControlTrigger;
}

int ControlTriggerToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // PIPE
  if (charAtPosIs(str, pos, '|')) {
    ++pos;
    
    // ...
    
    return pos;
  }
  
  // LESS_THAN NumberLiteral
  if (charAtPosIs(str, pos, '<')) {
    ++pos;
    
    advance(str, pos, lineNum);
    token = readTokenOrFail(ParseTokenTypes::NumberLiteral,
                            str,
                            pos,
                            lineNum,
                            parseStateData);
    
    if (token == NULL) {
      return -1;
    }
    
    // ...
    
    return pos;
  }

  // BuiltInFunctionName OPEN_PAREN CLOSE_PAREN
  // BuiltInFunctionName OPEN_PAREN FunctionArgs CLOSE_PAREN
  token = readTokenOrFail(ParseTokenTypes::AlphNumCharacters,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    return -1;
  }
  
  advance(str, pos, lineNum);
  if (!(charAtPosIs(str, pos, '('))) {
    return -1;
  }
  else {
    ++pos;
  }
  
  advance(str, pos, lineNum);
  token = readTokenOrFail(ParseTokenTypes::FunctionArgs,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    throw GenericParseException("Could not read function arguments",
                                lineNum);
  }
  
/*  while (token != NULL) {
    advance(str, pos, lineNum);
    token = readTokenOrFail(ParseTokenTypes::FunctionArgs,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  } */
  
  advance(str, pos, lineNum);
  if (!(charAtPosIs(str, pos, ')'))) {
    return -1;
  }
  else {
    ++pos;
  }
  
  return pos;
}

void ControlTriggerToken::doSoundDataAssignment(ParseStateData& parseStateData,
               int lineNum,
               ParseConstants::Channel channel) {
  // check for short forms of loop tokens?
  // ...
  
  AlphNumCharactersToken& varName
    = dynamic_cast<AlphNumCharactersToken&>(*(subtokens_[0]));
  
  FunctionArgsToken& args
    = dynamic_cast<FunctionArgsToken&>(*(subtokens_[1]));
  
  ParseConstants::SoundCommand command
    = ParseConstants::commandByName(varName.characters());
  
  if (command == ParseConstants::soundCommandNone) {
    throw GenericParseException("Unrecognized sound command: "
                                  + varName.characters(),
                                lineNum);
  }
  
  parseStateData.callSoundCommand(channel,
                                  command,
                                  args,
                                  lineNum);
}
  
const std::string& ControlTriggerToken::name() const {
  return (dynamic_cast<const AlphNumCharactersToken&>(*(subtokens_[0])))
              .characters();
}
  
FunctionArgsToken& ControlTriggerToken::args() {
  return (dynamic_cast<FunctionArgsToken&>(*(subtokens_[1])));
}


};
