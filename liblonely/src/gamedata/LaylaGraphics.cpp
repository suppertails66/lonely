#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPatternToken.h"
#include "util/StringConversion.h"

namespace Lonely {


const char* LaylaGraphics::offsetFileGraphicsName_ = "Graphics";

LaylaGraphics::LaylaGraphics() { };

LaylaGraphics::LaylaGraphics(const NesRom& rom,
              const LaylaOffsetFile& offsets) {
  
  Tstring graphicsName(offsetFileGraphicsName_);
  caveSpritesOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "CaveSpritesOffset"));
  caveBackgroundOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "CaveBackgroundOffset"));
  baseSpritesOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "BaseSpritesOffset"));
  baseBackgroundOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "BaseBackgroundOffset"));
  bossSpritesOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "BossSpritesOffset"));
  bossBackgroundOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "BossBackgroundOffset"));
  titleSpritesOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "TitleSpritesOffset"));
  titleBackgroundOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(graphicsName,
                               "TitleBackgroundOffset"));
  
  caveSprites_.fromUncompressedData(rom.directRead(caveSpritesOffset_));
  caveBackground_.fromUncompressedData(rom.directRead(caveBackgroundOffset_));
  baseSprites_.fromUncompressedData(rom.directRead(baseSpritesOffset_));
  baseBackground_.fromUncompressedData(rom.directRead(baseBackgroundOffset_));
  bossSprites_.fromUncompressedData(rom.directRead(bossSpritesOffset_));
  bossBackground_.fromUncompressedData(rom.directRead(bossBackgroundOffset_));
  titleSprites_.fromUncompressedData(
      rom.directRead(titleSpritesOffset_));
  titleBackground_.fromUncompressedData(
      rom.directRead(titleBackgroundOffset_));
}

void LaylaGraphics::exportToRom(NesRom& rom) const {
  // probably don't implement this at some point
}

int LaylaGraphics::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LaylaPalettes,
                   0);
  
  SaveHelper::saveInt(data, caveSpritesOffset_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, caveBackgroundOffset_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, baseSpritesOffset_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, baseBackgroundOffset_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, bossSpritesOffset_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, bossBackgroundOffset_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, titleSpritesOffset_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, titleBackgroundOffset_,
                      ByteSizes::uint32Size);
  
  // Don't save the pattern tables -- use whatever was in the loaded ROM.
  // This gives the user more flexibility to change them later on, since
  // we don't provide graphics editing.
/*  caveSprites_.save(data);
  caveBackground_.save(data);
  baseSprites_.save(data);
  baseBackground_.save(data);
  bossSprites_.save(data);
  bossBackground_.save(data);
  titleSprites_.save(data);
  titleBackground_.save(data); */
  
  return saver.finalize();
}

int LaylaGraphics::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);
  
  caveSpritesOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  caveBackgroundOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  baseSpritesOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  baseBackgroundOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  bossSpritesOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  bossBackgroundOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  titleSpritesOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  titleBackgroundOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  
/*  byteCount += caveSprites_.load(data + byteCount);
  byteCount += caveBackground_.load(data + byteCount);
  byteCount += baseSprites_.load(data + byteCount);
  byteCount += baseBackground_.load(data + byteCount);
  byteCount += bossSprites_.load(data + byteCount);
  byteCount += bossBackground_.load(data + byteCount);
  byteCount += titleSprites_.load(data + byteCount);
  byteCount += titleBackground_.load(data + byteCount); */
  
  return byteCount;
}
  
NesPatternTable& LaylaGraphics::caveSprites() {
  return caveSprites_;
}

NesPatternTable& LaylaGraphics::caveBackground() {
  return caveBackground_;
}

NesPatternTable& LaylaGraphics::baseSprites() {
  return baseSprites_;
}

NesPatternTable& LaylaGraphics::baseBackground() {
  return baseBackground_;
}

NesPatternTable& LaylaGraphics::bossSprites() {
  return bossSprites_;
}

NesPatternTable& LaylaGraphics::bossBackground() {
  return bossBackground_;
}

NesPatternTable& LaylaGraphics::titleSprites() {
  return titleSprites_;
}

NesPatternTable& LaylaGraphics::titleBackground() {
  return titleBackground_;
}
  
const NesPatternTable& LaylaGraphics::caveSprites() const {
  return caveSprites_;
}

const NesPatternTable& LaylaGraphics::caveBackground() const {
  return caveBackground_;
}

const NesPatternTable& LaylaGraphics::baseSprites() const {
  return baseSprites_;
}

const NesPatternTable& LaylaGraphics::baseBackground() const {
  return baseBackground_;
}

const NesPatternTable& LaylaGraphics::bossSprites() const {
  return bossSprites_;
}

const NesPatternTable& LaylaGraphics::bossBackground() const {
  return bossBackground_;
}

const NesPatternTable& LaylaGraphics::titleSprites() const {
  return titleSprites_;
}

const NesPatternTable& LaylaGraphics::titleBackground() const {
  return titleBackground_;
}
  
NesPatternTable& LaylaGraphics::areaBackground(Tbyte areaNum) {
  if (areaNum & SetAreaToken::bossFlag) {
    return bossBackground_;
  }
  else if (areaNum & SetAreaToken::baseFlag) {
    return baseBackground_;
  }
  else {
    return caveBackground_;
  }
}

NesPatternTable& LaylaGraphics::areaSprites(Tbyte areaNum) {
  if (areaNum & SetAreaToken::bossFlag) {
    return bossSprites_;
  }
  else if (areaNum & SetAreaToken::baseFlag) {
    return baseSprites_;
  }
  else {
    return caveSprites_;
  }
}
  
const NesPatternTable& LaylaGraphics::areaBackground(Tbyte areaNum) const {
  if (areaNum & SetAreaToken::bossFlag) {
    return bossBackground_;
  }
  else if (areaNum & SetAreaToken::baseFlag) {
    return baseBackground_;
  }
  else {
    return caveBackground_;
  }
}

const NesPatternTable& LaylaGraphics::areaSprites(Tbyte areaNum) const {
  if (areaNum & SetAreaToken::bossFlag) {
    return bossSprites_;
  }
  else if (areaNum & SetAreaToken::baseFlag) {
    return baseSprites_;
  }
  else {
    return caveSprites_;
  }
}


};
