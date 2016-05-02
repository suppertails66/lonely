#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaPalettes.h"
#include "nes/NesColor.h"
#include "nes/UxRomBanking.h"
#include "structs/Tarray.h"
#include "structs/AddressTableReader.h"
#include "util/StringConversion.h"
#include <cstring>
#include <iostream>

namespace Lonely {


LaylaLevel::LaylaLevel()
  : bossVulnerabilities_(numBossVulnerabilities),
    objectVulnerabilities_(numObjectVulnerabilities),
    objectSetNum_(0),
    objectCodeBlockBaseOffset_(0) { };
  
void LaylaLevel::readFromData(const NesRom& rom,
                  int bankNum,
                  int objectSetNum__,
                  Taddress objectCodeBlockStart,
                  Taddress objectCodeBlockEnd,
                  int numPatternDefinitionTableEntries,
                  int numObjectPatternTableEntries,
                  int objectPatternDataSize,
                  Taddress spawnTableStart,
                  int numSpawnTableEntries,
                  Taddress areaDataStart,
                  Taddress areaDataEnd) {
  // Get the direct address of the start of the bank
  Taddress bankBaseDirect = 
      UxRomBanking::bankedToDirectAddressMovable(
                                bankNum,
                                UxRomBanking::movableBankOffset);
                  
  commonCodeBlock_.resize(commonCodeBlockSize_);
  std::memcpy(commonCodeBlock_.data(),
              rom.directRead(bankBaseDirect + commonCodeBlockBaseOffset_),
              commonCodeBlockSize_);
              
  bossInitTable_.resize(bossInitTableSize_);
  std::memcpy(bossInitTable_.data(),
              rom.directRead(bankBaseDirect + bossInitTableBaseStart_),
              bossInitTableSize_);
              
  levelPointerTable_.resize(levelPointerTableSize_);
  std::memcpy(levelPointerTable_.data(),
              rom.directRead(bankBaseDirect + levelPointerTableBaseStart_),
              levelPointerTableSize_);
              
//  bossVulnerabilities_ = LaylaVulnerabilityArray(numBossVulnerabilities);
  bossVulnerabilities_.readFromData(
      rom.directRead(bankBaseDirect + bossVulnerabilityTableBase_));
              
//  objectVulnerabilities_ = LaylaVulnerabilityArray(numObjectVulnerabilities);
  objectVulnerabilities_.readFromData(
      rom.directRead(bankBaseDirect + objectVulnerabilityTableBase_));
  
  objectScores_.readFromData(
      rom.directRead(bankBaseDirect + objectScoreTableBase_));
              
  objectInitTable_.resize(objectInitTableSize_);
  std::memcpy(objectInitTable_.data(),
              rom.directRead(bankBaseDirect + objectInitTableBaseStart_),
              objectInitTableSize_);
  
  objectHps_.readFromData(
      rom.directRead(bankBaseDirect + objectHpTableBase_));
  
  baseMetatileSet_.readFromData(rom,
                                baseNumMetatilesPerSet_,
                                bankBaseDirect
                                  + baseEvenTileIndexTableBase_,
                                bankBaseDirect
                                  + baseOddTileIndexTableBase_,
                                bankBaseDirect
                                  + baseMetatileAttributeTableBase_,
                                bankBaseDirect
                                  + baseMetatilePropertiesBase_);
  
  Tarray<Taddress> addresses;
  AddressTableReader::readAddressTable(addresses,
                                       rom.directRead(spawnTableStart),
                                       numSpawnTableEntries,
                                       ByteSizes::uint16Size,
                                       EndiannessTypes::little);
          
  areaData_ = LaylaArea(rom.directRead(areaDataStart),
                 areaDataEnd - areaDataStart,
                 areaDataStart,
                 addresses);
                 
  patternDefinitions_ = LaylaPatternDefinitionTable(rom,
       numPatternDefinitionTableEntries,
       bankBaseDirect + patternDefinitionTableBase_);
                 
  Taddress objectPatternBaseDirect = bankBaseDirect
      + objectPatternTableBaseStart_;
  objectPatterns_ = LaylaObjectPatternTable(
                      rom.directRead(objectPatternBaseDirect),
                      numObjectPatternTableEntries,
                      objectPatternBaseDirect,
                      objectPatternDataSize);
  
  int objectCodeBlockSize = objectCodeBlockEnd - objectCodeBlockStart;
  objectCodeBlock_.resize(objectCodeBlockSize);
  std::memcpy(objectCodeBlock_.data(),
              rom.directRead(objectCodeBlockStart),
              objectCodeBlockSize);
  objectCodeBlockBaseOffset_ =
    UxRomBanking::directToBankedAddressMovable(objectCodeBlockStart)
        - UxRomBanking::movableBankOffset;
  
//  for (int i = 0; i < numPaletteCycleEntries_; i++) {
//  std::cout << bankBaseDirect + paletteCycleBaseAddress_ << std::endl;
  LaylaPalettes::readPatchTable(paletteCyclePatches_,
                 rom.directRead(bankBaseDirect + paletteCycleBaseAddress_),
                 paletteCyclePositionIndex_,
                 numPaletteCycleEntries_,
                 numColorsPerPaletteCycleEntry_);
  LaylaPalettes::readPatchTable(regularHpCyclePatches_,
                 rom.directRead(bankBaseDirect + regularHpCycleBaseAddress_),
                 0,
                 1,
                 1);
  LaylaPalettes::readPatchTable(lowHpCyclePatches_,
                 rom.directRead(bankBaseDirect + lowHpCycleBaseAddress_),
                 0,
                 1,
                 1);
  LaylaPalettes::readPatchTable(specialFlashCyclePatches_,
                 rom.directRead(bankBaseDirect
                    + specialFlashCycleBaseAddress_),
                 0,
                 1,
                 1);
//  }
  
  objectSetNum_ = objectSetNum__;
  
/*    for (int j = 0; j < patternDefinitions_.numPatterns(); j++) {
    LaylaPattern pattern = patternDefinitions_.pattern(j);
    
    std::cout << "pattern " << j << std::endl;
    std::cout << "\t";
    
    for (int k = 0; k < pattern.numMetatiles(); k++) {
      std::cout << StringConversion::intToString(pattern.metatileID(k),
                                               StringConversion::baseHex)
        << " ";
    }
    std::cout << std::endl;
  } */
  
//    std::cout << std::endl;
}
  
void LaylaLevel::exportToRom(NesRom& rom,
                 int bankNum,
                 Taddress freeSpaceStart,
                 int freeSpaceSize,
                 const LaylaObjectSetCollection& objectSets) const {
  // Get the direct address of the start of the bank
  Taddress bankBaseDirect = 
      UxRomBanking::bankedToDirectAddressMovable(
                                bankNum,
                                UxRomBanking::movableBankOffset);
  
  // Common code block
  std::memcpy(rom.directWrite(bankBaseDirect
                                + commonCodeBlockBaseOffset_),
              commonCodeBlock_.data(),
              commonCodeBlockSize_);
  
  // Palette cycle.
  // This is actually part of the common code block and got hacked into
  // Lonely at the last minute, so it has to be written after the common
  // code block.
  LaylaPalettes::writePatchTable(rom.directWrite(
                                   bankBaseDirect + paletteCycleBaseAddress_),
                                 paletteCyclePatches_);
  LaylaPalettes::writePatchTable(rom.directWrite(
                                   bankBaseDirect
                                     + regularHpCycleBaseAddress_),
                                 regularHpCyclePatches_);
  LaylaPalettes::writePatchTable(rom.directWrite(
                                   bankBaseDirect + lowHpCycleBaseAddress_),
                                 lowHpCyclePatches_);
  LaylaPalettes::writePatchTable(rom.directWrite(
                                   bankBaseDirect
                                     + specialFlashCycleBaseAddress_),
                                 specialFlashCyclePatches_);
  
  // Level init table would go here, but we have to wait until
  // after writing the pattern definitions due to the extension
  // allowing it to spill out of its assigned space.
/*  std::memcpy(rom.directWrite(bankBaseDirect
                                + levelPointerTableBaseStart_),
              levelPointerTable_.data(),
              levelPointerTableSize_); */
  
  // Boss init table
  std::memcpy(rom.directWrite(bankBaseDirect
                                + bossInitTableBaseStart_),
              objectSets[objectSetNum_].bossInitTable().data(),
              objectSets[objectSetNum_].bossInitTable().size());
  
  // Boss vulnerabilities
  bossVulnerabilities_.writeToData(
      rom.directWrite(bankBaseDirect + bossVulnerabilityTableBase_));
  
  // Object vulnerabilities
  objectVulnerabilities_.writeToData(
      rom.directWrite(bankBaseDirect + objectVulnerabilityTableBase_));
  
  // Object score table
  objectScores_.writeToData(
      rom.directWrite(bankBaseDirect + objectScoreTableBase_));
  
  // Object init table
  std::memcpy(rom.directWrite(bankBaseDirect
                                + objectInitTableBaseStart_),
              objectSets[objectSetNum_].objectInitTable().data(),
              objectSets[objectSetNum_].objectInitTable().size());
  
  // Object HPs
  objectHps_.writeToData(
      rom.directWrite(bankBaseDirect + objectHpTableBase_));
  
  // Object patterns
  objectPatterns_.writeToData(
      rom.directWrite(bankBaseDirect + objectPatternTableBaseStart_),
      bankBaseDirect + objectPatternTableBaseStart_,
      objectPatternTableSize_);
  
  // Base metatiles
  baseMetatileSet_.writeToData(
          rom,
          baseNumMetatilesPerSet_,
          bankBaseDirect + baseEvenTileIndexTableBase_,
          bankBaseDirect + baseOddTileIndexTableBase_,
          bankBaseDirect + baseMetatileAttributeTableBase_,
          bankBaseDirect + baseMetatilePropertiesBase_);
  
  // Pattern definitions
  Taddress patternDefinitionTableAddress
    = bankBaseDirect + patternDefinitionTableBase_;
  Taddress freeSpaceBlockStart
    = bankBaseDirect
        + objectCodeBlockBaseOffset_
        + objectCodeBlock_.size();
  int freeSpaceBlockSize =
    UxRomBanking::bankedToDirectAddressMovable(
                                bankNum + 1,
                                UxRomBanking::movableBankOffset)
    - freeSpaceBlockStart;
  patternDefinitions_.writeToData(
          rom,
          patternDefinitionTableAddress,
          bankBaseDirect + objectCodeBlockBaseOffset_
            - patternDefinitionTableAddress,
          freeSpaceBlockStart,
          freeSpaceBlockSize);
  
  // Level init table.
  // Since we expanded the ROM so only one level is in each bank,
  // the same address needs to appear in both slots.
  for (int i = 0; i < numLevelStartPointersEntries; i++) {
    ByteConversion::toBytes(
        UxRomBanking::directToBankedAddressMovable(freeSpaceBlockStart),
        rom.directWrite(
          bankBaseDirect
            + levelPointerTableBaseStart_
            + (i * ByteSizes::uint16Size)),
        ByteSizes::uint16Size,
        EndiannessTypes::little,
        SignednessTypes::nosign);
  }
  
  // Object code block
  std::memcpy(rom.directWrite(bankBaseDirect
                + objectSets[objectSetNum_].objectCodeBlockBaseOffset()),
              objectSets[objectSetNum_].objectCodeBlock().data(),
              objectSets[objectSetNum_].objectCodeBlock().size());
  
//  std::cout << bankBaseDirect + objectCodeBlockBaseOffset_
//    << " " << objectCodeBlock_.size() << std::endl;
  
  // Area data
  areaData_.writeToData(rom,
                        freeSpaceBlockStart,
                        freeSpaceBlockSize);
}
  
int LaylaLevel::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LaylaLevel,
                   0);
  
  // Common code block
//  commonCodeBlock_.save(data);
  
  // Level init table would go here, but we have to wait until
  // after writing the pattern definitions due to the extension
  // allowing it to spill out of its assigned space.
  
  // Boss init table
//  bossInitTable_.save(data);
  
  // Boss vulnerabilities
  bossVulnerabilities_.save(data);
  
  // Object vulnerabilities
  objectVulnerabilities_.save(data);
  
  // Object score table
  objectScores_.save(data);

  // Object init table
//  objectInitTable_.save(data);
  
  // Object HPs
  objectHps_.save(data);
  
  // Object patterns
  objectPatterns_.save(data);
  
  // Base metatiles
  baseMetatileSet_.save(data);
  
  patternDefinitions_.save(data);
  
  levelPointerTable_.save(data);
  
  
  SaveHelper::saveInt(data, objectSetNum_,
                      ByteSizes::uint8Size);
//  SaveHelper::saveInt(data, objectCodeBlockBaseOffset_,
//                      ByteSizes::uint32Size);
//  objectCodeBlock_.save(data);
  
  areaData_.save(data);
  
  LaylaPalettes::savePatchTable(data, paletteCyclePatches_);
  LaylaPalettes::savePatchTable(data, regularHpCyclePatches_);
  LaylaPalettes::savePatchTable(data, lowHpCyclePatches_);
  LaylaPalettes::savePatchTable(data, specialFlashCyclePatches_);
  
  return saver.finalize();
}

int LaylaLevel::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);

  // Common code block
//  byteCount += commonCodeBlock_.load(data + byteCount);
  
  // Level init table would go here, but we have to wait until
  // after writing the pattern definitions due to the extension
  // allowing it to spill out of its assigned space.
  
  // Boss init table
//  byteCount += bossInitTable_.load(data + byteCount);
  
  // Boss vulnerabilities
  byteCount += bossVulnerabilities_.load(data + byteCount);
  
  // Object vulnerabilities
  byteCount += objectVulnerabilities_.load(data + byteCount);
  
  // Object score table
  byteCount += objectScores_.load(data + byteCount);

  // Object init table
//  byteCount += objectInitTable_.load(data + byteCount);

  // Object HPs
  byteCount += objectHps_.load(data + byteCount);
  
  // Object patterns
  byteCount += objectPatterns_.load(data + byteCount);
  
  // Base metatiles
  byteCount += baseMetatileSet_.load(data + byteCount);
  
  byteCount += patternDefinitions_.load(data + byteCount);
  
  byteCount += levelPointerTable_.load(data + byteCount);
  
  objectSetNum_ = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
//  objectCodeBlockBaseOffset_ = LoadHelper::loadInt(data, byteCount,
//                      ByteSizes::uint32Size);
//  byteCount += objectCodeBlock_.load(data + byteCount);
  
  byteCount += areaData_.load(data + byteCount);
  
  byteCount += LaylaPalettes
                  ::loadPatchTable(paletteCyclePatches_, data + byteCount);
  byteCount += LaylaPalettes
                  ::loadPatchTable(regularHpCyclePatches_, data + byteCount);
  byteCount += LaylaPalettes
                  ::loadPatchTable(lowHpCyclePatches_, data + byteCount);
  byteCount += LaylaPalettes
                  ::loadPatchTable(
                      specialFlashCyclePatches_, data + byteCount);
  
  return byteCount;
}
  
void LaylaLevel::changeObjectSet(const LaylaObjectSet& objectSet) {
  objectSetNum_ = objectSet.objectSetNum();
  bossInitTable_ = objectSet.bossInitTable();
  objectInitTable_ = objectSet.objectInitTable();
  objectCodeBlockBaseOffset_ = objectSet.objectCodeBlockBaseOffset();
  objectCodeBlock_ = objectSet.objectCodeBlock();
}
  
LaylaVulnerabilityArray& LaylaLevel::bossVulnerabilities() {
  return bossVulnerabilities_;
}

LaylaVulnerabilityArray& LaylaLevel::objectVulnerabilities() {
  return objectVulnerabilities_;
}

LaylaObjectScoreArray& LaylaLevel::objectScores() {
  return objectScores_;
}

LaylaHpArray& LaylaLevel::objectHps() {
  return objectHps_;
}

LaylaObjectPatternTable& LaylaLevel::objectPatterns() {
  return objectPatterns_;
}

LaylaMetatileSet& LaylaLevel::baseMetatileSet() {
  return baseMetatileSet_;
}

LaylaPatternDefinitionTable& LaylaLevel::patternDefinitions() {
  return patternDefinitions_;
}

int& LaylaLevel::objectSetNum() {
  return objectSetNum_;
}

LaylaArea& LaylaLevel::areaData() {
  return areaData_;
}
  
LaylaLevel::PalettePatchArray& LaylaLevel::paletteCyclePatches() {
  return paletteCyclePatches_;
}

LaylaLevel::PalettePatchArray& LaylaLevel::regularHpCyclePatches() {
  return regularHpCyclePatches_;
}

LaylaLevel::PalettePatchArray& LaylaLevel::lowHpCyclePatches() {
  return lowHpCyclePatches_;
}

LaylaLevel::PalettePatchArray& LaylaLevel::specialFlashCyclePatches() {
  return specialFlashCyclePatches_;
}
  
Tarray<Tbyte>& LaylaLevel::bossInitTable() {
  return bossInitTable_;
}

Tarray<Tbyte>& LaylaLevel::objectInitTable() {
  return objectInitTable_;
}

Taddress& LaylaLevel::objectCodeBlockBaseOffset() {
  return objectCodeBlockBaseOffset_;
}

Tarray<Tbyte>& LaylaLevel::objectCodeBlock() {
  return objectCodeBlock_;
}
  
void LaylaLevel::fixAfterMetatileQuantityChange(
    Tbyte areaType,
    int numMetatiles,
    int index) {
  PatternPreviewAreaTypeCollection patternTypes;
  areaData_.generatePatternAreaTypeGuesses(
      patternTypes,
      patternDefinitions_);
  
  for (int i = 0; i < patternTypes.size(); i++) {
    if (patternTypes[i] == areaType) {
      //
      patternDefinitions_.pattern(i).fixAfterMetatileQuantityChange(
          numMetatiles,
          index);
    }
  }
}


}; 
