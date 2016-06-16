#include "gamedata/LaylaPatternDefinitionTable.h"
#include "util/ByteConversion.h"
#include "nes/UxRomBanking.h"
#include "exception/OutOfRangeIndexException.h"
#include "exception/NotEnoughSpaceException.h"
#include <iostream>

namespace Lonely {


LaylaPatternDefinitionTable::LaylaPatternDefinitionTable() { };
  
LaylaPatternDefinitionTable::LaylaPatternDefinitionTable(
                            const NesRom& src,
                            int numEntries,
                            Taddress startAddress) {
  readFromData(src, numEntries, startAddress);
}

void LaylaPatternDefinitionTable::readFromData(const NesRom& src,
                  int numEntries,
                  Taddress startAddress) {
  patterns_.clear();
                  
  Taddress target = startAddress;
  
//  std::cout << "--start--" << std::endl << std::endl;
  for (int i = 0; i < numEntries; i++) {
    Taddress bankedAddress = ByteConversion::fromBytes(src.directRead(target),
                                                 ByteSizes::uint16Size,
                                                 EndiannessTypes::little,
                                                 SignednessTypes::nosign);
    target += ByteSizes::uint16Size;
    
/*    for (int j = 0; j < numEntries; j++) {
      if (i == j) continue;
    
      Taddress addr = ByteConversion::fromBytes(src.directRead(
                                                  startAddress +
                                                    (j * 2)),
                                                 ByteSizes::uint16Size,
                                                 EndiannessTypes::little,
                                                 SignednessTypes::nosign);
      
      if (addr == bankedAddress) {
        std::cout << "match: " << i << " at " << j << std::endl;
      }
    } */
    
    Taddress directAddress
        = UxRomBanking::bankedToDirectAddressMovable(
                    UxRomBanking::directToBankNumMovable(startAddress),
                    bankedAddress);
    
    patterns_.push_back(LaylaPattern(src.directRead(directAddress)));
    
//    std::cout << directAddress << std::endl;
  }
}
  
void LaylaPatternDefinitionTable::writeToData(NesRom& dst,
                 Taddress mainBlockAddress,
                 int mainBlockLength,
                 Taddress& supplementaryBlockAddress,
                 int& supplementaryBlockLength) const {
//  std::cout << "main: " << mainBlockAddress
//    << " " << mainBlockLength << std::endl;
//  std::cout << "supplement: " << supplementaryBlockAddress
//    << " " << supplementaryBlockLength << std::endl;

  Taddress indexAddress = mainBlockAddress;
  int indexSize = (patterns_.size() * ByteSizes::uint16Size);
  Taddress mainDataAddress = mainBlockAddress + indexSize;
  Taddress mainDataLimit = mainDataAddress
    + (mainBlockLength - indexSize);
  Taddress supplementaryDataLimit
    = supplementaryBlockAddress + supplementaryBlockLength;
    
//  std::cout << mainDataLimit << std::endl;

  for (int i = 0; i < patterns_.size(); i++) {
    Tarray<Tbyte> exportData(patterns_[i].safeCompressedDataAllocationSize());
    int compressedSize = patterns_[i].writeToData(exportData.data());
    
    Taddress dstAddress = 0;
    
    if (patterns_[i].inheritPreviousLayout()) {
      // do nothing and don't throw
    }
    // Try to put pattern in main block
    else if (mainDataAddress + compressedSize <= mainDataLimit) {
      dstAddress = mainDataAddress;
      mainDataAddress += compressedSize;
//      std::cout << "1: " << dstAddress << std::endl;
    }
    // If pattern doesn't fit in main block, put in supplementary block
    // and update position of supplementary block
    else if (supplementaryBlockAddress + compressedSize
                <= supplementaryDataLimit) {
      dstAddress = supplementaryBlockAddress;
      supplementaryBlockAddress += compressedSize;
      supplementaryBlockLength -= compressedSize;
//      std::cout << "2: " << dstAddress << std::endl;
    }
    else {
      throw NotEnoughSpaceException(TALES_SRCANDLINE,
                                    "LaylaPatternDefinitionTable"
                                    "::writeToData()",
                                    compressedSize);
    }
    
//    std::cout << dstAddress << std::endl;

    Taddress indexEntryAddress = UxRomBanking::directToBankedAddressMovable(
                                dstAddress);
    
    if (patterns_[i].inheritPreviousLayout()) {
      // Get previous entry
      indexEntryAddress = ByteConversion::fromBytes(
                              dst.directRead(indexAddress - 2),
                              ByteSizes::uint16Size,
                              EndiannessTypes::little,
                              SignednessTypes::nosign);
    }
    else {
      // Write pattern data
      std::memcpy(dst.directWrite(dstAddress),
                  exportData.data(),
                  compressedSize);
    }
    
    // Add to index
    ByteConversion::toBytes(indexEntryAddress,
                            dst.directWrite(indexAddress),
                            ByteSizes::uint16Size,
                            EndiannessTypes::little,
                            SignednessTypes::nosign);
    indexAddress += ByteSizes::uint16Size;
    
//    std::cout << dstAddress << std::endl;
  }
}
  
int LaylaPatternDefinitionTable::save(Tstring& data) const {
  int byteCount = 0;
  
  byteCount += SaveHelper::saveInt(data, patterns_.size(),
                                   ByteSizes::uint32Size);
  
  for (int i = 0; i < patterns_.size(); i++) {
    byteCount += patterns_[i].save(data);
  }
  
  return byteCount;
}

int LaylaPatternDefinitionTable::load(const Tbyte* data) {
  int byteCount = 0;
  
  patterns_.resize(LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size));
  
  for (int i = 0; i < patterns_.size(); i++) {
    byteCount += patterns_[i].load(data + byteCount);
  }
  
  return byteCount;
}
  
int LaylaPatternDefinitionTable::numPatterns() const {
  return patterns_.size();
}

LaylaPattern& LaylaPatternDefinitionTable::pattern(int index) {
	if (index >= patterns_.size()) {
		throw OutOfRangeIndexException(TALES_SRCANDLINE,
																	 "LaylaPatternDefinitionTable"
																	 "::pattern(int)",
																	 index);
	}
	
	return patterns_[index];
}
  
void LaylaPatternDefinitionTable::insertPattern(int index) {
  LaylaPattern newPattern;
  newPattern.changeWidth(4);

  patterns_.insert(patterns_.begin() + index,
                   newPattern);
}
  
void LaylaPatternDefinitionTable::removePattern(int index) {
  patterns_.erase(patterns_.begin() + index);
}


};
