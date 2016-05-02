#include "gamedata/LaylaObjectPatternToken.h"
#include "util/ByteConversion.h"
#include "util/StringConversion.h"
#include "exception/TGenericException.h"
#include <iostream>

namespace Lonely {


LaylaObjectPatternToken::LaylaObjectPatternToken() { };
  
LaylaObjectPatternToken::~LaylaObjectPatternToken() { };
  
int LaylaObjectPatternToken::save(Tstring& dst) const {
  int byteCount = 0;
  Tbyte buffer[maxDataSize_];
  
  ByteConversion::toBytes(type(),
                          buffer,
                          ByteSizes::uint8Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  dst += Tstring((char*)buffer, ByteSizes::uint8Size);
  byteCount += ByteSizes::uint8Size;
  
  writeToData(buffer);
  dst += Tstring((char*)buffer, size());
  byteCount += size();
  
  return byteCount;
}
  
LaylaObjectPatternToken*
    LaylaObjectPatternToken::loadAndAllocateToken(const Tbyte* src) {
  
  LaylaObjectPatternTypes::LaylaObjectPatternType patternType
    = static_cast<LaylaObjectPatternTypes::LaylaObjectPatternType>
       (ByteConversion::fromBytes(src,
                          ByteSizes::uint8Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign));
  src += ByteSizes::uint8Size;
  
  switch (patternType) {
  case LaylaObjectPatternTypes::delay:
    return new LaylaObjectDelayToken(src);
    break;
  case LaylaObjectPatternTypes::standardSpawn:
    return new LaylaObjectStandardSpawnToken(src);
    break;
  case LaylaObjectPatternTypes::expandedSpawn:
    return new LaylaObjectExpandedSpawnToken(src);
    break;
  default:
    throw TGenericException(TALES_SRCANDLINE,
                            "LaylaObjectPatternToken::loadAndAllocateToken()",
                            "Default case on LaylaObjectPatternType");
    break;
  }
}

LaylaObjectDelayToken::LaylaObjectDelayToken(Tbyte delayAmount__)
  : LaylaObjectPatternToken(),
    delayAmount_(delayAmount__) { };
  
LaylaObjectDelayToken::LaylaObjectDelayToken(const Tbyte* src)
  : LaylaObjectPatternToken(),
    delayAmount_(0) {
  delayAmount_ = *src & 0x1F;
}
  
int LaylaObjectDelayToken::writeToData(Tbyte* dst) const {
  *dst = (delayAmount_ | 0x80);
  
  return size();
}
  
int LaylaObjectDelayToken::size() const {
  return 1;
}

LaylaObjectPatternTypes::LaylaObjectPatternType
    LaylaObjectDelayToken::type() const {
  return LaylaObjectPatternTypes::delay;
}

Tbyte LaylaObjectDelayToken::delayAmount() const {
  return delayAmount_;
}

void LaylaObjectDelayToken::setDelayAmount(Tbyte delayAmount__) {
  delayAmount_ = delayAmount__;
}
  
Tstring LaylaObjectDelayToken::descriptionString() const {
  return Tstring("Delay: ")
    + StringConversion::toString((int)delayAmount_);
//    + " subtiles";
}
  
LaylaObjectPatternToken* LaylaObjectDelayToken::cloneNew() const {
  return new LaylaObjectDelayToken(*this);
}

LaylaObjectStandardSpawnToken::LaylaObjectStandardSpawnToken(
                                Tbyte objectType__,
                                bool continueInterpreting__)
  : LaylaObjectPatternToken(),
    objectType_(objectType__),
    continueInterpreting_(continueInterpreting__) { };
  
int LaylaObjectStandardSpawnToken::writeToData(Tbyte* dst) const {
  *dst = objectType_;
  
  if (continueInterpreting_) {
    *dst |= 0x40;
  }
  
  return size();
}
  
LaylaObjectStandardSpawnToken
    ::LaylaObjectStandardSpawnToken(const Tbyte* src)
  : LaylaObjectPatternToken(),
    objectType_(0),
    continueInterpreting_(false) {
  objectType_ = *src & 0x1F;
  continueInterpreting_ = (*src & 0x40) != 0;
}
  
int LaylaObjectStandardSpawnToken::size() const {
  return 1;
}

LaylaObjectPatternTypes::LaylaObjectPatternType
    LaylaObjectStandardSpawnToken::type() const {
  return LaylaObjectPatternTypes::standardSpawn;
}

Tbyte LaylaObjectStandardSpawnToken::objectType() const {
  return objectType_;
}

void LaylaObjectStandardSpawnToken::setObjectType(Tbyte objectType__) {
  objectType_ = objectType__;
}

bool LaylaObjectStandardSpawnToken::continueInterpreting() const {
  return continueInterpreting_;
}

void LaylaObjectStandardSpawnToken
    ::setContinueInterpreting(bool continueInterpreting__) {
  continueInterpreting_ = continueInterpreting__;
}
  
LaylaObjectPatternToken* LaylaObjectStandardSpawnToken
    ::cloneNew() const {
  return new LaylaObjectStandardSpawnToken(*this);
}
  
Tstring LaylaObjectStandardSpawnToken
    ::descriptionString() const {
  Tstring description = Tstring("Spawn: obj ")
    + StringConversion::toString((int)objectType_);
  
  if (continueInterpreting_) {
    description += " (c)";
  }
  
  return description;
}

LaylaObjectExpandedSpawnToken
    ::LaylaObjectExpandedSpawnToken(Tbyte objectType__,
                                bool continueInterpreting__,
                                Tbyte spawnCount__,
                                bool positionFlag__,
                                Tbyte yNybble__)
  : LaylaObjectStandardSpawnToken(objectType__,
                                  continueInterpreting__),
    spawnCount_(spawnCount__),
    positionFlag_(positionFlag__),
    yNybble_(yNybble__) { };
  
LaylaObjectExpandedSpawnToken
    ::LaylaObjectExpandedSpawnToken(const Tbyte* src)
  : LaylaObjectStandardSpawnToken(src),
    spawnCount_(0),
    positionFlag_(false),
    yNybble_(0) {
  spawnCount_ = (*(src + 1) & 0x30) >> 4;
  positionFlag_ = (*(src + 1) & 0x80) != 0;
  yNybble_ = *(src + 1) & 0x0F;
  
  // ?
  continueInterpreting_ = (*(src + 1) & 0x40) != 0;
}
  
int LaylaObjectExpandedSpawnToken
    ::writeToData(Tbyte* dst) const {
  LaylaObjectStandardSpawnToken::writeToData(dst);
  
  *dst |= (spawnCount_ << 4);
  *(dst + 1) = yNybble_;
  
  if (positionFlag_) {
    *(dst + 1) |= 0x80;
  }
  
  return size();
}
  
int LaylaObjectExpandedSpawnToken::size() const {
  return 2;
}

LaylaObjectPatternTypes::LaylaObjectPatternType
    LaylaObjectExpandedSpawnToken::type() const {
  return LaylaObjectPatternTypes::expandedSpawn;
}

Tbyte LaylaObjectExpandedSpawnToken::spawnCount() const {
  return spawnCount_;
}

void LaylaObjectExpandedSpawnToken::setSpawnCount(Tbyte spawnCount__) {
  spawnCount_ = spawnCount__;
}

bool LaylaObjectExpandedSpawnToken::positionFlag() const {
  return positionFlag_;
}

void LaylaObjectExpandedSpawnToken::setPositionFlag(bool positionFlag__) {
  positionFlag_ = positionFlag__;
}

Tbyte LaylaObjectExpandedSpawnToken::yNybble() const {
  return yNybble_;
}

void LaylaObjectExpandedSpawnToken::setYNybble(Tbyte yNybble__) {
  yNybble_ = yNybble__;
}
  
LaylaObjectPatternToken* LaylaObjectExpandedSpawnToken
    ::cloneNew() const {
  return new LaylaObjectExpandedSpawnToken(*this);
}
  
int LaylaObjectExpandedSpawnToken::realSpawnCount() const {
  return (int)(spawnCount_) + 1;
}
  
int LaylaObjectExpandedSpawnToken
    ::realY() const {
  return (yNybble_ << 4);
}
  
Tstring LaylaObjectExpandedSpawnToken
    ::descriptionString() const {
  Tstring description = Tstring("Special: ")
    + StringConversion::toString((int)objectType_)
    + ", "
    + StringConversion::toString((int)spawnCount_)
    + ", "
    + StringConversion::toString((int)positionFlag_)
    + ", "
    + StringConversion::toString((int)yNybble_);
  
  if (continueInterpreting_) {
    description += " (c)";
  }
  
  return description;
}


};
