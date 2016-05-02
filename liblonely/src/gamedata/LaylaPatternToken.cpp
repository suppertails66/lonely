#include "gamedata/LaylaPatternToken.h"
#include "util/ByteConversion.h"
#include "util/StringConversion.h"
#include "exception/TGenericException.h"
#include <iostream>

namespace Lonely {


LaylaPatternToken::LaylaPatternToken() { };
  
LaylaPatternToken::~LaylaPatternToken() { };
  
LaylaPatternToken* LaylaPatternToken::loadAndAllocateToken(const Tbyte* src) {
  
  LaylaPatternTypes::LaylaPatternType patternType
    = static_cast<LaylaPatternTypes::LaylaPatternType>
       (ByteConversion::fromBytes(src,
                          ByteSizes::uint8Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign));
  src += ByteSizes::uint8Size;
  
//  std::cout << patternType << std::endl;
  
  LaylaPatternToken* token = NULL;
  switch (patternType) {
  case LaylaPatternTypes::spawnPoint:
    token = new SpawnPointToken();
    break;
  case LaylaPatternTypes::standardPattern:
    token = new StandardPatternToken();
    break;
  case LaylaPatternTypes::spawnBoss:
    token = new SpawnBossToken();
    break;
  case LaylaPatternTypes::setScrolling:
    token = new SetScrollingToken();
    break;
  case LaylaPatternTypes::setArea:
    token = new SetAreaToken();
    break;
  case LaylaPatternTypes::setMap:
    token = new SetMapToken();
    break;
  case LaylaPatternTypes::setElevators:
    token = new SetElevatorsToken();
    break;
  case LaylaPatternTypes::jumpToPosition:
    token = new JumpToPositionToken();
    break;
  default:
    throw TGenericException(TALES_SRCANDLINE,
                            "LaylaPatternToken::loadAndAllocateToken()",
                            "Default case on LaylaPatternType");
    break;
  }
  
  token->loadInternal(src);
  
  return token;
}
  
int LaylaPatternToken::save(Tstring& dst) const {
  int byteCount = 0;
  Tbyte buffer[maxDataSize_];
  
  ByteConversion::toBytes(type(),
                          buffer,
                          ByteSizes::uint8Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  dst += Tstring((char*)buffer, ByteSizes::uint8Size);
  byteCount += ByteSizes::uint8Size;
  
  byteCount += saveInternal(buffer);
  dst += Tstring((char*)buffer, dataSerializedSize());
  
  return byteCount;
}
  
int LaylaPatternToken::saveInternal(Tbyte* dst) const {
  writeToData(dst);
  return serializedSize();
}
  
int LaylaPatternToken::loadInternal(const Tbyte* dst) {
  readFromData(dst);
  return size();
}
  
int LaylaPatternToken::serializedSize() const {
  return size() + 1;
}
  
int LaylaPatternToken::dataSerializedSize() const {
  return size();
}

SpawnPointToken::SpawnPointToken()
  : LaylaPatternToken(),
    spawnNum_(0) { };

SpawnPointToken::SpawnPointToken(int spawnNum__)
  : LaylaPatternToken(),
    spawnNum_(spawnNum__) { };
  
int SpawnPointToken::readFromData(const Tbyte* src) {
  // do nothing
  
  return size();
}
  
int SpawnPointToken::writeToData(Tbyte* dst) const {
  // do nothing
  
  return size();
}
  
int SpawnPointToken::saveInternal(Tbyte* dst) const {
  *(dst++) = spawnNum_;
  
//  std::cout << "out: " << spawnNum_ << std::endl;
  
  return serializedSize();
}
  
int SpawnPointToken::loadInternal(const Tbyte* dst) {
  spawnNum_ = *(dst++);
  
//  std::cout << "in: " << spawnNum_ << std::endl;
  
  return serializedSize();
}
  
int SpawnPointToken::serializedSize() const {
  return 2;
}
  
int SpawnPointToken::dataSerializedSize() const {
  return 1;
}

LaylaPatternTypes::LaylaPatternType SpawnPointToken::type() const {
  return LaylaPatternTypes::spawnPoint;
}

int SpawnPointToken::size() const {
  return 0;
}

int SpawnPointToken::spawnNum() const {
  return spawnNum_;
}

void SpawnPointToken::setSpawnNum(int spawnNum__) {
  spawnNum_ = spawnNum__;
}
  
LaylaPatternToken* SpawnPointToken::cloneNew() const {
  return new SpawnPointToken(spawnNum_);
}
  
Tstring SpawnPointToken::descriptionString() const {
  return Tstring("Spawn point: #")
            + StringConversion::toString((int)spawnNum_);
}
  
StandardPatternToken::StandardPatternToken()
  : LaylaPatternToken(),
    patternNum_(0) { };
  
StandardPatternToken::StandardPatternToken(Tbyte patternNum__)
  : LaylaPatternToken(),
    patternNum_(patternNum__) { };
  
int StandardPatternToken::readFromData(const Tbyte* src) {
  patternNum_ = *(src++);
  
  return size();
}
  
int StandardPatternToken::writeToData(Tbyte* dst) const {
  *(dst++) = patternNum_;
  
  return size();
}

LaylaPatternTypes::LaylaPatternType StandardPatternToken::type() const {
  return LaylaPatternTypes::standardPattern;
}

int StandardPatternToken::size() const {
  return 1;
}

Tbyte StandardPatternToken::patternNum() const {
  return patternNum_;
}

void StandardPatternToken::setPatternNum(Tbyte patternNum__) {
  patternNum_ = patternNum__;
}
  
LaylaPatternToken* StandardPatternToken::cloneNew() const {
  return new StandardPatternToken(patternNum_);
}
  
Tstring StandardPatternToken::descriptionString() const {
  return Tstring("Standard pattern: #")
            + StringConversion::toString((int)patternNum_);
}

SpawnBossToken::SpawnBossToken()
  : LaylaPatternToken(),
    bossNum_(0) { };

SpawnBossToken::SpawnBossToken(Tbyte bossNum__)
  : LaylaPatternToken(),
    bossNum_(bossNum__) { };
  
int SpawnBossToken::readFromData(const Tbyte* src) {
  ++src;
  bossNum_ = *(src++);
  
  return size();
}
  
int SpawnBossToken::writeToData(Tbyte* dst) const {
  *(dst++) = 0xFA;
  *(dst++) = bossNum_;
  
  return size();
}

LaylaPatternTypes::LaylaPatternType SpawnBossToken::type() const {
  return LaylaPatternTypes::spawnBoss;
}

int SpawnBossToken::size() const {
  return 2;
}

Tbyte SpawnBossToken::bossNum() const {
  return bossNum_;
}

void SpawnBossToken::setBossNum(Tbyte bossNum__) {
  bossNum_ = bossNum__;
}
  
LaylaPatternToken* SpawnBossToken::cloneNew() const {
  return new SpawnBossToken(bossNum_);
}
  
Tstring SpawnBossToken::descriptionString() const {
  return Tstring("Spawn boss: #")
            + StringConversion::toString((int)bossNum_);
}

SetScrollingToken::SetScrollingToken()
  : LaylaPatternToken(),
    killsToNextFood_(0),
    scrollingDisabled_(0) { };

SetScrollingToken::SetScrollingToken(Tbyte killsToNextFood__,
                    Tbyte scrollingDisabled__)
  : LaylaPatternToken(),
    killsToNextFood_(killsToNextFood__),
    scrollingDisabled_(scrollingDisabled__) { };
  
int SetScrollingToken::readFromData(const Tbyte* src) {
  ++src;
  killsToNextFood_ = *(src++);
  scrollingDisabled_ = *(src++);
  
  return size();
}
  
int SetScrollingToken::writeToData(Tbyte* dst) const {
  *(dst++) = 0xFB;
  *(dst++) = killsToNextFood_;
  *(dst++) = scrollingDisabled_;
  
  return size();
}

LaylaPatternTypes::LaylaPatternType SetScrollingToken::type() const {
  return LaylaPatternTypes::setScrolling;
}

int SetScrollingToken::size() const {
  return 3;
}

Tbyte SetScrollingToken::scrollingDisabled() const {
  return scrollingDisabled_;
}

void SetScrollingToken::setScrollingDisabled(Tbyte scrollingDisabled__) {
  scrollingDisabled_ = scrollingDisabled__;
}

Tbyte SetScrollingToken::killsToNextFood() const {
  return killsToNextFood_;
}

void SetScrollingToken::setKillsToNextFood(Tbyte killsToNextFood__) {
  killsToNextFood_ = killsToNextFood__;
}
  
LaylaPatternToken* SetScrollingToken::cloneNew() const {
  return new SetScrollingToken(killsToNextFood_, scrollingDisabled_);
}
  
Tstring SetScrollingToken::descriptionString() const {
/*  Tstring str("Scrolling: ");
  str += StringConversion::toString((int)scrollingDisabled_);
  str += ", food: "
            + StringConversion::toString((int)killsToNextFood_); */
  Tstring str("Scrolling: ");
  ((scrollingDisabled_ == 0)
                 ? (str += "on")
                 : (str += "off"));
  str += ", food: "
            + StringConversion::toString((int)killsToNextFood_);
  return str;
}
  
bool SetScrollingToken::scrollingIsDisabled() const {
  if (scrollingDisabled_ == scrollingEnabledValue) {
    return false;
  }
  else {
    return true;
  }
}

SetAreaToken::SetAreaToken()
  : LaylaPatternToken(),
    backgroundMetatile_(0),
    areaType_(0) { };

SetAreaToken::SetAreaToken(Tbyte backgroundMetatile__,
             Tbyte areaType__)
  : LaylaPatternToken(),
    backgroundMetatile_(backgroundMetatile__),
    areaType_(areaType__) { };
  
int SetAreaToken::readFromData(const Tbyte* src) {
  ++src;
  backgroundMetatile_ = *(src++);
  areaType_ = *(src++);
  
  return size();
}
  
int SetAreaToken::writeToData(Tbyte* dst) const {
  *(dst++) = 0xFC;
  *(dst++) = backgroundMetatile_;
  *(dst++) = areaType_;
  
  return size();
}

LaylaPatternTypes::LaylaPatternType SetAreaToken::type() const {
  return LaylaPatternTypes::setArea;
}

int SetAreaToken::size() const {
  return 3;
}

Tbyte SetAreaToken::areaType() const {
  return areaType_;
}

void SetAreaToken::setAreaType(Tbyte areaType__) {
  areaType_ = areaType__;
}

Tbyte SetAreaToken::backgroundMetatile() const {
  return backgroundMetatile_;
}

void SetAreaToken::setBackgroundMetatile(Tbyte backgroundMetatile__) {
  backgroundMetatile_ = backgroundMetatile__;
}
  
LaylaPatternToken* SetAreaToken::cloneNew() const {
  return new SetAreaToken(backgroundMetatile_, areaType_);
}
  
Tstring SetAreaToken::descriptionString() const {
  return Tstring("Area type: ")
            + StringConversion::toString((int)areaType_)
            + ", BG tile: "
            + StringConversion::toString((int)backgroundMetatile_);
}
  
SetAreaToken::AreaType SetAreaToken::realAreaType() const {
  if (areaType_ == caveID) {
    return cave;
  }
  else if (areaType_ & baseFlag) {
    return base;
  }
  else {
    return boss;
  }
}

void SetAreaToken::setRealAreaType(AreaType realAreaType__) {
  switch (realAreaType__) {
  case cave:
    areaType_ = caveID;
    break;
  case base:
    areaType_ = baseFlag;
    break;
  case boss:
    areaType_ = bossFlag;
    break;
  default:
    break;
  }
}

SetMapToken::SetMapToken(Tbyte mapNum__,
            Tbyte param2__)
  : LaylaPatternToken(),
    mapNum_(mapNum__),
    param2_(param2__) { };

SetMapToken::SetMapToken()
  : LaylaPatternToken(),
    mapNum_(0),
    param2_(0) { };
  
int SetMapToken::readFromData(const Tbyte* src) {
  ++src;
  mapNum_ = *(src++);
  param2_ = *(src++);
  
  return size();
}
  
int SetMapToken::writeToData(Tbyte* dst) const {
  *(dst++) = 0xFD;
  *(dst++) = mapNum_;
  *(dst++) = param2_;
  
  return size();
}

LaylaPatternTypes::LaylaPatternType SetMapToken::type() const {
  return LaylaPatternTypes::setMap;
}

int SetMapToken::size() const {
  return 3;
}

Tbyte SetMapToken::mapNum() const {
  return mapNum_;
}

void SetMapToken::setMapNum(Tbyte mapNum__) {
  mapNum_ = mapNum__;
}

Tbyte SetMapToken::param2() const {
  return param2_;
}

void SetMapToken::setParam2(int param2__) {
  param2_ = param2__;
}
  
LaylaPatternToken* SetMapToken::cloneNew() const {
  return new SetMapToken(mapNum_, param2_);
}
  
Tstring SetMapToken::descriptionString() const {
  return Tstring("Respawn: ")
            + StringConversion::toString((int)mapNum_)
            + ", y-flag: "
            + StringConversion::toString((int)param2_);
}

SetElevatorsToken::SetElevatorsToken()
  : LaylaPatternToken(),
    upDestination_(0),
    downDestination_(0) { };

SetElevatorsToken::SetElevatorsToken(Tbyte upDestination__,
              Tbyte downDestination__)
  : LaylaPatternToken(),
    upDestination_(upDestination__),
    downDestination_(downDestination__) { };
  
int SetElevatorsToken::readFromData(const Tbyte* src) {
  ++src;
  upDestination_ = *(src++);
  downDestination_ = *(src++);
  
  return size();
}
  
int SetElevatorsToken::writeToData(Tbyte* dst) const {
  *(dst++) = 0xFE;
  *(dst++) = upDestination_;
  *(dst++) = downDestination_;
  
  return size();
}

LaylaPatternTypes::LaylaPatternType SetElevatorsToken::type() const {
  return LaylaPatternTypes::setElevators;
}

int SetElevatorsToken::size() const {
  return 3;
}

Tbyte SetElevatorsToken::upDestination() const {
  return upDestination_;
}

void SetElevatorsToken::setUpDestination(Tbyte upDestination__) {
  upDestination_ = upDestination__;
}

Tbyte SetElevatorsToken::downDestination() const {
  return downDestination_;
}

void SetElevatorsToken::setDownDestination(Tbyte downDestination__) {
  downDestination_ = downDestination__;
}
  
LaylaPatternToken* SetElevatorsToken::cloneNew() const {
  return new SetElevatorsToken(upDestination_, downDestination_);
}
  
Tstring SetElevatorsToken::descriptionString() const {
  return Tstring("Elev: up ")
            + StringConversion::toString((int)upDestination_)
            + ", down "
            + StringConversion::toString((int)downDestination_);
}

JumpToPositionToken::JumpToPositionToken()
  : LaylaPatternToken(),
    offset_(0) { };

JumpToPositionToken::JumpToPositionToken(int offset__)
  : LaylaPatternToken(),
    offset_(offset__) { };
  
int JumpToPositionToken::readFromData(const Tbyte* src) {
  return 0;
}
  
int JumpToPositionToken::writeToData(Tbyte* dst) const {
/*  *(dst++) = 0xFF; 
  
  return size(); */
  
  // can't do this -- we don't have the offset info
  return 0;
}
  
int JumpToPositionToken::saveInternal(Tbyte* dst) const {
  ByteConversion::toBytes(offset_,
                          dst,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  
  return serializedSize();
}

int JumpToPositionToken::loadInternal(const Tbyte* dst) {
  offset_ = ByteConversion::fromBytes(
                          dst,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  
  return serializedSize();
}
  
int JumpToPositionToken::writeToData(Tbyte* dst,
                        Taddress address) const {
  *(dst++) = 0xFF;
  ByteConversion::toBytes(address,
                          dst,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  
  return size();
}
  
int JumpToPositionToken::serializedSize() const {
  return 4;
}

LaylaPatternTypes::LaylaPatternType JumpToPositionToken::type() const {
  return LaylaPatternTypes::jumpToPosition;
}

int JumpToPositionToken::size() const {
  return 3;
}

int JumpToPositionToken::offset() const {
  return offset_;
}

void JumpToPositionToken::setOffset(int offset__) {
  offset_ = offset__;
}
  
LaylaPatternToken* JumpToPositionToken::cloneNew() const {
  return new JumpToPositionToken(offset_);
}
  
Tstring JumpToPositionToken::descriptionString() const {
  return Tstring("Jump to pos: ")
            + StringConversion::toString((int)offset_);
}


};
