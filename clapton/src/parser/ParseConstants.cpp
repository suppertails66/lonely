#include "parser/ParseConstants.h"

namespace Lonely {


const std::string ParseConstants::genericChannelName
    ("GENERIC");
const std::string ParseConstants::sq1ChannelName
    ("SQ1");
const std::string ParseConstants::sq2ChannelName
    ("SQ2");
const std::string ParseConstants::triChannelName
    ("TRI");
  
const std::string ParseConstants::genericChannelAddressName
    ("GENERICADDR");
const std::string ParseConstants::sq1ChannelAddressName
    ("SQ1ADDR");
const std::string ParseConstants::sq2ChannelAddressName
    ("SQ2ADDR");
const std::string ParseConstants::triChannelAddressName
    ("TRIADDR");

//const std::string ParseConstants::noteCommandName
//    ("NOTE");
const std::string ParseConstants::delayCommandName
    ("DELAY");
const std::string ParseConstants::channelVolumeCommandName
    ("CHANVOL");
const std::string ParseConstants::playNoiseCommandName
    ("NOISE");
const std::string ParseConstants::setLoopCommandName
    ("SETLOOP");
const std::string ParseConstants::playLoopCommandName
    ("REPLAYLOOP");
const std::string ParseConstants::baseTempoCommandName
    ("TEMPO");
const std::string ParseConstants::driverDelayCommandName
    ("DRIVERDELAY");
const std::string ParseConstants::channelKeyShiftCommandName
    ("CHANSHIFT");
const std::string ParseConstants::masterKeyShiftCommandName
    ("MASTSHIFT");
const std::string ParseConstants::effectsCommandName
    ("EFFECTS");
const std::string ParseConstants::callPatternCommandName
    ("CALL");
const std::string ParseConstants::returnFromPatternCommandName
    ("RET");
const std::string ParseConstants::channelDecayCommandName
    ("DECAY");
const std::string ParseConstants::dummy8dCommandName
    ("DUMMY8D");
const std::string ParseConstants::driverSlowdownCommandName
    ("DRIVERSLOWDOWN");
const std::string ParseConstants::op8fCommandName
    ("OP8F");
const std::string ParseConstants::jumpToAddressCommandName
    ("JUMP");
const std::string ParseConstants::setReg1CommandName
    ("REG1");
const std::string ParseConstants::setReg2CommandName
    ("REG2");
const std::string ParseConstants::setReg3CommandName
    ("REG3");
const std::string ParseConstants::lengthEffectCommandName
    ("LENGTHEFFECT");
const std::string ParseConstants::dummy95CommandName
    ("DUMMY95");

const std::string ParseConstants::setByteFunctionName
    ("SETBYTE");
const std::string ParseConstants::setAddressFunctionName
    ("SETADDRESS");
  
ParseConstants::Channel ParseConstants
  ::channelByName(const std::string& name) {
  if (name.compare(genericChannelName) == 0) {
    return generic;
  }
  else if (name.compare(sq1ChannelName) == 0) {
    return sq1;
  }
  else if (name.compare(sq2ChannelName) == 0) {
    return sq2;
  }
  else if (name.compare(triChannelName) == 0) {
    return tri;
  }
  else {
    return channelNone;
  }
}
  
ParseConstants::Channel ParseConstants
  ::channelAddressByName(const std::string& name) {
  if (name.compare(genericChannelAddressName) == 0) {
    return generic;
  }
  else if (name.compare(sq1ChannelAddressName) == 0) {
    return sq1;
  }
  else if (name.compare(sq2ChannelAddressName) == 0) {
    return sq2;
  }
  else if (name.compare(triChannelAddressName) == 0) {
    return tri;
  }
  else {
    return channelNone;
  }
}
  
ParseConstants::SoundCommand
    ParseConstants::commandByName(const std::string& name) {
  if (name.compare(delayCommandName) == 0) {
    return delay;
  }
  else if (name.compare(channelVolumeCommandName) == 0) {
    return channelVolume;
  }
  else if (name.compare(playNoiseCommandName) == 0) {
    return playNoise;
  }
  else if (name.compare(setLoopCommandName) == 0) {
    return setLoop;
  }
  else if (name.compare(playLoopCommandName) == 0) {
    return playLoop;
  }
  else if (name.compare(baseTempoCommandName) == 0) {
    return baseTempo;
  }
  else if (name.compare(driverDelayCommandName) == 0) {
    return driverDelay;
  }
  else if (name.compare(channelKeyShiftCommandName) == 0) {
    return channelKeyShift;
  }
  else if (name.compare(masterKeyShiftCommandName) == 0) {
    return masterKeyShift;
  }
  else if (name.compare(effectsCommandName) == 0) {
    return effects;
  }
  else if (name.compare(callPatternCommandName) == 0) {
    return callPattern;
  }
  else if (name.compare(returnFromPatternCommandName) == 0) {
    return returnFromPattern;
  }
  else if (name.compare(channelDecayCommandName) == 0) {
    return channelDecay;
  }
  else if (name.compare(dummy8dCommandName) == 0) {
    return dummy8d;
  }
  else if (name.compare(driverSlowdownCommandName) == 0) {
    return driverSlowdown;
  }
  else if (name.compare(op8fCommandName) == 0) {
    return op8f;
  }
  else if (name.compare(jumpToAddressCommandName) == 0) {
    return jumpToAddress;
  }
  else if (name.compare(setReg1CommandName) == 0) {
    return setReg1;
  }
  else if (name.compare(setReg2CommandName) == 0) {
    return setReg2;
  }
  else if (name.compare(setReg3CommandName) == 0) {
    return setReg3;
  }
  else if (name.compare(lengthEffectCommandName) == 0) {
    return lengthEffect;
  }
  else if (name.compare(dummy95CommandName) == 0) {
    return dummy95;
  }
  else {
    return soundCommandNone;
  }
}
  
ParseConstants::FunctionName
    ParseConstants::functionByName(const std::string& name) {
  if (name.compare(setByteFunctionName) == 0) {
    return setByte;
  }
  else if (name.compare(setAddressFunctionName) == 0) {
    return setAddress;
  }
  else {
    return functionNameNone;
  }
}


};
