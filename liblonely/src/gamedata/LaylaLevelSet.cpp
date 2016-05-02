#include "gamedata/LaylaLevelSet.h"
#include "gamedata/LaylaPatternToken.h"
#include "util/StringConversion.h"
#include "structs/AddressTableReader.h"
#include <cstdlib>
#include <iostream>

namespace Lonely {


LaylaLevelSet::LaylaLevelSet()
  : numLevels_(0) { };
              
LaylaLevelSet::LaylaLevelSet(const NesRom& rom,
                             const LaylaOffsetFile& offsets)
  : numLevels_(0) {
  int numAreas = StringConversion::stringToInt(
                    offsets.ini().valueOfKey("AreaProperties", "NumAreas"));
  numLevels_ = numAreas;
  levels_.resize(numAreas);
  
//  int numAreas = numLevels_;
  
  for (int i = 0; i < numAreas; i++) {
    Tstring areaName = Tstring("Area")
        + StringConversion::intToString(i,
                                        StringConversion::baseDec);
    
//    std::cout << areaName << std::endl;
    
    // Basic level variables
    int bankNum = StringConversion::stringToInt(
          offsets.ini().valueOfKey(areaName, "BankNum"));
//    int setNum = (bankConversionBase_ - bankNum);
    int setNum = i / levelsPerBank_;
    if (setNum < 0) {
      setNum = 0;
    }
    else if (setNum >= LaylaObjectSets::numObjectSets) {
      setNum = LaylaObjectSets::numObjectSets - 1;
    }
    Taddress entranceTableAddress = StringConversion::stringToInt(
          offsets.ini().valueOfKey(areaName, "EntranceAddressTableOffset"));
    int numEntranceTableEntries = StringConversion::stringToInt(
          offsets.ini().valueOfKey(areaName, "NumEntrances"));
    int areaDataAddress = StringConversion::stringToInt(
          offsets.ini().valueOfKey(areaName, "AreaDataOffset"));
    int areaDataEndAddress = StringConversion::stringToInt(
          offsets.ini().valueOfKey(areaName, "AreaDataEndOffset"));
    int areaDataSize = areaDataEndAddress - areaDataAddress;
    
    // Object code variables
    Tstring objectCodeKeyName = Tstring("ObjectCodeBlock")
        + StringConversion::intToString(setNum,
                                        StringConversion::baseDec);
    Taddress codeBlockStartOffset = StringConversion::stringToInt(
          offsets.ini().valueOfKey(objectCodeKeyName,
                                   "BlockStartOffset"));
    Taddress codeBlockEndOffset = StringConversion::stringToInt(
          offsets.ini().valueOfKey(objectCodeKeyName,
                                   "BlockEndOffset"));
    
    // Pattern definition variables
    Tstring patternDefinitionKeyName = Tstring("PatternDefinitionTable")
        + StringConversion::intToString(setNum,
                                        StringConversion::baseDec);
//    Taddress patternDefinitionStartOffset = StringConversion::stringToInt(
//          offsets.ini().valueOfKey(patternDefinitionKeyName,
//                                   "TableIndexOffset"));
    int numPatternDefinitionEntries = StringConversion::stringToInt(
          offsets.ini().valueOfKey(patternDefinitionKeyName,
                                   "TableIndexEntries"));
    
    Tstring objectPatternKeyName = Tstring("ObjectPatternSet")
        + StringConversion::intToString(setNum,
                                        StringConversion::baseDec);
    Taddress objectPatternDataStartOffset = StringConversion::stringToInt(
          offsets.ini().valueOfKey(objectPatternKeyName,
                                   "PatternDataStartOffset"));
    Taddress objectPatternDataEndOffset = StringConversion::stringToInt(
          offsets.ini().valueOfKey(objectPatternKeyName,
                                   "PatternDataEndOffset"));
    int numObjectPatternEntries = StringConversion::stringToInt(
          offsets.ini().valueOfKey(objectPatternKeyName,
                                   "NumIndexEntries"));
    int objectPatternDataSize
      = objectPatternDataEndOffset - objectPatternDataStartOffset;
    
    levels_[i].readFromData(rom,
                            bankNum,
                            setNum,
                            codeBlockStartOffset,
                            codeBlockEndOffset,
                            numPatternDefinitionEntries,
                            numObjectPatternEntries,
                            objectPatternDataSize,
                            entranceTableAddress,
                            numEntranceTableEntries,
                            areaDataAddress,
                            areaDataEndAddress);
    
/*    Tarray<Taddress> addresses;
    AddressTableReader::readAddressTable(addresses,
                                         rom.directRead(entranceTableAddress),
                                         numEntranceTableEntries,
                                         ByteSizes::uint16Size,
                                         EndiannessTypes::little);
          
    LaylaArea area(rom.directRead(areaDataAddress),
                   areaDataSize,
                   areaDataAddress,
                   addresses); */
    
//    std::cout << std::endl;
  }
}
  
void LaylaLevelSet::exportToRom(NesRom& rom,
                   const LaylaObjectSetCollection& objectSets) const {
  for (int i = 0; i < numLevels_; i++) {
    levels_[i].exportToRom(rom,
                           i + exportBankOffset_,
                           0,
                           0,
                           objectSets);
  }
}
  
int LaylaLevelSet::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LaylaLevelSet,
                   0);
  
  SaveHelper::saveInt(data, numLevels_, ByteSizes::uint8Size);
  
  for (int i = 0; i < numLevels_; i++) {
    levels_[i].save(data);
  }
  
  return saver.finalize();
}

int LaylaLevelSet::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);
  
  numLevels_ = LoadHelper::loadInt(data, byteCount, ByteSizes::uint8Size);
  
  // Something in in this resize operation causes a crash. For now,
  // the load will only work if the program was started with the correct
  // number of levels (i.e. by specifying them in the offset file).
//  levels_.resize(numLevels_);
  for (int i = 0; i < numLevels_; i++) {
    byteCount += levels_[i].load(data + byteCount);
  }
  
  return byteCount;
//  return loader.fullSize();
}
  
LaylaLevel& LaylaLevelSet::level(int levelNum) {
  return levels_[levelNum];
}

void LaylaLevelSet::fixAfterMetatileQuantityChange(
    Tbyte areaType,
    int numMetatiles,
    int index) {
  if ((numMetatiles == 0)
      || (areaType & SetAreaToken::baseFlag)) {
    return;
  }
/*  else if (areaType & SetAreaToken::bossFlag) {
    
  }
  else {
    
  } */
  
  for (int i = 0; i < numLevels_; i++) {
    levels_[i].fixAfterMetatileQuantityChange(areaType,
                                              numMetatiles,
                                              index);
  }
}
    
int LaylaLevelSet::numLevels() {
  return numLevels_;
}


};
