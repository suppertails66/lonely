#include "parser/StatementToken.h"
#include "parser/WrongTokenException.h"
#include "parser/ControlTriggerToken.h"
#include "parser/ParseConstants.h"

namespace Lonely {


StatementToken::StatementToken()
  : ParseToken() { };
  

ParseTokenTypes::ParseTokenType StatementToken::type() const {
  return ParseTokenTypes::Statement;
}

int StatementToken::fromStringStep(std::string& str,
                           int pos,
                           int& lineNum,
                           ParseStateData& parseStateData) {
  ParseToken* token = NULL;

  // ControlTrigger
  token = readTokenOrFail(ParseTokenTypes::ControlTrigger,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token != NULL) {
    callFunction(parseStateData,
                 dynamic_cast<ControlTriggerToken&>(*(token)),
                 lineNum);
    
    return pos;
  }

  // Assignment
  token = readTokenOrFail(ParseTokenTypes::Assignment,
                          str,
                          pos,
                          lineNum,
                          parseStateData);
  
  if (token == NULL) {
    throw WrongTokenException(ParseTokenTypes::Assignment,
                              type(),
                              lineNum);
  }
  
  return pos;
}
  
void StatementToken::callFunction(
            ParseStateData& parseStateData,
            ControlTriggerToken& controlTriggerToken,
            int lineNum) {
  ParseConstants::FunctionName functionName
    = ParseConstants::functionByName(controlTriggerToken.name());
  
  if (functionName == ParseConstants::functionNameNone) {
    throw GenericParseException("Called unknown function: "
                                  + controlTriggerToken.name(),
                                lineNum);
  }
  
  parseStateData.callFunction(functionName,
                              controlTriggerToken.args(),
                              lineNum);
}


};
