#include "parser/ParseStateData.h"
#include "parser/FunctionArgsToken.h"
#include "parser/NumberLiteralToken.h"
#include "parser/PositionLabelToken.h"
#include "parser/WrongNumArgsException.h"
#include "parser/WrongArgTypeException.h"
#include "nes/UxRomBanking.h"
#include "util/StringConversion.h"
#include "util/ByteConversion.h"
#include <cstring>
#include <fstream>
#include <iostream>

namespace Lonely {


ParseStateData::ParseStateData() { };
  
void ParseStateData::addMacro(const std::string& name,
              const std::string& value) {
  macros_[name] = value;
}
  
bool ParseStateData::macroExists(const std::string& name) const {
  MacroMap::const_iterator it
    = macros_.find(name);
  
  if (it != macros_.end()) {
    return true;
  }
  
  return false;
}

const std::string& ParseStateData::macroValue(const std::string& name) const {
  return macros_.find(name)->second;
}
  
LonelySoundChannel& ParseStateData
  ::channelObj(ParseConstants::Channel channel) {
  switch (channel) {
  case ParseConstants::generic:
    return genericChannel_;
    break;
  case ParseConstants::sq1:
    return sq1Channel_;
    break;
  case ParseConstants::sq2:
    return sq2Channel_;
    break;
  case ParseConstants::tri:
    return triChannel_;
    break;
  default:
    break;
  }
}
  
const LonelySoundChannel& ParseStateData
  ::channelObj(ParseConstants::Channel channel) const {
  switch (channel) {
  case ParseConstants::generic:
    return genericChannel_;
    break;
  case ParseConstants::sq1:
    return sq1Channel_;
    break;
  case ParseConstants::sq2:
    return sq2Channel_;
    break;
  case ParseConstants::tri:
    return triChannel_;
    break;
  default:
    break;
  }
}
  
Taddress ParseStateData
  ::channelCurrentAddress(ParseConstants::Channel channel) const {
  return channelObj(channel).activeFragmentCurrentAddress();
}

void ParseStateData::changeChannelCurrentAddress(
                                     ParseConstants::Channel channel,
                                     Taddress address) {
  channelObj(channel).startNewFragment(address);
}
  
void ParseStateData::appendToChannel(ParseConstants::Channel channel,
                     LaylaSoundToken& token) {
  appendToChannel(channelObj(channel),
                  token);
}

void ParseStateData::appendToChannel(LonelySoundChannel& dst,
                     LaylaSoundToken& token) {
  dst.appendToActiveFragment(token);
}
  
  
void ParseStateData::callSoundCommand(ParseConstants::Channel channel,
                      ParseConstants::SoundCommand soundCommand,
                      FunctionArgsToken& functionArgs,
                      int lineNum) {
  switch (soundCommand) {
  case ParseConstants::soundCommandNone:
    throw GenericParseException("Null sound command called",
                                lineNum);
    break;
  case ParseConstants::delay:
    doDelay(channel, functionArgs, lineNum);
    break;
  case ParseConstants::channelVolume:
    doChannelVolume(channel, functionArgs, lineNum);
    break;
  case ParseConstants::playNoise:
    doPlayNoise(channel, functionArgs, lineNum);
    break;
  case ParseConstants::setLoop:
    doSetLoop(channel, functionArgs, lineNum);
    break;
  case ParseConstants::playLoop:
    doPlayLoop(channel, functionArgs, lineNum);
    break;
  case ParseConstants::baseTempo:
    doBaseTempo(channel, functionArgs, lineNum);
    break;
  case ParseConstants::driverDelay:
    doDriverDelay(channel, functionArgs, lineNum);
    break;
  case ParseConstants::channelKeyShift:
    doChannelKeyShift(channel, functionArgs, lineNum);
    break;
  case ParseConstants::masterKeyShift:
    doMasterKeyShift(channel, functionArgs, lineNum);
    break;
  case ParseConstants::effects:
    doEffects(channel, functionArgs, lineNum);
    break;
  case ParseConstants::callPattern:
    doCallPattern(channel, functionArgs, lineNum);
    break;
  case ParseConstants::returnFromPattern:
    doReturnFromPattern(channel, functionArgs, lineNum);
    break;
  case ParseConstants::channelDecay:
    doChannelDecay(channel, functionArgs, lineNum);
    break;
  case ParseConstants::dummy8d:
    doDummy8d(channel, functionArgs, lineNum);
    break;
  case ParseConstants::driverSlowdown:
    doDriverSlowdown(channel, functionArgs, lineNum);
    break;
  case ParseConstants::op8f:
    doOp8f(channel, functionArgs, lineNum);
    break;
  case ParseConstants::jumpToAddress:
    doJumpToAddress(channel, functionArgs, lineNum);
    break;
  case ParseConstants::setReg1:
    doSetReg1(channel, functionArgs, lineNum);
    break;
  case ParseConstants::setReg2:
    doSetReg2(channel, functionArgs, lineNum);
    break;
  case ParseConstants::setReg3:
    doSetReg3(channel, functionArgs, lineNum);
    break;
  case ParseConstants::lengthEffect:
    doLengthEffect(channel, functionArgs, lineNum);
    break;
  case ParseConstants::dummy95:
    doDummy95(channel, functionArgs, lineNum);
    break;
  default:
    throw GenericParseException("Unhandled sound command type: "
                                  + StringConversion::toString(soundCommand),
                                lineNum);
    break;
  }
}
                        
void ParseStateData::callFunction(ParseConstants::FunctionName functionName,
                  FunctionArgsToken& functionArgs,
                  int lineNum) {
  switch (functionName) {
  case ParseConstants::functionNameNone:
    throw GenericParseException("Null function called",
                                lineNum);
    break;
  case ParseConstants::setByte:
    doSetByte(functionArgs, lineNum);
    break;
  case ParseConstants::setAddress:
    doSetAddress(functionArgs, lineNum);
    break;
  default:
    throw GenericParseException("Unhandled function type: "
                                  + StringConversion::toString(functionName),
                                lineNum);
    break;
  }
}
  
std::string ParseStateData
  ::nameOfCommand(ParseConstants::SoundCommand command) {
  // Placeholder
  return StringConversion::toString(command);
}
  
void ParseStateData
  ::verifyNumArgs(FunctionArgsToken& functionArgs,
                   int requiredArgs,
                   ParseConstants::SoundCommand command,
                   int lineNum) {
  if (functionArgs.subtokens().size() != requiredArgs) {
    throw WrongNumArgsException(nameOfCommand(command),
                                requiredArgs,
                                functionArgs.subtokens().size(),
                                lineNum);
  }
}
  
Taddress ParseStateData
  ::getLabelAddress(PositionLabelToken& positionLabelToken,
                    ParseConstants::SoundCommand command,
                    int lineNum) {
  std::string labelName = positionLabelToken.label();
  
  if (!(positionLabelExists(labelName))) {
    throw GenericParseException("Unknown label address in token type "
                                 + nameOfCommand(command),
                                lineNum);
  }
  
  return UxRomBanking::directToBankedAddressMovable(
              positionLabelValue(labelName));
}
  
void ParseStateData
  ::verifyArgType(FunctionArgsToken& functionArgs,
                   int argNum,
                   ParseTokenTypes::ParseTokenType requiredType,
                   ParseConstants::SoundCommand command,
                   int lineNum) {
  
  if (functionArgs.arg(argNum).type()
        != requiredType) {
    throw WrongArgTypeException(nameOfCommand(command),
                                argNum,
                                requiredType,
                                functionArgs.subtokens()[0]->type(),
                                lineNum);
  }
}
  
void ParseStateData
  ::verifyNumArgs(FunctionArgsToken& functionArgs,
                   int requiredArgs,
                   int lineNum) {
  if (functionArgs.subtokens().size() != requiredArgs) {
    throw WrongNumArgsException("",
                                requiredArgs,
                                functionArgs.subtokens().size(),
                                lineNum);
  }
}
  
Taddress ParseStateData
  ::getLabelAddress(PositionLabelToken& positionLabelToken,
                    int lineNum) {
  std::string labelName = positionLabelToken.label();
  
  if (!(positionLabelExists(labelName))) {
    throw GenericParseException("Unknown label",
                                lineNum);
  }
  
  return UxRomBanking::directToBankedAddressMovable(
              positionLabelValue(labelName));
}
  
void ParseStateData
  ::verifyArgType(FunctionArgsToken& functionArgs,
                   int argNum,
                   ParseTokenTypes::ParseTokenType requiredType,
                   int lineNum) {
  
  if (functionArgs.arg(argNum).type()
        != requiredType) {
    throw WrongArgTypeException("",
                                argNum,
                                requiredType,
                                functionArgs.subtokens()[0]->type(),
                                lineNum);
  }
}
  
void ParseStateData::doDelay(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::delay, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::delay, lineNum);
  
  appendToChannel(channel,
                  *(new SoundDelayToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doChannelVolume(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::channelVolume, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::channelVolume, lineNum);
  
  appendToChannel(channel,
                  *(new SoundChannelVolumeToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doPlayNoise(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 2, ParseConstants::playNoise, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::playNoise, lineNum);
  verifyArgType(functionArgs, 1,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::playNoise, lineNum);
  
  appendToChannel(channel,
                  *(new SoundPlayNoiseToken(
                      dynamic_cast<NumberLiteralToken&>(
                        functionArgs.arg(0)).number(),
                      dynamic_cast<NumberLiteralToken&>(
                        functionArgs.arg(1)).number()
                      )
                    )
                  );
}
  
void ParseStateData::doSetLoop(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 0, ParseConstants::setLoop, lineNum);
  
  appendToChannel(channel,
                  *(new SoundSetLoopToken()));
}
  
void ParseStateData::doPlayLoop(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::playLoop, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::playLoop, lineNum);
  
  appendToChannel(channel,
                  *(new SoundPlayLoopToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doBaseTempo(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::baseTempo, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::baseTempo, lineNum);
  
  appendToChannel(channel,
                  *(new SoundBaseTempoToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doDriverDelay(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::driverDelay, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::driverDelay, lineNum);
  
  appendToChannel(channel,
                  *(new SoundDriverDelayToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doChannelKeyShift(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::channelKeyShift, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::channelKeyShift, lineNum);
  
  appendToChannel(channel,
                  *(new SoundChannelKeyShiftToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doMasterKeyShift(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::masterKeyShift, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::masterKeyShift, lineNum);
  
  appendToChannel(channel,
                  *(new SoundMasterKeyShiftToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doEffects(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::effects, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::effects, lineNum);
  
  appendToChannel(channel,
                  *(new SoundEffectsToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doCallPattern(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::callPattern, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::PositionLabel,
                ParseConstants::callPattern, lineNum);
  
  PositionLabelToken& positionLabelToken
    = dynamic_cast<PositionLabelToken&>(
                      functionArgs.arg(0));
  
  Taddress address = getLabelAddress(positionLabelToken,
                                ParseConstants::callPattern,
                                lineNum);
  
  appendToChannel(channel,
                  *(new SoundCallPatternToken(address)));
}
  
void ParseStateData::doReturnFromPattern(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 0, ParseConstants::returnFromPattern, lineNum);
  
  appendToChannel(channel,
                  *(new SoundReturnFromPatternToken()));
}
  
void ParseStateData::doChannelDecay(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::channelDecay, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::channelDecay, lineNum);
  
  appendToChannel(channel,
                  *(new SoundChannelDecayToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doDummy8d(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 0, ParseConstants::dummy8d, lineNum);
  
  appendToChannel(channel,
                  *(new SoundOp8DToken()));
}
  
void ParseStateData::doDriverSlowdown(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::driverSlowdown, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::driverSlowdown, lineNum);
  
  appendToChannel(channel,
                  *(new SoundDriverSlowdownToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doOp8f(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::op8f, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::op8f, lineNum);
  
  appendToChannel(channel,
                  *(new SoundOp8FToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doJumpToAddress(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::jumpToAddress, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::PositionLabel,
                ParseConstants::jumpToAddress, lineNum);
  
  PositionLabelToken& positionLabelToken
    = dynamic_cast<PositionLabelToken&>(
                      functionArgs.arg(0));
  
  Taddress address = getLabelAddress(positionLabelToken,
                                ParseConstants::jumpToAddress,
                                lineNum);
  
  appendToChannel(channel,
                  *(new SoundJumpToAddressToken(address)));
}
  
void ParseStateData::doSetReg1(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::setReg1, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::setReg1, lineNum);
  
  appendToChannel(channel,
                  *(new SoundSetReg1Token(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doSetReg2(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::setReg2, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::setReg2, lineNum);
  
  appendToChannel(channel,
                  *(new SoundSetReg2Token(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doSetReg3(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::setReg3, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::setReg3, lineNum);
  
  appendToChannel(channel,
                  *(new SoundSetReg3Token(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doLengthEffect(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 1, ParseConstants::lengthEffect, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                ParseConstants::lengthEffect, lineNum);
  
  appendToChannel(channel,
                  *(new SoundLengthEffectToken(
                    dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number())));
}
  
void ParseStateData::doDummy95(ParseConstants::Channel channel,
             FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 0, ParseConstants::dummy95, lineNum);
  
  appendToChannel(channel,
                  *(new SoundOp95Token()));
}
  
void ParseStateData::doSetByte(FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 2, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                lineNum);
  verifyArgType(functionArgs, 1,
                ParseTokenTypes::NumberLiteral,
                lineNum);
  
  Taddress byteAddress = dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number();
  Tbyte byteVal = dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(1)).number();
  
  bytePatches_[byteAddress] = byteVal;
}

void ParseStateData::doSetAddress(FunctionArgsToken& functionArgs,
             int lineNum) {
  verifyNumArgs(functionArgs, 2, lineNum);
  verifyArgType(functionArgs, 0,
                ParseTokenTypes::NumberLiteral,
                lineNum);
        
  Taddress addressLocation = dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(0)).number();
  Taddress addressValue;
      
  if ((functionArgs.arg(1).type() == ParseTokenTypes::NumberLiteral)) {
     addressValue = dynamic_cast<NumberLiteralToken&>(
                      functionArgs.arg(1)).number();
  }
  else if (functionArgs.arg(1).type() == ParseTokenTypes::PositionLabel) {
     addressValue = getLabelAddress(
          dynamic_cast<PositionLabelToken&>(
                      functionArgs.arg(1)),
          lineNum);
  }
  else {
    throw GenericParseException("Invalid argument 1 to SETADDRESS "
                                "(expected literal or position label)",
                                lineNum);
  }
  
  addressPatches_[addressLocation] = addressValue;
}
  
void ParseStateData::addPositionLabel(const std::string& name,
                      Taddress value) {
  positionLabels_[name] = value;
}
  
void ParseStateData::addPositionLabelAtChannelPos(
                                  ParseConstants::Channel channel,
                                  const std::string& name,
                                  int offset) {
  addPositionLabel(name,
                   channelCurrentAddress(channel) + offset);
}

bool ParseStateData::positionLabelExists(const std::string& name) const {
  PositionLabelMap::const_iterator it
    = positionLabels_.find(name);
  
  if (it != positionLabels_.end()) {
    return true;
  }
  
  return false;
}

Taddress ParseStateData::positionLabelValue(const std::string& name) const {
  return positionLabels_.find(name)->second;
}
  
//ParseConstants::Channel ParseStateData
//  ::channelFromName(const std::string& channelName) {
//}

void ParseStateData::outputToFiles(const std::string& filepath) const {
  outputToFile(genericChannel_, filepath + "generic-");
  outputToFile(sq1Channel_, filepath + "sq1-");
  outputToFile(sq2Channel_, filepath + "sq2-");
  outputToFile(triChannel_, filepath + "tri-");
}
                       
void ParseStateData::outputToFile(const LonelySoundChannel& dst,
                  const std::string& basename) const {
  for (int i = 0; i < dst.numFragments(); i++) {
    if (!(dst.fragmentHasData(i))) {
      continue;
    }
  
    std::string filename
      = basename + StringConversion::toString(
                      dst.fragmentAddress(i))
                 + ".bin";
    
//    std::cout << filename << std::endl;
    
    std::string data;
    dst.writeFragmentToString(i, data);
    
    std::ofstream ofs(filename, std::ios_base::binary
                                  | std::ios_base::trunc);
    ofs.write(data.c_str(), data.size());
  }
}

void ParseStateData::outputToRom(NesRom& rom) const {
  outputToRom(genericChannel_, rom);
  outputToRom(sq1Channel_, rom);
  outputToRom(sq2Channel_, rom);
  outputToRom(triChannel_, rom);
}

void ParseStateData::outputToRom(const LonelySoundChannel& dst,
                 NesRom& rom) const {
  for (int i = 0; i < dst.numFragments(); i++) {
    if (!(dst.fragmentHasData(i))) {
      continue;
    }
    
    std::string data;
    dst.writeFragmentToString(i, data);
    
    Taddress address = dst.fragmentAddress(i);
    
//    std::cout << StringConversion::intToString(
//                    address,
//                    StringConversion::baseHex) << std::endl;
    
    std::memcpy(rom.directWrite(address),
                data.c_str(),
                data.size());
  }
  
  // Write byte and address patches
  for (BytePatchesMap::const_iterator it = bytePatches_.cbegin();
       it != bytePatches_.cend();
       ++it) {
    *(rom.directWrite(it->first)) = it->second;
  }
  
  for (AddressPatchesMap::const_iterator it = addressPatches_.cbegin();
       it != addressPatches_.cend();
       ++it) {
    ByteConversion::toBytes(it->second,
                            rom.directWrite(it->first),
                            ByteSizes::uint16Size,
                            EndiannessTypes::little,
                            SignednessTypes::nosign);
  }
}


};
