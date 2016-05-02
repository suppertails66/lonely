#include "parser/ParseToken.h"

namespace Lonely {


std::string ParseTokenTypes::nameOfType(
    ParseTokenTypes::ParseTokenType type) {
  switch (type) {
  case none:
    return "None";
    break;
  case SoundProgram:
    return "SoundProgram";
    break;
  case Expressions:
    return "Expressions";
    break;
  case Expression:
    return "Expression";
    break;
  case Statement:
    return "Statement";
    break;
  case Assignment:
    return "Assignment";
    break;
  case MacroName:
    return "MacroName";
    break;
  case BuiltInVariableName:
    return "BuiltInVariableName";
    break;
  case SoundData:
    return "SoundData";
    break;
  case SoundCommands:
    return "SoundCommands";
    break;
  case SoundCommand:
    return "SoundCommand";
    break;
  case Note:
    return "Note";
    break;
  case NoteIdentifier:
    return "NoteIdentifier";
    break;
  case NoteName:
    return "NoteName";
    break;
  case NoteLetter:
    return "NoteLetter";
    break;
  case ControlTrigger:
    return "ControlTrigger";
    break;
  case FunctionArgs:
    return "FunctionArgs";
    break;
  case FunctionArg:
    return "FunctionArg";
    break;
  case PositionLabel:
    return "PositionLabel";
    break;
  case StringLiteral:
    return "StringLiteral";
    break;
//    Characters:
  case AlphNumCharacters:
    return "AlphNumCharacters";
    break;
  case AlphNumCharacter:
    return "AlphNumCharacter";
    break;
  case NumberLiteral:
    return "Number";
    break;
  case NumberDigitSequence:
    return "NumberDigitSequence";
    break;
  case Digits:
    return "Digits";
    break;
  default:
    return "***UNKNOWN***";
    break;
  }
}


};
