#ifndef PARSECONSTANTS_H
#define PARSECONSTANTS_H


#include <string>
#include "gamedata/LaylaSoundToken.h"

namespace Lonely {


class ParseConstants {
public:
  enum Channel {
    channelNone = 0,
    generic,
    sq1,
    sq2,
    tri
  };
  
  enum SoundCommand {
    soundCommandNone = 0,
    delay,
    channelVolume,
    playNoise,
    setLoop,
    playLoop,
    baseTempo,
    driverDelay,
    channelKeyShift,
    masterKeyShift,
    effects,
    callPattern,
    returnFromPattern,
    channelDecay,
    dummy8d,
    driverSlowdown,
    op8f,
    jumpToAddress,
    setReg1,
    setReg2,
    setReg3,
    lengthEffect,
    dummy95
  };
  
  enum FunctionName {
    functionNameNone = 0,
    setByte,
    setAddress
  };

  //***********************************
  // Built-in variable names
  //***********************************
  // Channel names
  const static std::string genericChannelName;
  const static std::string sq1ChannelName;
  const static std::string sq2ChannelName;
  const static std::string triChannelName;
  // Channel base address names
  const static std::string genericChannelAddressName;
  const static std::string sq1ChannelAddressName;
  const static std::string sq2ChannelAddressName;
  const static std::string triChannelAddressName;
  
  //***********************************
  // Command names
  //***********************************
//  const static std::string noteCommandName;
  const static std::string delayCommandName;
  const static std::string channelVolumeCommandName;
  const static std::string playNoiseCommandName;
  const static std::string setLoopCommandName;
  const static std::string playLoopCommandName;
  const static std::string baseTempoCommandName;
  const static std::string driverDelayCommandName;
  const static std::string channelKeyShiftCommandName;
  const static std::string masterKeyShiftCommandName;
  const static std::string effectsCommandName;
  const static std::string callPatternCommandName;
  const static std::string returnFromPatternCommandName;
  const static std::string channelDecayCommandName;
  const static std::string dummy8dCommandName;
  const static std::string driverSlowdownCommandName;
  const static std::string op8fCommandName;
  const static std::string jumpToAddressCommandName;
  const static std::string setReg1CommandName;
  const static std::string setReg2CommandName;
  const static std::string setReg3CommandName;
  const static std::string lengthEffectCommandName;
  const static std::string dummy95CommandName;
  
  //***********************************
  // Built-in function names
  //***********************************
  const static std::string setByteFunctionName;
  const static std::string setAddressFunctionName;
  
  static Channel channelByName(const std::string& name);
  
  static Channel channelAddressByName(const std::string& name);
  
  static SoundCommand
    commandByName(const std::string& name);
  
  static FunctionName
    functionByName(const std::string& name);
protected:
  
};


};


#endif
