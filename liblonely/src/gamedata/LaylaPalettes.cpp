#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaPatternToken.h"
#include "util/StringConversion.h"

namespace Lonely {


const char* LaylaPalettes::offsetFilePalettesName_ = "Palettes";
  
LaylaPalettes::LaylaPalettes() {
  
}
  
LaylaPalettes::LaylaPalettes(const NesRom& rom,
              const LaylaOffsetFile& offsets) {
              
  Tstring palettesName(offsetFilePalettesName_);
  standardPaletteOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "StandardPaletteOffset"));
  bossPaletteOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "BossPaletteOffset"));
  caveReplacementTableOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "CaveReplacementTableOffset"));
  numCaveReplacementTableEntries_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "NumCaveReplacementTableEntries"));
  baseReplacementTableOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "BaseReplacementTableOffset"));
  numBaseReplacementTableEntries_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "NumBaseReplacementTableEntries"));
  bossReplacementTableOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "BossReplacementTableOffset"));
  numBossReplacementTableEntries_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "NumBossReplacementTableEntries"));
  laylaFadeTableOffset_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "LaylaFadeTableOffset"));
  numLaylaFadeTableEntries_ = StringConversion::stringToInt(
      offsets.ini().valueOfKey(palettesName,
                               "NumLaylaFadeTableEntries"));
  
  standardPalette_.readFullPaletteSet(rom.directRead(standardPaletteOffset_));
  bossPalette_.readFullPaletteSet(rom.directRead(bossPaletteOffset_));
  
//  caveReplacementBackgroundPalettes_.resize(numCaveReplacementTableEntries);
//  baseReplacementBackgroundPalettes_.resize(numBaseReplacementTableEntries);
//  bossReplacementSpritePalettes_.resize(numBossReplacementTableEntries);
  
  readPatchTable(caveReplacementBackgroundPalettes_,
                 rom.directRead(caveReplacementTableOffset_),
                 caveReplacementBackgroundStartIndex_,
                 numCaveReplacementTableEntries_,
                 caveReplacementBackgroundColorsPerEntry_);
  
  readPatchTable(baseReplacementBackgroundPalettes_,
                 rom.directRead(baseReplacementTableOffset_),
                 baseReplacementBackgroundStartIndex_,
                 numBaseReplacementTableEntries_,
                 baseReplacementBackgroundColorsPerEntry_);
  
  readPatchTable(bossReplacementSpritePalettes_,
                 rom.directRead(bossReplacementTableOffset_),
                 bossReplacementSpriteStartIndex_,
                 numBossReplacementTableEntries_,
                 bossReplacementSpriteColorsPerEntry_);
  
  readPatchTable(laylaFadeSpritePalettes_,
                 rom.directRead(laylaFadeTableOffset_),
                 laylaFadeStartIndex_,
                 numLaylaFadeTableEntries_,
                 laylaFadeColorsPerEntry_);
}

void LaylaPalettes::exportToRom(NesRom& rom) const {
  standardPalette_.writeToData(rom.directWrite(standardPaletteOffset_));
  bossPalette_.writeToData(rom.directWrite(bossPaletteOffset_));
  
  writePatchTable(rom.directWrite(caveReplacementTableOffset_),
                  caveReplacementBackgroundPalettes_);
  writePatchTable(rom.directWrite(baseReplacementTableOffset_),
                  baseReplacementBackgroundPalettes_);
  writePatchTable(rom.directWrite(bossReplacementTableOffset_),
                  bossReplacementSpritePalettes_);
  writePatchTable(rom.directWrite(laylaFadeTableOffset_),
                  laylaFadeSpritePalettes_);
}

int LaylaPalettes::save(Tstring& data) const {
//  int byteCount = 0;
  SaveHelper saver(data,
                   DataChunkIDs::LaylaPalettes,
                   0);
  
  SaveHelper::saveInt(data, standardPaletteOffset_,
                                   ByteSizes::uint32Size);
  SaveHelper::saveInt(data, bossPaletteOffset_,
                                   ByteSizes::uint32Size);
  SaveHelper::saveInt(data, caveReplacementTableOffset_,
                                   ByteSizes::uint32Size);
  SaveHelper::saveInt(data, numCaveReplacementTableEntries_,
                                   ByteSizes::uint32Size);
  SaveHelper::saveInt(data, baseReplacementTableOffset_,
                                   ByteSizes::uint32Size);
  SaveHelper::saveInt(data, numBaseReplacementTableEntries_,
                                   ByteSizes::uint32Size);
  SaveHelper::saveInt(data, bossReplacementTableOffset_,
                                   ByteSizes::uint32Size);
  SaveHelper::saveInt(data, numBossReplacementTableEntries_,
                                   ByteSizes::uint32Size);
  
  Tbyte colorDataBuffer[NesColorData::size];
  standardPalette_.writeToData(colorDataBuffer);
  data += Tstring((char*)(colorDataBuffer), NesColorData::size);
  bossPalette_.writeToData(colorDataBuffer);
  data += Tstring((char*)(colorDataBuffer), NesColorData::size);
  
  savePatchTable(data,
                 caveReplacementBackgroundPalettes_);
  savePatchTable(data,
                 baseReplacementBackgroundPalettes_);
  savePatchTable(data,
                 bossReplacementSpritePalettes_);
  savePatchTable(data,
                 laylaFadeSpritePalettes_);
  
  return saver.finalize();
}

int LaylaPalettes::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);
  
  standardPaletteOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  bossPaletteOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  caveReplacementTableOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  numCaveReplacementTableEntries_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  baseReplacementTableOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  numBaseReplacementTableEntries_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  bossReplacementTableOffset_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  numBossReplacementTableEntries_ = LoadHelper::loadInt(data, byteCount,
                                               ByteSizes::uint32Size);
  
  byteCount += standardPalette_.readFromData(data + byteCount);
  byteCount += bossPalette_.readFromData(data + byteCount);
  
  byteCount += loadPatchTable(caveReplacementBackgroundPalettes_,
                              data + byteCount);
  byteCount += loadPatchTable(baseReplacementBackgroundPalettes_,
                              data + byteCount);
  byteCount += loadPatchTable(bossReplacementSpritePalettes_,
                              data + byteCount);
  byteCount += loadPatchTable(laylaFadeSpritePalettes_,
                              data + byteCount);
  
  return byteCount;
}
  
int LaylaPalettes::cavePatchIndex(int levelNum) const {
  return (levelNum % numCaveReplacementTableEntries_);
}

int LaylaPalettes::basePatchIndex(int levelNum) const {
  return (levelNum % numBaseReplacementTableEntries_);
}

int LaylaPalettes::bossPatchIndex(int levelNum) const {
  return (levelNum % numBossReplacementTableEntries_);
}
  
NesColorData LaylaPalettes::generateCavePalette(int levelNum) const {
  NesColorData palettes = standardPalette_;
  
  LaylaPalettePatch patch = caveReplacementBackgroundPalettes_
                                [cavePatchIndex(levelNum)];
  patch.apply(palettes.backgroundPalettes());
  
  palettes.spritePalettes().palette(1)
    = palettes.backgroundPalettes().palette(1);
  
  return palettes;
}

NesColorData LaylaPalettes::generateBasePalette(int levelNum) const {
  NesColorData palettes = standardPalette_;
  
  LaylaPalettePatch patch = baseReplacementBackgroundPalettes_
                                [basePatchIndex(levelNum)];
  patch.apply(palettes.backgroundPalettes());
  
  return palettes;
}

NesColorData LaylaPalettes::generateBossPalette(int levelNum) const {
  NesColorData palettes = bossPalette_;
  
  LaylaPalettePatch patch = bossReplacementSpritePalettes_
                                [bossPatchIndex(levelNum)];
  patch.apply(palettes.spritePalettes());
  
  return palettes;
}
  
void LaylaPalettes::readPatchTable(PalettePatchArray& patches,
                    const Tbyte* src,
                    int startIndex,
                    int numEntries,
                    int colorsPerEntry) {
  patches.clear();
  patches.resize(numEntries);
                    
  for (int i = 0; i < numEntries; i++) {
    
    for (int j = 0; j < colorsPerEntry; j++) {
      NesColor color = NesColor(*(src++));
      
      patches[i].setAndEnableColor(j + startIndex, color);
    }
    
  }
}
  
void LaylaPalettes::writePatchTable(
                    Tbyte* dst,
                    const PalettePatchArray& patches) {
  for (int i = 0; i < patches.size(); i++) {
    
    const LaylaPalettePatch& patch = patches[i];
    
    for (int j = 0; j < patch.numColorsPerPatch(); j++) {
      if (patch.colorEnabled(j)) {
        *(dst++) = patch.color(j).nativeValue();
      }
    }
    
  }
}
  
int LaylaPalettes::savePatchTable(Tstring& data,
                    const PalettePatchArray& patches) {
  int byteCount = 0;
  
  byteCount += SaveHelper::saveInt(data,
                                   patches.size(),
                                   ByteSizes::uint8Size);
  for (int i = 0; i < patches.size(); i++) {
    byteCount += patches[i].save(data);
  }
  
  return byteCount;
}

int LaylaPalettes::loadPatchTable(PalettePatchArray& patches,
                    const Tbyte* data) {
  int byteCount = 0;
  
  patches.resize(LoadHelper::loadInt(data, byteCount,
                  ByteSizes::uint8Size));
  for (int i = 0; i < patches.size(); i++) {
    byteCount += patches[i].load(data + byteCount);
  }
  
  return byteCount;
}
  
NesColorData LaylaPalettes::generateAreaPalette(
    Tbyte areaNum,
    int levelNum) const {
  if (areaNum & SetAreaToken::bossFlag) {
    return generateBossPalette(levelNum);
  }
  else if (areaNum & SetAreaToken::baseFlag) {
    return generateBasePalette(levelNum);
  }
  else {
    return generateCavePalette(levelNum);
  }
}
  
NesColorData& LaylaPalettes::standardPalette() {
  return standardPalette_;
}

NesColorData& LaylaPalettes::bossPalette() {
  return bossPalette_;
}

LaylaPalettePatch& LaylaPalettes
  ::caveReplacementBackgroundPalette(int index) {
  return caveReplacementBackgroundPalettes_[index];
}

LaylaPalettePatch& LaylaPalettes
  ::baseReplacementBackgroundPalette(int index) {
  return baseReplacementBackgroundPalettes_[index];
}

LaylaPalettePatch& LaylaPalettes
  ::bossReplacementSpritePalette(int index) {
  return bossReplacementSpritePalettes_[index];
}

LaylaPalettePatch& LaylaPalettes
  ::laylaFadeSpritePalette(int index) {
  return laylaFadeSpritePalettes_[index];
}


};
