#ifndef PARSESTATEDATA_H
#define PARSESTATEDATA_H


#include "parser/MacroMap.h"
#include "parser/ParseConstants.h"
#include "parser/ParseTokenType.h"
#include "gamedata/LonelySoundChannel.h"
#include "nes/NesRom.h"
#include "structs/Taddress.h"
#include <string>
#include <map>

namespace Lonely {


class FunctionArgsToken;
class PositionLabelToken;

class ParseStateData {
public:
  ParseStateData();
  
  void addMacro(const std::string& name,
                const std::string& value);
  
  bool macroExists(const std::string& name) const;
  
  const std::string& macroValue(const std::string& name) const;
  
  void addPositionLabel(const std::string& name,
                        Taddress value);
  
  void addPositionLabelAtChannelPos(ParseConstants::Channel channel,
                                    const std::string& name);
  
  bool positionLabelExists(const std::string& name) const;
  
  Taddress positionLabelValue(const std::string& name) const;
  
  Taddress channelCurrentAddress(ParseConstants::Channel channel) const;
  
  void changeChannelCurrentAddress(ParseConstants::Channel channel,
                                       Taddress address);
  
  void appendToChannel(ParseConstants::Channel channel,
                       LaylaSoundToken& token);
  
  void callSoundCommand(ParseConstants::Channel channel,
                        ParseConstants::SoundCommand soundCommand,
                        FunctionArgsToken& functionArgs,
                        int lineNum);
                        
  void callFunction(ParseConstants::FunctionName functionName,
                    FunctionArgsToken& functionArgs,
                    int lineNum);
  
//  ParseConstants::Channel channelFromName(const std::string& channelName);

  void outputToFiles(const std::string& filepath) const;

  void outputToRom(NesRom& rom) const;
protected:
  typedef std::map<std::string, Taddress> PositionLabelMap;
  typedef std::map<Taddress, Tbyte> BytePatchesMap;
  typedef std::map<Taddress, Taddress> AddressPatchesMap;
  
  LonelySoundChannel& channelObj(ParseConstants::Channel channel);
  const LonelySoundChannel& channelObj(ParseConstants::Channel channel) const;

  void appendToChannel(LonelySoundChannel& dst,
                       LaylaSoundToken& token);
                       
  void outputToFile(const LonelySoundChannel& dst,
                    const std::string& basename) const;

  void outputToRom(const LonelySoundChannel& dst,
                   NesRom& rom) const;

  MacroMap macros_;
  PositionLabelMap positionLabels_;
  BytePatchesMap bytePatches_;
  AddressPatchesMap addressPatches_;
  
  LonelySoundChannel genericChannel_;
  LonelySoundChannel sq1Channel_;
  LonelySoundChannel sq2Channel_;
  LonelySoundChannel triChannel_;
  
  std::string nameOfCommand(ParseConstants::SoundCommand command);
  
  void verifyArgType(FunctionArgsToken& functionArgs,
                     int argNum,
                     ParseTokenTypes::ParseTokenType requiredType,
                     ParseConstants::SoundCommand command,
                     int lineNum);
  
  void verifyNumArgs(FunctionArgsToken& functionArgs,
                     int requiredArgs,
                     ParseConstants::SoundCommand command,
                     int lineNum);
  
  Taddress getLabelAddress(PositionLabelToken& positionLabelToken,
                      ParseConstants::SoundCommand command,
                      int lineNum);
  
  void verifyArgType(FunctionArgsToken& functionArgs,
                     int argNum,
                     ParseTokenTypes::ParseTokenType requiredType,
                     int lineNum);
  
  void verifyNumArgs(FunctionArgsToken& functionArgs,
                     int requiredArgs,
                     int lineNum);
  
  Taddress getLabelAddress(PositionLabelToken& positionLabelToken,
                      int lineNum);
  
  void doDelay(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doChannelVolume(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doPlayNoise(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doSetLoop(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doPlayLoop(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doBaseTempo(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doDriverDelay(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doChannelKeyShift(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doMasterKeyShift(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doEffects(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doCallPattern(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doReturnFromPattern(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doChannelDecay(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doDummy8d(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doDriverSlowdown(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doOp8f(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doJumpToAddress(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doSetReg1(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doSetReg2(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doSetReg3(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doLengthEffect(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doDummy95(ParseConstants::Channel channel,
               FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doSetByte(FunctionArgsToken& functionArgs,
               int lineNum);
  
  void doSetAddress(FunctionArgsToken& functionArgs,
               int lineNum);
};


};


#endif
