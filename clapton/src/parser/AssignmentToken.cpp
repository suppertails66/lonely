#include "parser/AssignmentToken.h"
#include "parser/WrongTokenException.h"
#include "parser/BuiltInVariableNameToken.h"
#include "parser/MacroNameToken.h"
#include "parser/StringLiteralToken.h"
#include "parser/NumberLiteralToken.h"
#include <iostream>

namespace Lonely {


AssignmentToken::AssignmentToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType AssignmentToken::type() const {
  return ParseTokenTypes::Assignment;
}

int AssignmentToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // MacroName EQUALS QUOTE Characters QUOTE
  token = readTokenOrFail(ParseTokenTypes::MacroName,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  // Macro assignment
  if (token != NULL) {
//    std::cout << "STARTING" << std::endl;
//    std::cout << str.substr(pos, str.size() - pos) << std::endl;
//    std::cout << pos << " " << str.size() << std::endl;
    
//    std::cout << str.substr(pos, str.size() - pos) << std::endl;
    MacroNameToken& macroNameToken
      = dynamic_cast<MacroNameToken&>(*token);
  
    advance(str, pos, lineNum);
    if (charAtPosIs(str, pos, '=')) {
      ++pos;
    }
    else {
      return -1;
    }
    
    advance(str, pos, lineNum);
    
    token = readTokenOrFail(ParseTokenTypes::StringLiteral,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
    
    if (token == NULL) {
      throw GenericParseException("Couldn't read value assigned to macro",
                                  lineNum);
    }
    
    StringLiteralToken& stringLiteralToken
      = dynamic_cast<StringLiteralToken&>(*token);
    
    // Add macro mapping to state data
    parseStateData.addMacro(macroNameToken.macroName(),
                            stringLiteralToken.characters());
    
    return pos;
  }
  
  token = readTokenOrFail(ParseTokenTypes::BuiltInVariableName,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token != NULL) {
    BuiltInVariableNameToken& variableNameToken
      = dynamic_cast<BuiltInVariableNameToken&>(*token);
      
//    std::cout << variableNameToken.variableName() << std::endl;
  
    advance(str, pos, lineNum);
    if (charAtPosIs(str, pos, '=')) {
      ++pos;
    }
    else {
      return -1;
    }
    
    advance(str, pos, lineNum);
    
    // BuiltInVariableName EQUALS Number
    token = readTokenOrFail(ParseTokenTypes::NumberLiteral,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
    
    if (token != NULL) {
      doNumberLiteralAssignment(parseStateData,
                                lineNum,
                                variableNameToken.variableName(),
                                dynamic_cast<NumberLiteralToken&>(*token));
      return pos;
    }
    
    // BuiltInVariableName EQUALS SoundData
    token = readTokenOrFail(ParseTokenTypes::SoundData,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
    
    if (token != NULL) {
      doSoundDataAssignment(parseStateData,
                                       lineNum,
                                       variableNameToken.variableName(),
                                       dynamic_cast<SoundDataToken&>(*token));
      
      return pos;
    }
  }
  
  throw GenericParseException("Unable to resolve statement to assignment",
                              lineNum);
  
  return -1;
}

void AssignmentToken::doNumberLiteralAssignment(
                        ParseStateData& parseStateData,
                        int lineNum,
                        const std::string& name,
                        NumberLiteralToken& numberLiteralToken) {
  ParseConstants::Channel channel
    = ParseConstants::channelAddressByName(name);
  
  if (channel == ParseConstants::channelNone) {
    throw GenericParseException("Assigned to unrecognized channel address: "
                                  + name,
                                lineNum);
  }
  
  parseStateData.changeChannelCurrentAddress(channel,
                                             numberLiteralToken.number());
}

void AssignmentToken::doSoundDataAssignment(
                           ParseStateData& parseStateData,
                           int lineNum,
                           const std::string& name,
                           SoundDataToken& soundDataToken) {
  ParseConstants::Channel channel
    = ParseConstants::channelByName(name);
  
  if (channel == ParseConstants::channelNone) {
    throw GenericParseException("Assigned to unrecognized channel: "
                                  + name,
                                lineNum);
  }
  
  soundDataToken.doSoundDataAssignment(parseStateData,
                                       lineNum,
                                       channel);
}


};
