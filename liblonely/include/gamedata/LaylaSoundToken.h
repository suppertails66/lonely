#ifndef LAYLASOUNDTOKEN_H
#define LAYLASOUNDTOKEN_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tstring.h"
#include "structs/TpointerVector.h"

namespace Lonely {


class LaylaSoundTypes {
public:
  enum LaylaSoundType {
    none = 0,
    note,
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
};

class LaylaSoundToken {
public:
  const static int minTokenBufferSize = 4;
  
  LaylaSoundToken();
  virtual ~LaylaSoundToken();
  
  virtual LaylaSoundTypes::LaylaSoundType type() const =0;
  virtual Tbyte opcode() const =0;
  virtual int size() const =0;
  virtual int writeToData(Tbyte* dst) const =0;
  virtual int readFromData(Tbyte* dst) =0;
protected:
  
};

typedef TpointerVector<LaylaSoundToken> LaylaSoundTokenCollection;

class SoundNoteToken : public LaylaSoundToken {
public:
  SoundNoteToken();
  SoundNoteToken(Tbyte note__,
                 Tbyte duration__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte note_;
  Tbyte duration_;
};

class SoundDelayToken : public LaylaSoundToken {
public:
  SoundDelayToken();
  SoundDelayToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundChannelVolumeToken : public LaylaSoundToken {
public:
  SoundChannelVolumeToken();
  SoundChannelVolumeToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundPlayNoiseToken : public LaylaSoundToken {
public:
  SoundPlayNoiseToken();
  SoundPlayNoiseToken(Tbyte reg1__,
                          Tbyte reg2__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte reg1_;
  Tbyte reg2_;
};

class SoundSetLoopToken : public LaylaSoundToken {
public:
  SoundSetLoopToken();
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  
};

class SoundPlayLoopToken : public LaylaSoundToken {
public:
  SoundPlayLoopToken();
  SoundPlayLoopToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundBaseTempoToken : public LaylaSoundToken {
public:
  SoundBaseTempoToken();
  SoundBaseTempoToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundDriverDelayToken : public LaylaSoundToken {
public:
  SoundDriverDelayToken();
  SoundDriverDelayToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundChannelKeyShiftToken : public LaylaSoundToken {
public:
  SoundChannelKeyShiftToken();
  SoundChannelKeyShiftToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundMasterKeyShiftToken : public LaylaSoundToken {
public:
  SoundMasterKeyShiftToken();
  SoundMasterKeyShiftToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundEffectsToken : public LaylaSoundToken {
public:
  SoundEffectsToken();
  SoundEffectsToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundCallPatternToken : public LaylaSoundToken {
public:
  SoundCallPatternToken();
  SoundCallPatternToken(Taddress address__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Taddress address_;
};

class SoundReturnFromPatternToken : public LaylaSoundToken {
public:
  SoundReturnFromPatternToken();
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
};

class SoundChannelDecayToken : public LaylaSoundToken {
public:
  SoundChannelDecayToken();
  SoundChannelDecayToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundOp8DToken : public LaylaSoundToken {
public:
  SoundOp8DToken();
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  
};

class SoundDriverSlowdownToken : public LaylaSoundToken {
public:
  SoundDriverSlowdownToken();
  SoundDriverSlowdownToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundOp8FToken : public LaylaSoundToken {
public:
  SoundOp8FToken();
  SoundOp8FToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundJumpToAddressToken : public LaylaSoundToken {
public:
  SoundJumpToAddressToken();
  SoundJumpToAddressToken(Taddress address__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Taddress address_;
};

class SoundSetReg1Token : public LaylaSoundToken {
public:
  SoundSetReg1Token();
  SoundSetReg1Token(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundSetReg2Token : public LaylaSoundToken {
public:
  SoundSetReg2Token();
  SoundSetReg2Token(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundSetReg3Token : public LaylaSoundToken {
public:
  SoundSetReg3Token();
  SoundSetReg3Token(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundLengthEffectToken : public LaylaSoundToken {
public:
  SoundLengthEffectToken();
  SoundLengthEffectToken(Tbyte value__);
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
  Tbyte value_;
};

class SoundOp95Token : public LaylaSoundToken {
public:
  SoundOp95Token();
  
  virtual LaylaSoundTypes::LaylaSoundType type() const;
  virtual Tbyte opcode() const;
  virtual int size() const;
  virtual int writeToData(Tbyte* dst) const;
  virtual int readFromData(Tbyte* dst);
protected:
};


};


#endif
