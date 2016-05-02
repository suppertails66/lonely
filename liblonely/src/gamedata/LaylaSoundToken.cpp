#include "gamedata/LaylaSoundToken.h"
#include "util/ByteConversion.h"
#include <iostream>

namespace Lonely {


LaylaSoundToken::LaylaSoundToken() { };
LaylaSoundToken::~LaylaSoundToken() {
  
}

//*******************************
// SoundNoteToken
//*******************************

SoundNoteToken::SoundNoteToken()
  : LaylaSoundToken(),
    note_(0),
    duration_(0) { };

SoundNoteToken::SoundNoteToken(Tbyte note__,
                                 Tbyte duration__)
  : LaylaSoundToken(),
    note_(note__),
    duration_(duration__) { };

LaylaSoundTypes::LaylaSoundType SoundNoteToken::type() const {
  return LaylaSoundTypes::note;
}

Tbyte SoundNoteToken::opcode() const {
  return note_;
}

int SoundNoteToken::size() const {
  return 2;
}

int SoundNoteToken::writeToData(Tbyte* dst) const {
  *(dst++) = note_;
  *(dst++) = duration_;
  return size();
}

int SoundNoteToken::readFromData(Tbyte* dst) {
  note_ = *(dst++);
  duration_ = *(dst++);
  return size();
}

//*******************************
// SoundDelayToken
//*******************************

SoundDelayToken::SoundDelayToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundDelayToken::SoundDelayToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundDelayToken::type() const {
  return LaylaSoundTypes::delay;
}

Tbyte SoundDelayToken::opcode() const {
  return 0x80;
}

int SoundDelayToken::size() const {
  return 2;
}

int SoundDelayToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundDelayToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundChannelVolumeToken
//*******************************

SoundChannelVolumeToken::SoundChannelVolumeToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundChannelVolumeToken::SoundChannelVolumeToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundChannelVolumeToken::type() const {
  return LaylaSoundTypes::channelVolume;
}

Tbyte SoundChannelVolumeToken::opcode() const {
  return 0x81;
}

int SoundChannelVolumeToken::size() const {
  return 2;
}

int SoundChannelVolumeToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundChannelVolumeToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundPlayNoiseToken
//*******************************

SoundPlayNoiseToken::SoundPlayNoiseToken()
  : LaylaSoundToken(),
    reg1_(0),
    reg2_(0) { };

SoundPlayNoiseToken::SoundPlayNoiseToken(Tbyte reg1__,
                        Tbyte reg2__)
  : LaylaSoundToken(),
    reg1_(reg1__),
    reg2_(reg2__) { };

LaylaSoundTypes::LaylaSoundType SoundPlayNoiseToken::type() const {
  return LaylaSoundTypes::playNoise;
}

Tbyte SoundPlayNoiseToken::opcode() const {
  return 0x82;
}

int SoundPlayNoiseToken::size() const {
  return 3;
}

int SoundPlayNoiseToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = reg1_;
  *(dst++) = reg2_;
  return size();
}

int SoundPlayNoiseToken::readFromData(Tbyte* dst) {
  ++dst;
  reg1_ = *(dst++);
  reg2_ = *(dst++);
  return size();
}

//*******************************
// SoundSetLoopToken
//*******************************

SoundSetLoopToken::SoundSetLoopToken()
  : LaylaSoundToken() { };

LaylaSoundTypes::LaylaSoundType SoundSetLoopToken::type() const {
  return LaylaSoundTypes::setLoop;
}

Tbyte SoundSetLoopToken::opcode() const {
  return 0x83;
}

int SoundSetLoopToken::size() const {
  return 2;
}

int SoundSetLoopToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = 0;
  return size();
}

int SoundSetLoopToken::readFromData(Tbyte* dst) {
  ++dst;
  ++dst;
  return size();
}

//*******************************
// SoundPlayLoopToken
//*******************************

SoundPlayLoopToken::SoundPlayLoopToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundPlayLoopToken::SoundPlayLoopToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundPlayLoopToken::type() const {
  return LaylaSoundTypes::playLoop;
}

Tbyte SoundPlayLoopToken::opcode() const {
  return 0x84;
}

int SoundPlayLoopToken::size() const {
  return 2;
}

int SoundPlayLoopToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundPlayLoopToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundBaseTempoToken
//*******************************

SoundBaseTempoToken::SoundBaseTempoToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundBaseTempoToken::SoundBaseTempoToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundBaseTempoToken::type() const {
  return LaylaSoundTypes::baseTempo;
}

Tbyte SoundBaseTempoToken::opcode() const {
  return 0x85;
}

int SoundBaseTempoToken::size() const {
  return 2;
}

int SoundBaseTempoToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundBaseTempoToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundDriverDelayToken
//*******************************

SoundDriverDelayToken::SoundDriverDelayToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundDriverDelayToken::SoundDriverDelayToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundDriverDelayToken::type() const {
  return LaylaSoundTypes::driverDelay;
}

Tbyte SoundDriverDelayToken::opcode() const {
  return 0x86;
}

int SoundDriverDelayToken::size() const {
  return 2;
}

int SoundDriverDelayToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundDriverDelayToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundChannelKeyShiftToken
//*******************************

SoundChannelKeyShiftToken::SoundChannelKeyShiftToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundChannelKeyShiftToken::SoundChannelKeyShiftToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundChannelKeyShiftToken::type() const {
  return LaylaSoundTypes::channelKeyShift;
}

Tbyte SoundChannelKeyShiftToken::opcode() const {
  return 0x87;
}

int SoundChannelKeyShiftToken::size() const {
  return 2;
}

int SoundChannelKeyShiftToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundChannelKeyShiftToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundMasterKeyShiftToken
//*******************************

SoundMasterKeyShiftToken::SoundMasterKeyShiftToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundMasterKeyShiftToken::SoundMasterKeyShiftToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundMasterKeyShiftToken::type() const {
  return LaylaSoundTypes::masterKeyShift;
}

Tbyte SoundMasterKeyShiftToken::opcode() const {
  return 0x88;
}

int SoundMasterKeyShiftToken::size() const {
  return 2;
}

int SoundMasterKeyShiftToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundMasterKeyShiftToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundEffectsToken
//*******************************

SoundEffectsToken::SoundEffectsToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundEffectsToken::SoundEffectsToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundEffectsToken::type() const {
  return LaylaSoundTypes::effects;
}

Tbyte SoundEffectsToken::opcode() const {
  return 0x89;
}

int SoundEffectsToken::size() const {
  return 2;
}

int SoundEffectsToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundEffectsToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundCallPatternToken
//*******************************

SoundCallPatternToken::SoundCallPatternToken()
  : LaylaSoundToken(),
    address_(0) { };

SoundCallPatternToken::SoundCallPatternToken(Taddress address__)
  : LaylaSoundToken(),
    address_(address__) { };

LaylaSoundTypes::LaylaSoundType SoundCallPatternToken::type() const {
  return LaylaSoundTypes::callPattern;
}

Tbyte SoundCallPatternToken::opcode() const {
  return 0x8A;
}

int SoundCallPatternToken::size() const {
  return 3;
}

int SoundCallPatternToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  ByteConversion::toBytes(address_,
                          dst,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  return size();
}

int SoundCallPatternToken::readFromData(Tbyte* dst) {
  ++dst;
  address_ = ByteConversion::fromBytes(dst,
                                       ByteSizes::uint16Size,
                                       EndiannessTypes::little,
                                       SignednessTypes::nosign);
  return size();
}

//*******************************
// SoundReturnFromPatternToken
//*******************************

SoundReturnFromPatternToken::SoundReturnFromPatternToken()
  : LaylaSoundToken() { };

LaylaSoundTypes::LaylaSoundType SoundReturnFromPatternToken::type() const {
  return LaylaSoundTypes::returnFromPattern;
}

Tbyte SoundReturnFromPatternToken::opcode() const {
  return 0x8B;
}

int SoundReturnFromPatternToken::size() const {
  return 2;
}

int SoundReturnFromPatternToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = 0;
  return size();
}

int SoundReturnFromPatternToken::readFromData(Tbyte* dst) {
  ++dst;
  ++dst;
  return size();
}

//*******************************
// SoundChannelDecayToken
//*******************************

SoundChannelDecayToken::SoundChannelDecayToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundChannelDecayToken::SoundChannelDecayToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundChannelDecayToken::type() const {
  return LaylaSoundTypes::channelDecay;
}

Tbyte SoundChannelDecayToken::opcode() const {
  return 0x8C;
}

int SoundChannelDecayToken::size() const {
  return 2;
}

int SoundChannelDecayToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundChannelDecayToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundOp8DToken
//*******************************

SoundOp8DToken::SoundOp8DToken()
  : LaylaSoundToken() { };

LaylaSoundTypes::LaylaSoundType SoundOp8DToken::type() const {
  return LaylaSoundTypes::dummy8d;
}

Tbyte SoundOp8DToken::opcode() const {
  return 0x8D;
}

int SoundOp8DToken::size() const {
  return 2;
}

int SoundOp8DToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = 0;
  return size();
}

int SoundOp8DToken::readFromData(Tbyte* dst) {
  ++dst;
  ++dst;
  return size();
}

//*******************************
// SoundDriverSlowdownToken
//*******************************

SoundDriverSlowdownToken::SoundDriverSlowdownToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundDriverSlowdownToken::SoundDriverSlowdownToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundDriverSlowdownToken::type() const {
  return LaylaSoundTypes::channelDecay;
}

Tbyte SoundDriverSlowdownToken::opcode() const {
  return 0x8E;
}

int SoundDriverSlowdownToken::size() const {
  return 2;
}

int SoundDriverSlowdownToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundDriverSlowdownToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundOp8FToken
//*******************************

SoundOp8FToken::SoundOp8FToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundOp8FToken::SoundOp8FToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundOp8FToken::type() const {
  return LaylaSoundTypes::op8f;
}

Tbyte SoundOp8FToken::opcode() const {
  return 0x8F;
}

int SoundOp8FToken::size() const {
  return 2;
}

int SoundOp8FToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundOp8FToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundJumpToAddressToken
//*******************************

SoundJumpToAddressToken::SoundJumpToAddressToken()
  : LaylaSoundToken(),
    address_(0) { };

SoundJumpToAddressToken::SoundJumpToAddressToken(Taddress address__)
  : LaylaSoundToken(),
    address_(address__) { };

LaylaSoundTypes::LaylaSoundType SoundJumpToAddressToken::type() const {
  return LaylaSoundTypes::jumpToAddress;
}

Tbyte SoundJumpToAddressToken::opcode() const {
  return 0x90;
}

int SoundJumpToAddressToken::size() const {
  return 3;
}

int SoundJumpToAddressToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  ByteConversion::toBytes(address_,
                          dst,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  return size();
}

int SoundJumpToAddressToken::readFromData(Tbyte* dst) {
  ++dst;
  address_ = ByteConversion::fromBytes(dst,
                                       ByteSizes::uint16Size,
                                       EndiannessTypes::little,
                                       SignednessTypes::nosign);
  return size();
}

//*******************************
// SoundSetReg1Token
//*******************************

SoundSetReg1Token::SoundSetReg1Token()
  : LaylaSoundToken(),
    value_(0) { };

SoundSetReg1Token::SoundSetReg1Token(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundSetReg1Token::type() const {
  return LaylaSoundTypes::setReg1;
}

Tbyte SoundSetReg1Token::opcode() const {
  return 0x91;
}

int SoundSetReg1Token::size() const {
  return 2;
}

int SoundSetReg1Token::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundSetReg1Token::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundSetReg2Token
//*******************************

SoundSetReg2Token::SoundSetReg2Token()
  : LaylaSoundToken(),
    value_(0) { };

SoundSetReg2Token::SoundSetReg2Token(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundSetReg2Token::type() const {
  return LaylaSoundTypes::setReg2;
}

Tbyte SoundSetReg2Token::opcode() const {
  return 0x92;
}

int SoundSetReg2Token::size() const {
  return 2;
}

int SoundSetReg2Token::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundSetReg2Token::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundSetReg3Token
//*******************************

SoundSetReg3Token::SoundSetReg3Token()
  : LaylaSoundToken(),
    value_(0) { };

SoundSetReg3Token::SoundSetReg3Token(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundSetReg3Token::type() const {
  return LaylaSoundTypes::setReg3;
}

Tbyte SoundSetReg3Token::opcode() const {
  return 0x93;
}

int SoundSetReg3Token::size() const {
  return 2;
}

int SoundSetReg3Token::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundSetReg3Token::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundLengthEffectToken
//*******************************

SoundLengthEffectToken::SoundLengthEffectToken()
  : LaylaSoundToken(),
    value_(0) { };

SoundLengthEffectToken::SoundLengthEffectToken(Tbyte value__)
  : LaylaSoundToken(),
    value_(value__) { };

LaylaSoundTypes::LaylaSoundType SoundLengthEffectToken::type() const {
  return LaylaSoundTypes::lengthEffect;
}

Tbyte SoundLengthEffectToken::opcode() const {
  return 0x94;
}

int SoundLengthEffectToken::size() const {
  return 2;
}

int SoundLengthEffectToken::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = value_;
  return size();
}

int SoundLengthEffectToken::readFromData(Tbyte* dst) {
  ++dst;
  value_ = *(dst++);
  return size();
}

//*******************************
// SoundOp95Token
//*******************************

SoundOp95Token::SoundOp95Token()
  : LaylaSoundToken() { };

LaylaSoundTypes::LaylaSoundType SoundOp95Token::type() const {
  return LaylaSoundTypes::dummy95;
}

Tbyte SoundOp95Token::opcode() const {
  return 0x95;
}

int SoundOp95Token::size() const {
  return 2;
}

int SoundOp95Token::writeToData(Tbyte* dst) const {
  *(dst++) = opcode();
  *(dst++) = 0;
  return size();
}

int SoundOp95Token::readFromData(Tbyte* dst) {
  ++dst;
  ++dst;
  return size();
}


};
