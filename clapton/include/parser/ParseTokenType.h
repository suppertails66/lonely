#ifndef PARSETOKENTYPE_H
#define PARSETOKENTYPE_H


#include <string>

namespace Lonely {


class ParseTokenTypes {
public:
  enum ParseTokenType {
    none = 0,
    SoundProgram,
    Expressions,
    Expression,
    Statement,
    Assignment,
    MacroName,
    BuiltInVariableName,
    SoundData,
    SoundCommands,
    SoundCommand,
    Note,
    NoteIdentifier,
    NoteName,
    NoteLetter,
    ControlTrigger,
    FunctionArgs,
    FunctionArg,
    PositionLabel,
    StringLiteral,
//    Characters,
    AlphNumCharacters,
    AlphNumCharacter,
    NumberLiteral,
    NumberDigitSequence,
    Digits
  };
  
  static std::string nameOfType(ParseTokenTypes::ParseTokenType type);
};


};


#endif
