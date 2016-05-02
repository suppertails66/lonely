#include "gamedata/LaylaArea.h"
#include "util/StringConversion.h"
#include "util/ByteConversion.h"
#include "util/MiscMath.h"
#include "nes/UxRomBanking.h"
#include "structs/AddressTableReader.h"
#include "exception/OutOfRangeIndexException.h"
#include "exception/NotEnoughSpaceException.h"
#include <algorithm>
#include <iostream>

namespace Lonely {


LaylaArea::LaylaArea() { };
  
LaylaArea::LaylaArea(const Tbyte* src,
                    int size,
                    Taddress startAddress,
                    const Tarray<Taddress>& spawnTable) {
  readFromData(src, size, startAddress, spawnTable);
}
  
LaylaArea::~LaylaArea() {
  patternData_.deleteData();
}
  
LaylaArea::LaylaArea(const LaylaArea& a) {
  cloneFrom(a);
}

LaylaArea& LaylaArea::operator=(const LaylaArea& a) {
  cloneFrom(a);
  
  return *this;
}
  
void LaylaArea::readFromData(const Tbyte* src,
                             int size,
                             Taddress startAddress,
                             const Tarray<Taddress>& spawnTable) {
/*  for (int i = 0; i < size; i++) {
    std::cout << StringConversion::intToString(
      (int)(*src++), StringConversion::baseHex) << " ";
  }
  std::cout << std::endl; */
  patternData_.deleteData();
  patternData_.clear();
  spawnTokenPositions_.clear();
  
//  std::cout << "--start--" << std::endl << std::endl;
  
//  for (int i = 0; i < spawnTable.size(); i++) {
//    std::cout << spawnTable[i] << std::endl;
//  }
  
  // Initialize spawn token index to needed size
  spawnTokenPositions_.assign(spawnTable.size(), 0);
  
  int pos = 0;
  while (pos < size) {
    tokenize(src,
             pos,
             patternData_,
             startAddress,
             spawnTable);
  }
  
  for (int i = 0; i < patternData_.size(); i++) {
    // Resolve address-based offsets to absolute token positions
    if (patternData_[i]->type() == LaylaPatternTypes::jumpToPosition) {
      JumpToPositionToken& token = 
        dynamic_cast<JumpToPositionToken&>(*(patternData_[i]));
      
      int distance = token.offset();
      
      int j = i;
      if (distance >= 0) {
        while ((j >= 0) && (distance > 0)) {
          distance -= patternData_[j]->size();
          ++j;
        }
        
        // Offset doesn't include the jump token itself
        --j;
      }
      else {
        while ((j < patternData_.size()) && (distance < 0)) {
          distance += patternData_[j]->size();
          --j;
        }
        
        // Offset doesn't include the jump token itself
        ++j;
      }
      
      token.setOffset(i + (j - i));
      
//      std::cout << "i: " << i << std::endl;
//      std::cout << "j: " << j << std::endl;
//      std::cout << "offset: " << token.offset() << std::endl;
    }
    // Index spawn tokens
    else if (patternData_[i]->type() == LaylaPatternTypes::spawnPoint) {
      SpawnPointToken& token = 
        dynamic_cast<SpawnPointToken&>(*(patternData_[i]));
      
      spawnTokenPositions_[token.spawnNum()] = i;
    }
  }
  
/*  for (int i = 0; i < patternData_.size(); i++) {
    std::cout << i << ": ";
    switch (patternData_[i]->type()) {
    case LaylaPatternTypes::spawnPoint:
      std::cout << "-----SPAWN POINT "
        <<  StringConversion::intToString(
              dynamic_cast<SpawnPointToken&>(
                  (*patternData_[i])).spawnNum(),
                  StringConversion::baseHex) << "-----";
      std::cout << std::endl;
      break;
    case LaylaPatternTypes::standardPattern:
      std::cout << "Standard pattern: ";
      std::cout << StringConversion::intToString(
        dynamic_cast<StandardPatternToken&>(
            (*patternData_[i])).patternNum(),
            StringConversion::baseHex) << std::endl;
      break;
    case LaylaPatternTypes::spawnBoss:
      {
      SpawnBossToken& token = 
        dynamic_cast<SpawnBossToken&>(*(patternData_[i]));
      std::cout << "Spawn boss: ";
      std::cout <<
          StringConversion::intToString(
            token.bossNum(),
            StringConversion::baseHex)
        << std::endl;
      }
      break;
    case LaylaPatternTypes::setScrolling:
      {
      SetScrollingToken& token = 
        dynamic_cast<SetScrollingToken&>(*(patternData_[i]));
      std::cout << "Set scrolling: ";
      std::cout
        << StringConversion::intToString(
            token.killsToNextFood(),
            StringConversion::baseHex)
        << " "
        << StringConversion::intToString(
            token.scrollingDisabled(),
            StringConversion::baseHex)
        << std::endl;
      }
      break;
    case LaylaPatternTypes::setArea:
      {
      SetAreaToken& token = 
        dynamic_cast<SetAreaToken&>(*(patternData_[i]));
      std::cout << "Set area: ";
      std::cout
        << StringConversion::intToString(
            token.backgroundMetatile(),
            StringConversion::baseHex)
        << " "
        << StringConversion::intToString(
            token.areaType(),
            StringConversion::baseHex)
        << std::endl;
      }
      break;
    case LaylaPatternTypes::setMap:
      {
      SetMapToken& token = 
        dynamic_cast<SetMapToken&>(*(patternData_[i]));
      std::cout << "Set map: ";
      std::cout
        << StringConversion::intToString(
            token.mapNum(),
            StringConversion::baseHex)
        << " "
        << StringConversion::intToString(
            token.param2(),
            StringConversion::baseHex)
        << std::endl;
      }
      break;
    case LaylaPatternTypes::setElevators:
      {
      SetElevatorsToken& token = 
        dynamic_cast<SetElevatorsToken&>(*(patternData_[i]));
      std::cout << "Set elevators: ";
      std::cout
        << StringConversion::intToString(
            token.upDestination(),
            StringConversion::baseHex)
        << " "
        << StringConversion::intToString(
            token.downDestination(),
            StringConversion::baseHex)
        << std::endl;
      }
      break;
    case LaylaPatternTypes::jumpToPosition:
      {
      JumpToPositionToken& token = 
        dynamic_cast<JumpToPositionToken&>(*(patternData_[i]));
      std::cout << "Jump to position: ";
      std::cout
        << StringConversion::intToString(
            token.offset(),
            StringConversion::baseDec)
        << std::endl;
      }
      break;
    default:
      std::cout << "unknown type" << std::endl;
      break;
    }
  } */
}
  
void LaylaArea::writeToData(NesRom& dst,
                 Taddress mainBlockAddress,
                 int mainBlockLength) const {
  int addressTableSize
      = (spawnTokenPositions_.size()) * ByteSizes::uint16Size;
  int totalExportSize
    = addressTableSize + totalSize();
    
  if (totalExportSize > mainBlockLength) {
    throw NotEnoughSpaceException(TALES_SRCANDLINE,
                                  "LaylaArea::writeToData()",
                                  totalExportSize);
  }
  
  Taddress indexAddress
    = mainBlockAddress;
  Taddress dataAddress
    = mainBlockAddress + addressTableSize;
    
//  std::cout << indexAddress << " " << dataAddress << std::endl;
    
  Tarray<Taddress> resolvedAddresses(spawnTokenPositions_.size());
  
  for (int i = 0; i < patternData_.size(); i++) {
    // Resolve addresses for index
    for (int j = 0; j < spawnTokenPositions_.size(); j++) {
      if (spawnTokenPositions_[j] == i) {
        resolvedAddresses[j]
          = UxRomBanking::directToBankedAddressMovable(dataAddress);
      }
    }
    
    // Special-case pattern jump tokens: we have to resolve them
    // to addresses
    if (patternData_[i]->type() == LaylaPatternTypes::jumpToPosition) {
      const JumpToPositionToken& token
        = *(dynamic_cast<const JumpToPositionToken*>(patternData_[i]));
      
      int pos = i + 1;
      int total = 0;
      int direction = 1;
      if (token.offset() <= i) {
        direction = -1;
        // if jump is to a previous position, account for the size
        // of the token itself in the jump
        pos = i;
//        total = -(token.size());
      }
      
      while (pos != token.offset()) {
        total += (patternData_[pos]->size() * direction);
        pos += direction;
      }
      
      // destination contributes to total if jumping to a lower position
      if (direction < 0) {
        total += (patternData_[pos]->size() * direction);
      }
      
      Taddress targetAddress
        = UxRomBanking::directToBankedAddressMovable(dataAddress + total);
      
      // account for the fact that we haven't yet changed dataAddress
      // to reflect the position from which the game will calculate the
      // jump (i.e. after reading the token)
      targetAddress += token.size();
      
      dataAddress += token.writeToData(dst.directWrite(dataAddress),
                        targetAddress);
    }
    else {
      dataAddress += patternData_[i]->writeToData(
          dst.directWrite(dataAddress));
    }
  }
  
  // Write address table
  AddressTableReader::writeAddressTable(dst.directWrite(indexAddress),
                                        resolvedAddresses);
}
  
int LaylaArea::save(Tstring& data) const {
  int byteCount = 0;
  
  byteCount += SaveHelper::saveInt(data, patternData_.size(),
                                   ByteSizes::uint32Size);
  
  for (int i = 0; i < patternData_.size(); i++) {
    byteCount += patternData_[i]->save(data);
  }
  
  byteCount += SaveHelper::saveInt(data, spawnTokenPositions_.size(),
                                   ByteSizes::uint32Size);
  
  for (int i = 0; i < spawnTokenPositions_.size(); i++) {
    byteCount += SaveHelper::saveInt(data, spawnTokenPositions_[i],
                                   ByteSizes::uint16Size);
  }
  
  return byteCount;
}

int LaylaArea::load(const Tbyte* data) {
  int byteCount = 0;
  
  patternData_.deleteData();
  patternData_.clear();
  spawnTokenPositions_.clear();
  
  patternData_.resize(LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size));
  
  for (int i = 0; i < patternData_.size(); i++) {
//    std::cout << i << ": ";
    LaylaPatternToken* token
      = LaylaPatternToken::loadAndAllocateToken(
          data + byteCount);
    patternData_.set(i, token);
    byteCount += token->serializedSize();
  }
  
  spawnTokenPositions_.resize(LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size));
  
  for (int i = 0; i < spawnTokenPositions_.size(); i++) {
    spawnTokenPositions_[i] = LoadHelper::loadInt(data, byteCount,
                              ByteSizes::uint16Size);
  }
  
  return byteCount;
}
  
void LaylaArea::tokenize(const Tbyte* src,
              int& pos,
              LaylaPatternTokenVector& dst,
              Taddress startAddress,
              const Tarray<Taddress>& spawnTable) {
  // If there are one or more spawn points at this address,
  // insert spawn point token(s) as needed
  Taddress bankedReadAddress = UxRomBanking::directToBankedAddressMovable(
                            startAddress + pos);
//  std::cout << bankedReadAddress << std::endl;
  for (int i = 0; i < spawnTable.size(); i++) {
    if (spawnTable[i] == bankedReadAddress) {
//      std::cout << "match: " << i << std::endl;

      dst.push_back(new SpawnPointToken(i));
    }
  }
  
  Tbyte value = src[pos++];
  
  switch (value) {
  case opSpawnBoss_:
    {
    Tbyte bossNum = src[pos++];
    dst.push_back(new SpawnBossToken(bossNum));
    }
    break;
  case opSetScrolling_:
    {
    Tbyte killsToNextFood = src[pos++];
    Tbyte disableScrolling = src[pos++];
    dst.push_back(new SetScrollingToken(killsToNextFood,
                                        disableScrolling));
    }
    break;
  case opSetArea_:
    {
    Tbyte backgroundMetatile = src[pos++];
    Tbyte areaType = src[pos++];
    dst.push_back(new SetAreaToken(backgroundMetatile,
                                   areaType));
    }
    break;
  case opSetMap_:
    {
    Tbyte mapNum = src[pos++];
    Tbyte param2 = src[pos++];
    dst.push_back(new SetMapToken(mapNum,
                                  param2));
    }
    break;
  case opSetElevators_:
    {
    Tbyte upDestination = src[pos++];
    Tbyte downDestination = src[pos++];
    dst.push_back(new SetElevatorsToken(upDestination,
                                        downDestination));
    }
    break;
  case opJumpToPosition_:
    {
    Taddress address = ByteConversion::fromBytes(
                          src + pos,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
    pos += ByteSizes::uint16Size;
    
    Taddress bankedStartAddress
        = UxRomBanking::directToBankedAddressMovable(startAddress);
    
    int offset = (int)address
      - (int)(bankedStartAddress + pos);
    
    // HACK: Correct mistake in jump definition in Asteroid 3.
    // The third byte of the command (FF 2F B9) was omitted, so
    // it instead mixes with the next command (FD 03 00).
    if (offset == invalidJumpComputedOffset) {
      // Substitute the intended jump value
      offset = invalidJumpReplacementValue;
      dst.push_back(new JumpToPositionToken(offset));
      
      // Re-align pos with the data stream
      --pos;
    }
    else {
      dst.push_back(new JumpToPositionToken(offset));
    }
    
    }
    break;
  default:
    // Standard pattern
    dst.push_back(new StandardPatternToken(value));
    break;
  }
}
                    
int LaylaArea::numPatternTokens() const {
  return patternData_.size();
}

LaylaPatternToken& LaylaArea::patternToken(int index) {
  if (index >= patternData_.size()) {
    throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                   "LaylaArea::patternToken(int)",
                                   index);
  }
  
  return *(patternData_[index]);
}
  
void LaylaArea::cloneFrom(const LaylaArea& src) {
  // Clone each token
  // (Next time, use a virtual function in LaylaPatternToken for this, please)
  patternData_.deleteData();
  patternData_.clear();
  for (int i = 0; i < src.patternData_.size(); i++) {
    switch (src.patternData_[i]->type()) {
    case LaylaPatternTypes::spawnPoint:
      {
      const SpawnPointToken& token
        = *(dynamic_cast<const SpawnPointToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new SpawnPointToken(token));
      }
      break;
    case LaylaPatternTypes::standardPattern:
      {
      const StandardPatternToken& token
        = *(dynamic_cast<const StandardPatternToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new StandardPatternToken(token));
      }
      break;
    case LaylaPatternTypes::spawnBoss:
      {
      const SpawnBossToken& token
        = *(dynamic_cast<const SpawnBossToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new SpawnBossToken(token));
      }
      break;
    case LaylaPatternTypes::setScrolling:
      {
      const SetScrollingToken& token
        = *(dynamic_cast<const SetScrollingToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new SetScrollingToken(token));
      }
      break;
    case LaylaPatternTypes::setArea:
      {
      const SetAreaToken& token
        = *(dynamic_cast<const SetAreaToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new SetAreaToken(token));
      }
      break;
    case LaylaPatternTypes::setMap:
      {
      const SetMapToken& token
        = *(dynamic_cast<const SetMapToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new SetMapToken(token));
      }
      break;
    case LaylaPatternTypes::setElevators:
      {
      const SetElevatorsToken& token
        = *(dynamic_cast<const SetElevatorsToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new SetElevatorsToken(token));
      }
      break;
    case LaylaPatternTypes::jumpToPosition:
      {
      const JumpToPositionToken& token
        = *(dynamic_cast<const JumpToPositionToken*>
            (src.patternData_[i]));
      patternData_.push_back(
        new JumpToPositionToken(token));
      }
      break;
    default:
      break;
    }
  }

  // Copy spawn position table
  spawnTokenPositions_ = src.spawnTokenPositions_;
}

const LaylaPatternToken& LaylaArea::patternToken(int index) const {
  if (index >= patternData_.size()) {
    throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                   "LaylaArea::patternToken(int)"
                                   " const",
                                   index);
  }
  
  return *(patternData_[index]);
}
	
int LaylaArea::totalSize() const {
	// Total all token sizes
	int total = 0;
	
	for (int i = 0; i < patternData_.size(); i++) {
	  total += patternData_[i]->size();
	}
	
	return total;
}
	
int LaylaArea::findRealXOfIndex(int index,
                   LaylaPatternDefinitionTable& patterns) {
  int total = 0;
  
  if (index > patternData_.size()) {
    index = patternData_.size();
  }
  
  for (int i = 0; i < index; i++) {
    if (patternData_[i]->type() == LaylaPatternTypes::standardPattern) {
      total += patterns.pattern(
        (dynamic_cast<const StandardPatternToken*>
                  (patternData_[i]))->patternNum())
        .realWidth();
    }
  }
  
  return total;
}
	
void LaylaArea::insertStandardPatternToken(int position,
                 LaylaPatternToken* token) {
  // Fix any tokens that would be invalidated by the insert
/*  for (int i = 0; i < patternData_.size(); i++) {
    switch (patternData_[i]->type()) {
    case LaylaPatternTypes::jumpToPosition:
      {
      // All jumps whose destination is to the right of the insert
      // must have their target position increased by 1
      JumpToPositionToken& token
        = *(dynamic_cast<JumpToPositionToken*>
            (patternData_[i]));
      
      if (token.offset() >= position) {
        token.setOffset(token.offset() + 1);
      }
      
      
      }
      break;
    default:
      break;
    }
  }
  
  // Update positions of spawn tokens
  for (int i = 0; i < spawnTokenPositions_.size(); i++) {
    if (spawnTokenPositions_[i] >= position) {
      (spawnTokenPositions_[i])++;
    }
  }
  
  // Insert the token
  patternData_.insert(position,
                      token); */
  LaylaPatternTokenVector tokens;
  tokens.push_back(token);
  insertStandardPatternTokens(position, tokens);
}
	
void LaylaArea::insertStandardPatternTokens(int position,
                 LaylaPatternTokenVector tokens) {
  // Fix any tokens that would be invalidated by the insert
  for (int i = 0; i < patternData_.size(); i++) {
    switch (patternData_[i]->type()) {
    case LaylaPatternTypes::jumpToPosition:
      {
      // All jumps whose destination is to the right of the insert
      // must have their target position increased by 1
      JumpToPositionToken& token
        = *(dynamic_cast<JumpToPositionToken*>
            (patternData_[i]));
      
      if (token.offset() >= position) {
        token.setOffset(token.offset() + tokens.size());
      }
      
      
      }
      break;
    default:
      break;
    }
  }
  
  // Update positions of spawn tokens
  for (int i = 0; i < spawnTokenPositions_.size(); i++) {
    if (spawnTokenPositions_[i] >= position) {
      spawnTokenPositions_[i] += tokens.size();
    }
  }
  
  // Insert the tokens
  patternData_.insert(position,
                      tokens.size(),
                      NULL);
  
  for (int i = 0; i < tokens.size(); i++) {
    patternData_.set(position + i, tokens[i]);
  }
}

int LaylaArea::findPreviousStandardPatternIndexNext(int startIndex) {
  int i = startIndex;
  // To draw the first node, we need to iterate backwards through
  // the tokens until we find the previous standard pattern
  while (true) {
    --i;
    
    if (i <= 0) {
      i = 0;
      break;
    }
    
    if (patternToken(i).type()
               == LaylaPatternTypes::standardPattern) {
      ++i;
      break;
    }
  }
  
  return i;
}
  
int LaylaArea::findPreviousStandardPatternIndex(int startIndex) {
  int i = startIndex;
  // To draw the first node, we need to iterate backwards through
  // the tokens until we find the previous standard pattern
  while (true) {
    --i;
    
    if (i <= 0) {
      i = 0;
      break;
    }
    
    if (patternToken(i).type()
               == LaylaPatternTypes::standardPattern) {
      break;
    }
  }
  
  return i;
}
  
int LaylaArea::findNextStandardPatternIndex(int startIndex) {
  int i = startIndex;
  // To draw the first node, we need to iterate backwards through
  // the tokens until we find the previous standard pattern
  while (true) {
    ++i;
    
    if (i >= numPatternTokens()) {
      i = numPatternTokens();
      break;
    }
    
    if (patternToken(i).type()
               == LaylaPatternTypes::standardPattern) {
      break;
    }
  }
  
  return i;
}
  
int LaylaArea::findNearbyStandardPatternIndex(int index) {
  int i = index;
  while (i >= 0) {
    if (patternToken(i).type()
               == LaylaPatternTypes::standardPattern) {
      return i;
    }
    
    --i;
  }
  
  i = index;
  while (i < numPatternTokens()) {
    if (patternToken(i).type()
               == LaylaPatternTypes::standardPattern) {
      return i;
    }
    
    ++i;
  }
  
  return numPatternTokens();
}
  
int LaylaArea::findStartingPattern() {
  return findNextStandardPatternIndex(0);
}

void LaylaArea::deletePatternsAndNodesAt(int position,
                              int numPatterns) {
//  int totalTokensDeleted = 0;
//  int startPos = findPreviousStandardPatternIndexNext(position);
//  int pos = startPos;
  
  // Find the index of the end of the data (we have to compute this ahead
  // of time in order to update the spawn index)
  int endPos = position;
  for (int i = 0; i < numPatterns; i++) {
    endPos = findNextStandardPatternIndex(endPos);
  }
  
  deletePatternRange(position, endPos);
  
  // If there are any spawn tokens in the node data,
  // delete their entries in the index
/*  for (int spawnCheckPos = pos; spawnCheckPos < endPos; spawnCheckPos++) {
    if (patternToken(spawnCheckPos).type()
          == LaylaPatternTypes::spawnPoint) {
      int j = 0;
      while (j < spawnTokenPositions_.size()) {
        if (spawnTokenPositions_[j] == spawnCheckPos) {
          // Delete reference
          spawnTokenPositions_.erase(
            spawnTokenPositions_.begin() + j);
            
          // Don't increment j here because it now gives the next
          // index we need to check
        }
        else {
          ++j;
        }
      }
    }
  } */
  
  // Deal with silly edge case that should be handled more gracefully
//  if (endPos == patternData_.size()) {
//    ++endPos;
//  }
  
  // Delete the pattern data
//  patternData_.erase(startPos, endPos);
  
  // Fix anything else that would be broken by the deletion
//  int deleteSize = endPos - startPos;
//  fixTokensAfterErase(startPos, deleteSize);
  
  // Delete each node/pattern pair
/*  for (int i = 0; i < numPatterns; i++) {
    // Delete node
    while (pos < patternData_.size()) {
      // Stop when we reach the pattern data
      if (patternToken(pos).type() == LaylaPatternTypes::standardPattern) {
        break;
      }
      
      
    }
    
    // Delete pattern
  } */
}
  
void LaylaArea::deletePatternRange(int startIndex,
                                   int endIndex) {
  int startPos = findPreviousStandardPatternIndexNext(startIndex);
  int endPos = endIndex;
  
  // Delete the pattern data
  patternData_.erase(startPos, endPos);
  
  // Fix anything else that would be broken by the deletion
  int deleteSize = endPos - startPos;
  fixTokensAfterErase(startPos, deleteSize);
}
  
int LaylaArea
		::numPatternTokensInCollection(LaylaPatternTokenVector& patterns) {
  int total = 0;
  for (int i = 0; i < patterns.size(); i++) {
    if (patterns[i]->type() == LaylaPatternTypes::standardPattern) {
      ++total;
    }
  }
  
  return total;
}
  
void LaylaArea::insertPatternsAndNodesAt(int index,
                 LaylaPatternTokenVector& patterns) {
  int startPos = findPreviousStandardPatternIndexNext(index);
  patternData_.insert(startPos, patterns.size(), NULL);
//  std::cout << index << " " << startPos
//      << " " << patterns.size() << std::endl;
  for (int i = 0; i < patterns.size(); i++) {
    patternData_.set(startPos + i, patterns[i]->cloneNew());
  }
  fixTokensAfterInsert(startPos, patterns.size());
}
  
void LaylaArea::insertPatternsAt(int index,
                 LaylaPatternTokenVector& patterns) {
  LaylaPatternTokenVector insertVector;
  
  // Filter out nodes
  filterStandardPatternTokens(insertVector,
                              patterns);
  
  insertPatternsAndNodesAt(index, insertVector);
}
  
void LaylaArea::insertPatternsAndNodesAtPure(int index,
                 int originalPos,
                 LaylaPatternTokenVector& patterns) {
  int startPos = findPreviousStandardPatternIndexNext(index);
  patternData_.insert(startPos, patterns.size(), NULL);
//  std::cout << index << " " << startPos
//      << " " << patterns.size() << std::endl;
  for (int i = 0; i < patterns.size(); i++) {
    patternData_.set(startPos + i, patterns[i]->cloneNew());
  }
  fixTokensAfterInsertPure(startPos, patterns.size(), originalPos);
}

void LaylaArea::overwritePatternsAndNodesAt(int index,
                 LaylaPatternTokenVector& patterns) {
  // Get count of standard patterns to be added
  int numOverwritePatterns = numPatternTokensInCollection(patterns);
  
  // Delete that number of patterns and their nodes at the overwrite
  // position
  deletePatternsAndNodesAt(index,
                           numOverwritePatterns);
  
  // Insert the data at the overwrite position
  insertPatternsAndNodesAt(index,
                           patterns);
}
  
void LaylaArea::overwritePatternsAt(int index,
                 LaylaPatternTokenVector& patterns) {
/*  LaylaPatternTokenVector overwriteVector;
  
  // Filter out nodes
  filterStandardPatternTokens(overwriteVector,
                              patterns);
  
  overwritePatternsAndNodesAt(index, overwriteVector); */
  
  int srcPos = 0;
  int dstPos = index;
  while ((dstPos < patternData_.size())
         && (srcPos < patterns.size())) {
    // If index is invalid, try to fix it
    if (patternToken(dstPos).type() != LaylaPatternTypes::standardPattern) {
      dstPos = findNextStandardPatternIndex(dstPos);
      // If we can't find a valid position, give up
      if (patternToken(dstPos).type()
            != LaylaPatternTypes::standardPattern) {
        break;
      }
    }
    
    if (patterns[srcPos]->type() == LaylaPatternTypes::standardPattern) {
      
      StandardPatternToken& standardPatternToken
        = *(dynamic_cast<StandardPatternToken*>(patterns[srcPos]));
      
      dynamic_cast<StandardPatternToken&>(patternToken(dstPos))
        .setPatternNum(standardPatternToken.patternNum());
      dstPos = findNextStandardPatternIndex(dstPos);
    }
    
    ++srcPos;
  }
}
  
void LaylaArea::deletePattern(int index) {
  patternData_.deleteElement(index);
  patternData_.erase(index, index + 1);
  fixTokensAfterErase(index, 1);
}
  
int LaylaArea::correctEraseSpawnNum(int originalNum,
                         int originalPos,
                         int numSpawnsRemoved,
                         int erasePos,
                         int eraseEnd) {
  // To right of deleted area: decrease number
  if (originalPos >= eraseEnd) {
    return (originalNum - numSpawnsRemoved);
  }
  // Within deleted area: delete
  else if (isInRange(originalPos, erasePos, eraseEnd)) {
    // try to shift left
    while (true) {
      if (originalNum <= 0) {
        break;
      }
      
      --originalNum;
      
      if (spawnTokenPositions_[originalNum] < erasePos) {
        break;
      }
    }
    
    return originalNum;
//    return -1;
  }
  // Left of deleted area: no change
  else {
    return originalNum;
  }
}
  
int LaylaArea::correctInsertSpawnNum(int originalNum,
                         int originalPos,
                         int numSpawnsAdded,
                         int insertPos,
                         int insertEnd) {
  // To right of inserted area: increase number
  if (originalPos >= insertPos) {
    return (originalNum + numSpawnsAdded);
  }
  // Left of inserted area: no change
  else {
    return originalNum;
  }
}
  
bool LaylaArea::isInRange(int val, int lower, int upper) {
  if ((val >= lower) && (val < upper)) {
    return true;
  }
  else {
    return false;
  }
}
                                   
void LaylaArea::reorderSpawns() {
  // Sort spawns by positions
  std::sort(spawnTokenPositions_.begin(),
            spawnTokenPositions_.end());
  
  // Renumber tokens according to sorted position
  for (int j = 0; j < spawnTokenPositions_.size(); j++) {
//    std::cout << j << std::endl;
    SpawnPointToken& token
      = dynamic_cast<SpawnPointToken&>(patternToken(
          spawnTokenPositions_[j]));
      
    token.setSpawnNum(j);
  }
}
                
int LaylaArea::countSpawns(int start, int end) {
  int numSpawns = 0;
  for (int i = start; i < end; i++) {
    if (patternToken(i).type() == LaylaPatternTypes::spawnPoint) {
      ++numSpawns;
    }
  }
  return numSpawns;
}
                
int LaylaArea::countSpawnsRemoved(int erasePos, int eraseEnd) {
  int numSpawnsRemoved = 0;
  for (int i = 0; i < spawnTokenPositions_.size(); i++) {
    if ((spawnTokenPositions_[i] >= erasePos)
        && (spawnTokenPositions_[i] < eraseEnd)) {
      ++numSpawnsRemoved;
    }
  }
  return numSpawnsRemoved;
}
              
int LaylaArea::countSpawnsAdded(int insertPos, int insertEnd) {
  int numSpawnsAdded = 0;
  for (int i = insertPos; i < insertEnd; i++) {
    if (patternToken(i).type() == LaylaPatternTypes::spawnPoint) {
      ++numSpawnsAdded;
    }
  }
  return numSpawnsAdded;
}
  
void LaylaArea::fixTokensAfterErase(int erasePos,
                         int eraseSize) {
  int eraseEnd = erasePos + eraseSize;
//  std::cout << erasePos << " " << eraseSize << " " << eraseEnd << std::endl;
  
  // Compute number of spawns removed (for correction below)
  int numSpawnsRemoved = countSpawnsRemoved(erasePos, eraseEnd);

  fixReferencesAfterEraseRaw(erasePos, eraseSize, numSpawnsRemoved);
  
  updateSpawnIndexAfterErase(erasePos, eraseEnd);
  
/*  for (int i = 0;
       i < spawnTokenPositions_.size();
       i++) {
    std::cout << i << " " << spawnTokenPositions_[i] << std::endl;
  } */
}

void LaylaArea::fixTokensAfterInsert(int insertPos,
                          int insertSize) {
  int insertEnd = insertPos + insertSize;
  
//  std::cout << "insert size: " << insertSize << std::endl;
  
/*  for (int i = 0;
       i < spawnTokenPositions_.size();
       i++) {
    std::cout << i << " " << spawnTokenPositions_[i] << std::endl;
  } */
  
//  std::cout << insertPos << " " << insertSize << " "
//    << insertEnd << std::endl;
  
  // Count number of spawns added
  int numSpawnsAdded = countSpawnsAdded(insertPos, insertEnd);
  
  fixReferencesAfterInsertRaw(insertPos, insertSize, numSpawnsAdded);
  
  updateSpawnIndexAfterInsert(insertPos, insertEnd);
  
  // Sort spawns
  reorderSpawns();
}
  
void LaylaArea::fixTokensAfterInsertPure(int insertPos,
                         int insertSize,
                         int origPos) {
  int insertEnd = insertPos + insertSize;
  
  // Count number of spawns added
  int numSpawnsAdded = countSpawnsAdded(insertPos, insertEnd);
  
  fixReferencesAfterInsertPure(insertPos, insertSize, origPos, numSpawnsAdded);
  
  updateSpawnIndexAfterInsert(insertPos, insertEnd);
  
  // Sort spawns
  reorderSpawns();
}
  
void LaylaArea::fixTokensAfterMoveErase(int erasePos,
                         int eraseSize) {
  int eraseEnd = erasePos + eraseSize;
  
  // Compute number of spawns removed (for correction below)
  int numSpawnsRemoved = countSpawnsRemoved(erasePos, eraseEnd);
  
  updateSpawnIndexAfterErase(erasePos, eraseEnd);
}

void LaylaArea::fixTokensAfterMoveInsert(int insertPos,
                         int insertSize) {
  int insertEnd = insertPos + insertSize;
  
  // Count number of spawns added
  int numSpawnsAdded = countSpawnsAdded(insertPos, insertEnd);
  
  updateSpawnIndexAfterInsert(insertPos, insertEnd);
  
  // Sort spawns
  reorderSpawns();
}
  
void LaylaArea::updateSpawnIndexAfterErase(int erasePos, int eraseEnd) {
  int eraseSize = eraseEnd - erasePos;
  // Correct spawn index
  for (int i = 0;
       i < spawnTokenPositions_.size();
       /* NO INCREMENT */
       ) {
    // Delete any entries that point to the deleted area
    if ((spawnTokenPositions_[i] >= erasePos)
        && (spawnTokenPositions_[i] < eraseEnd)) {
      spawnTokenPositions_.erase(spawnTokenPositions_.begin() + i);
//      ++numSpawnsRemoved;
      // Don't increment -- i already indexes to next element
    }
    // Shift entries to the right of the deleted area to the left
    // by the amount of data deleted
    else if (spawnTokenPositions_[i] >= eraseEnd) {
//      std::cout << "before: " << spawnTokenPositions_[i] << std::endl;
      spawnTokenPositions_[i] -= eraseSize;
//      std::cout <<  "after: " << spawnTokenPositions_[i] << std::endl;
      ++i;
    }
    else {
      ++i;
    }
  }
}

void LaylaArea::updateSpawnIndexAfterInsert(int insertPos, int insertEnd) {
  int insertSize = insertEnd - insertPos;
  // Correct spawn index
  for (int i = 0; i < spawnTokenPositions_.size(); i++) {
    // Shift entries to the right of the insertion point to the right
    // by the amount of data inserted
    if ((spawnTokenPositions_[i] >= insertPos)) {
      spawnTokenPositions_[i] += insertSize;
    }
  }
  
  // Add inserted spawns to index
  for (int i = insertPos; i < insertEnd; i++) {
    if (patternToken(i).type() == LaylaPatternTypes::spawnPoint) {
      spawnTokenPositions_.push_back(i);
    }
  }
}

void LaylaArea::fixReferencesAfterEraseRaw(int erasePos,
                         int eraseSize,
                         int numSpawnsRemoved) {
  int eraseEnd = erasePos + eraseSize;
  // Correct pattern data
  for (int i = 0;
       i < patternData_.size();
       i++
       ) {
     fixReferenceTokenAfterErase(i, erasePos, eraseSize, numSpawnsRemoved);
  }
}
  
void LaylaArea::fixReferencesAfterInsertRaw(int insertPos,
                         int insertSize,
                         int numSpawnsAdded) {
  int insertEnd = insertPos + insertSize;
  
  // Correct pattern data
  for (int i = 0;
       i < patternData_.size();
       i++
       ) {
    fixReferenceTokenAfterInsert(i, insertPos, insertSize, numSpawnsAdded);
  }
}
  
void LaylaArea::fixReferencesAfterInsertPure(int insertPos,
                         int insertSize,
                         int origPos,
                         int numSpawnsAdded) {
  int insertEnd = insertPos + insertSize;
  
//  int passStart = origPos;
//  int passEnd = insertPos;
//  int numSpawnsPassed = countSpawns(
  
  // Correct pattern data
  for (int i = 0;
       i < patternData_.size();
       i++
       ) {
    // Don't correct tokens that were copied in -- we'll do them separately
    if (isInRange(i, insertPos, insertPos + insertSize)) {
      continue;
    }
    
    fixReferenceTokenAfterInsert(i, insertPos, insertSize, numSpawnsAdded);
  }
  
  // Count spawn points to the left of the insert position
  int numSpawnsToLeft = countSpawns(0, insertPos);
  
  // Correct copied-in tokens
  for (int i = insertPos; i < (insertPos + insertSize); i++) {
    switch (patternToken(i).type()) {
    case LaylaPatternTypes::spawnPoint:
      {
      SpawnPointToken& token
        = dynamic_cast<SpawnPointToken&>(patternToken(i));
      
      ++numSpawnsToLeft;
      }
      break;
    case LaylaPatternTypes::setMap:
      {
      SetMapToken& token
        = dynamic_cast<SetMapToken&>(patternToken(i));
      
      // Assume set map tokens are pointing to the nearest spawn to the left
      // (they usually are, and it's a bunch of trouble to try to fix them
      // for the few cases where they aren't)
      token.setMapNum((numSpawnsToLeft - 1));
      ++numSpawnsToLeft;
      }
      break;
    case LaylaPatternTypes::jumpToPosition:
      {
      JumpToPositionToken& token
        = dynamic_cast<JumpToPositionToken&>(patternToken(i));
      
      token.setOffset(token.offset()
        - origPos + insertPos);
      }
      break;
    default:
      break;
    }
  }
}
                 
void LaylaArea::fixReferenceTokenAfterErase(int i,
                                  int erasePos,
                                  int eraseSize,
                                  int numSpawnsRemoved) {
  switch (patternToken(i).type()) {
  case LaylaPatternTypes::spawnPoint:
    {
    SpawnPointToken& token
      = dynamic_cast<SpawnPointToken&>(patternToken(i));
    
    // Tokens to the right of the deleted area must have their spawn number
    // decremented to correspond to the removed tokens
    if (i >= erasePos) {
      token.setSpawnNum(token.spawnNum() - numSpawnsRemoved);
    }
    
    ++i;
    }
    break;
  case LaylaPatternTypes::setMap:
    {
    SetMapToken& token
      = dynamic_cast<SetMapToken&>(patternToken(i));
    int originalPos = spawnTokenPositions_[token.mapNum()];
    
    token.setMapNum(correctEraseSpawnNum(token.mapNum(),
                         originalPos,
                         numSpawnsRemoved,
                         erasePos,
                         erasePos + eraseSize));
    ++i;
    
//      if (token.mapNum() == 0xFF) {
//        patternData_.deleteElement(i);
//        patternData_.erase(i, i + 1);
//      }
    
    // To right of deleted area: decrease number
/*      if (originalPos >= eraseEnd) {
      token.setMapNum(token.mapNum() - numSpawnsRemoved);
      ++i;
    }
    // Within deleted area: delete
    else if (isInRange(originalPos, erasePos, eraseEnd)) {
      patternData_.deleteElement(i);
      patternData_.erase(i, i + 1);
      // no increment
    }
    // Left of deleted area: no change
    else {
      ++i;
    } */
      
/*      int originalPos = spawnTokenPositions_[token.mapNum()];
    token.setMapNum(correctEraseSpawnNum(token.mapNum(),
                                         spawnTokenPositions_[
                                           token.mapNum()],
                                         numSpawnsRemoved,
                                         erasePos,
                                         eraseEnd,
                                         asOverwrite));
      
    // If target is right of the deleted area, renumber
    if (originalPos >= eraseEnd) {
      ++i;
    }
    else if (isInRange(originalPos, erasePos, eraseEnd)) {
      // If this delete is done as part of an overwrite, it will be followed
      // by an insert operation. If some spawn tokens are subsequently
      // inserted to replace the deleted ones, we don't want to
      // remove the referencing tokens, so we leave them in place.
      if (asOverwrite) {
        ++i;
      }
      else {
        patternData_.deleteElement(i);
        patternData_.erase(i, i + 1);
        // no increment
      }
    }
    // Target is left of delete point: no change
    else {
      ++i;
    } */
    }
    break;
  case LaylaPatternTypes::setElevators:
    {
    SetElevatorsToken& token
      = dynamic_cast<SetElevatorsToken&>(patternToken(i));
      
    if ((token.upDestination() != SetElevatorsToken::noDestination)
        && (token.upDestination() != SetElevatorsToken::levelExit)) {
      int originalPos1 = spawnTokenPositions_[token.upDestination()];
      token.setUpDestination(correctEraseSpawnNum(token.upDestination(),
                           originalPos1,
                           numSpawnsRemoved,
                           erasePos,
                           erasePos + eraseSize));
    }
      
    if ((token.downDestination() != SetElevatorsToken::noDestination)
        && (token.downDestination() != SetElevatorsToken::levelExit)) {
      int originalPos2 = spawnTokenPositions_[token.downDestination()];
//        std::cout << "dest: " << (int)(token.downDestination()) << std::endl;
//        std::cout << "pos: " << originalPos2 << std::endl;
//        std::cout << "delete pos: " << erasePos << std::endl;
//        std::cout << "delete end: " << eraseEnd << std::endl;
      token.setDownDestination(correctEraseSpawnNum(token.downDestination(),
                           originalPos2,
                           numSpawnsRemoved,
                           erasePos,
                           erasePos + eraseSize));
//        std::cout << "result: " << (int)(token.downDestination()) << std::endl;
    }
    
    ++i;
    
    }
    break;
  case LaylaPatternTypes::jumpToPosition:
    {
    JumpToPositionToken& token
      = dynamic_cast<JumpToPositionToken&>(patternToken(i));
    // Any jumps into the deleted area from outside it are redirected
    // to the deletion point
    if (isInRange(token.offset(), erasePos, erasePos + eraseSize)) {
//        patternData_.erase(i, i + 1);
      token.setOffset(erasePos);
      ++i;
    }
    // Shift any jumps past the deleted area to the left
    else if (token.offset() >= (erasePos + eraseSize)) {
      token.setOffset(token.offset() - eraseSize);
      ++i;
    }
    else {
      ++i;
    }
    }
    break;
  default:
    ++i;
    break;
  }
}
                 
void LaylaArea::fixReferenceTokenAfterInsert(int i,
                                  int insertPos,
                                  int insertSize,
                                  int numSpawnsAdded) {
  switch (patternToken(i).type()) {
/*    case LaylaPatternTypes::spawnPoint:
    {
    
    ++i;
    }
    break; */
  case LaylaPatternTypes::setMap:
    {
    SetMapToken& token
      = dynamic_cast<SetMapToken&>(patternToken(i));
    
    token.setMapNum(correctInsertSpawnNum(token.mapNum(),
                             spawnTokenPositions_[token.mapNum()],
                             numSpawnsAdded,
                             insertPos,
                             insertPos + insertSize));
    }
    break;
  case LaylaPatternTypes::setElevators:
    {
    SetElevatorsToken& token
      = dynamic_cast<SetElevatorsToken&>(patternToken(i));
    
    if ((token.upDestination() != SetElevatorsToken::noDestination)
        && (token.upDestination() != SetElevatorsToken::levelExit)) {
      token.setUpDestination(correctInsertSpawnNum(token.upDestination(),
                               spawnTokenPositions_[token.upDestination()],
                               numSpawnsAdded,
                               insertPos,
                               insertPos + insertSize));
    }
    
    if ((token.downDestination() != SetElevatorsToken::noDestination)
        && (token.downDestination() != SetElevatorsToken::levelExit)) {
      token.setDownDestination(correctInsertSpawnNum(
                               token.downDestination(),
                               spawnTokenPositions_[token.downDestination()],
                               numSpawnsAdded,
                               insertPos,
                               insertPos + insertSize));
    }
    
    }
    break;
  case LaylaPatternTypes::jumpToPosition:
    {
    JumpToPositionToken& token
      = dynamic_cast<JumpToPositionToken&>(patternToken(i));
    // Shift any jumps past the insertion area to the right
    if ((token.offset() >= insertPos)) {
      token.setOffset(token.offset() + insertSize);
    }
    }
    break;
  default:
    break;
  }
}
  
void LaylaArea::filterStandardPatternTokens(LaylaPatternTokenVector& dst,
                                 LaylaPatternTokenVector& src) {
  dst.clear();
  
  // Filter out nodes
  for (int i = 0; i < src.size(); i++) {
    if (src[i]->type() == LaylaPatternTypes::standardPattern) {
      dst.push_back(src[i]);
    }
  }
}
  
TokenIndexCollection LaylaArea::listOfSpawns() const {
  return spawnTokenPositions_;
}
  
int LaylaArea::numSpawns() {
  return spawnTokenPositions_.size();
}

int LaylaArea::spawnPos(int spawnIndex) {
  return spawnTokenPositions_[spawnIndex];
}
  
int LaylaArea::nodeEntriesAtIndex(int index) {
  int prev = findPreviousStandardPatternIndexNext(index);
  return (index - prev);
}
  
int LaylaArea::moveNode(int dstIndex,
              int srcIndex) {
  if (dstIndex == srcIndex) {
    return dstIndex;
  }
              
  int srcPrev = findPreviousStandardPatternIndexNext(srcIndex);
  int entryCount = srcIndex - srcPrev;
  
//  std::cout << dstIndex << " " << srcPrev << " " << entryCount << std::endl;
  
  return moveTokens(dstIndex, srcPrev, entryCount);
}
  
int LaylaArea::cloneNode(int dstIndex,
                         int srcIndex) {
  int srcPrev = findPreviousStandardPatternIndexNext(srcIndex);
  int entryCount = srcIndex - srcPrev;
  
  // Insert items at new location
  patternData_.insert(srcIndex, entryCount, NULL);
  for (int i = srcIndex; i < srcIndex + entryCount; i++) {
    patternData_.set(i, patternData_[i - entryCount]->cloneNew());
  }
  // Correct tokens
  fixTokensAfterInsert(srcIndex, entryCount);
  
  // Account for increase in target position after inserting tokens
  if (dstIndex >= srcIndex) {
    dstIndex += entryCount;
  }
  
//  std::cout << dstIndex << " " << srcIndex << " " << entryCount << std::endl;
  
  return moveTokens(dstIndex, srcIndex, entryCount);
}
  
int LaylaArea::deleteNode(int index) {
  // If deleting the last node in the data, also delete any node data
  // to the right of the pattern. This obviously isn't correct behavior
  // and should really be removed ASAP, but the editor currently can't
  // touch that data any other way.
  if (index == findPreviousStandardPatternIndex(patternData_.size())) {
    deleteNode(patternData_.size());
  }
  
  int prev = findPreviousStandardPatternIndexNext(index);
  int sz = index - prev;
  
  for (int i = prev; i < index; i++) {
    patternData_.deleteElement(i);
  }
  
  patternData_.erase(prev, index);
  
  fixTokensAfterErase(prev, sz);
  
  return (index - sz);
}
  
bool LaylaArea::moveNodeItemLeft(int index) {
  if ((index <= 0)
      || (patternToken(index - 1).type()
            == LaylaPatternTypes::standardPattern)) {
    return false;
  }
  
  swapTokens(index, index - 1);
  return true;
}

bool LaylaArea::moveNodeItemRight(int index) {
  if ((index >= numPatternTokens())
      || (patternToken(index + 1).type()
            == LaylaPatternTypes::standardPattern)) {
    return false;
  }
  
  swapTokens(index, index + 1);
  return true;
}
  
void LaylaArea::swapTokens(int first, int second) {
  LaylaPatternToken* temp = patternData_[first];
  patternData_.set(first, patternData_[second]);
  patternData_.set(second, temp);
  
  // Correct spawn index if necessary
  if ((patternToken(first).type() == LaylaPatternTypes::spawnPoint)
      || (patternToken(second).type() == LaylaPatternTypes::spawnPoint)) {
    for (int i = 0; i < spawnTokenPositions_.size(); i++) {
      if (spawnTokenPositions_[i] == first) {
        spawnTokenPositions_[i] = second;
      }
      else if (spawnTokenPositions_[i] == second) {
        spawnTokenPositions_[i] = first;
      }
    }
    
    reorderSpawns();
  }
}
  
int LaylaArea::movePattern(int dstIndex,
                           int srcIndex) {
              
  int srcPrev = findPreviousStandardPatternIndexNext(srcIndex);
  int entryCount = srcIndex - srcPrev + 1;
  
//  std::cout << dstIndex << " " << srcIndex << " " << srcPrev << std::endl;
  
  return moveTokens(dstIndex, srcPrev, entryCount) - 1;
}
  
int LaylaArea::movePatterns(int dstIndex,
                            int srcIndex,
                            int srcEndIndex) {
/*  int srcEnd = srcIndex + srcSize;
  int pos = srcIndex;
  while (pos < srcEnd) {
    
  } */
  srcEndIndex = findNextStandardPatternIndex(srcEndIndex);
  srcEndIndex = findPreviousStandardPatternIndexNext(srcEndIndex);
  int dstPrev = findPreviousStandardPatternIndexNext(dstIndex);
  int srcPrev = findPreviousStandardPatternIndexNext(srcIndex);
  int srcNodeSize = srcIndex - srcPrev;
  int entryCount = srcEndIndex - srcPrev;
  
//  int patCount = countStandardPatterns(srcIndex, srcIndex + srcSize);
  
  int result = moveTokens(dstPrev, srcPrev, entryCount);
//  std::cout << result << " " << srcSize << std::endl;
  return result - entryCount + srcNodeSize;
}
  
int LaylaArea::countStandardPatterns(int start, int end) {
  int count = 0;
  for (int i = start; i < end; i++) {
    if (patternToken(i).type() == LaylaPatternTypes::standardPattern) {
      ++count;
    }
  }
  return count;
}
  
int LaylaArea::moveTokens(int dstIndex,
               int srcIndex,
               int srcSize) {
  int srcPrev = srcIndex;
  srcIndex += srcSize;
  int entryCount = srcIndex - srcPrev;
  
  int insertPos = dstIndex;
  // If copying from a lower position to a higher one, account for the
  // deleted tokens when deciding where to insert the data
  if (dstIndex >= srcIndex) {
    insertPos -= entryCount;
  }
//  std::cout << srcPrev << " " << srcIndex << " " << entryCount
//    << " " << insertPos << std::endl;
  
  // Copy items to be moved
  LaylaPatternTokenVector tokenCopy;
  for (int i = srcPrev; i < srcIndex; i++) {
    tokenCopy.push_back(patternData_[i]->cloneNew());
  }
  
  // Count spawns in copied data
/*  int numSpawnsCopied = countSpawns(srcPrev, srcIndex);
  int corrStart = srcIndex - entryCount;
//  int corrEnd = insertPos + entryCount;
  int corrEnd = insertPos;
  int copyCorrStart = insertPos;
  int copyCorrEnd = insertPos + entryCount;
  int spawnChange = -numSpawnsCopied;
  int jumpChange = -entryCount;
  if (dstIndex < srcIndex) {
    corrStart = dstIndex + entryCount;
    corrEnd = srcIndex;
    spawnChange = numSpawnsCopied;
    jumpChange = entryCount;
  } */
  int numSpawnsCopied = countSpawns(srcPrev, srcIndex);
  // The lowest index of data "passed" by the copy
  int corrStart = srcIndex;
  // The limit of data "passed" by the copy
  int corrEnd = dstIndex;
//  int rangeCorrStart = srcPrev;
//  int corrEnd = insertPos + entryCount;
//  int rangeCorrEnd = dstIndex;
  // Change in jump distance for copied jump tokens (the distance of the copy,
  // magnitude dependent on direction)
  int jumpChange = (corrEnd - corrStart);
  if (dstIndex < srcIndex) {
    corrStart = dstIndex;
    corrEnd = srcIndex - entryCount;
//    rangeCorrStart = findPreviousStandardPatternIndexNext(dstIndex);
//    rangeCorrEnd = srcIndex - entryCount;
    jumpChange = -(corrEnd - corrStart);
  }
  // Aliases for the range of the copy vector (due to a dumb mistake)
  int copyCorrStart = 0;
  int copyCorrEnd = entryCount;
  // Number of spaws between the copy source and destination
  int numSpawnsPassed = countSpawns(corrStart, corrEnd);
  // Amount by which to change spawn indices in the "passed" range
  int staticSpawnChange = -numSpawnsCopied;
  // Amount by which to change spawn indices in the copied data
  int spawnChange = -numSpawnsPassed;
  // Change in jump distance for tokens that are "passed" by the copy (the
  // number of tokens copied, magnitude dependent on direction)
  int staticJumpChange = -entryCount;
  if (dstIndex < srcIndex) {
    staticSpawnChange = -staticSpawnChange;
    spawnChange = -spawnChange;
    staticJumpChange = -staticJumpChange;
  }
//  std::cout << corrStart << " " << corrEnd
//    << " " << copyCorrStart << " " << copyCorrEnd << std::endl;
//  std::cout << spawnChange << std::endl;
//  std::cout << staticSpawnChange << std::endl;
//  std::cout << jumpChange << std::endl;
  
  // Fix references in non-copied data
  for (int i = 0; i < patternData_.size(); i++) {
    switch (patternToken(i).type()) {
/*    case LaylaPatternTypes::spawnPoint:
      {
      SpawnPointToken& token
        = dynamic_cast<SpawnPointToken&>(patternToken(i));
      
      if (isInRange(spawnTokenPositions_[token.spawnNum()],
                    corrStart, corrEnd)) {
        token.setSpawnNum(token.spawnNum() + spawnChange);
      }
      }
      break; */
    case LaylaPatternTypes::setMap:
      {
      SetMapToken& token
        = dynamic_cast<SetMapToken&>(patternToken(i));
      
      if (isInRange(spawnTokenPositions_[token.mapNum()],
                    corrStart, corrEnd)) {
        token.setMapNum(token.mapNum() + staticSpawnChange);
      }
      else if (isInRange(spawnTokenPositions_[token.mapNum()],
                    srcPrev, srcIndex)) {
        token.setMapNum(token.mapNum()  - spawnChange);
      }
      }
      break;
    case LaylaPatternTypes::setElevators:
      {
      SetElevatorsToken& token
        = dynamic_cast<SetElevatorsToken&>(patternToken(i));
      
      if ((token.upDestination() != SetElevatorsToken::noDestination)
          && (token.upDestination() != SetElevatorsToken::levelExit)) {
        if (isInRange(spawnTokenPositions_[token.upDestination()],
                    srcPrev, srcIndex)) {
          token.setUpDestination(token.upDestination() - spawnChange);
        }
        else if (isInRange(spawnTokenPositions_[token.upDestination()],
                    corrStart, corrEnd)) {
          token.setUpDestination(
              token.upDestination() + staticSpawnChange);
        }
      }
      
      if ((token.downDestination() != SetElevatorsToken::noDestination)
          && (token.downDestination() != SetElevatorsToken::levelExit)) {
        if (isInRange(spawnTokenPositions_[token.downDestination()],
                    srcPrev, srcIndex)) {
          token.setDownDestination(token.downDestination() - spawnChange);
        }
        else if (isInRange(spawnTokenPositions_[token.downDestination()],
                    corrStart, corrEnd)) {
          token.setDownDestination(
              token.downDestination() + staticSpawnChange);
        }
      }
      }
      break;
    case LaylaPatternTypes::jumpToPosition:
      {
      JumpToPositionToken& token
        = dynamic_cast<JumpToPositionToken&>(patternToken(i));
      
      if (isInRange(token.offset(), corrStart, corrEnd)) {
//        token.setOffset(token.offset() + staticJumpChange);
        int amount = token.offset() + staticJumpChange;
        MiscMath::clamp<int>(amount, 0, patternData_.size() - 1);
        token.setOffset(amount);
      }
      }
      break;
    default:
      break;
    }
  }
  
  // Fix copied tokens
  for (int i = copyCorrStart; i < copyCorrEnd; i++) {
    switch (tokenCopy[i]->type()) {
/*    case LaylaPatternTypes::spawnPoint:
      {
      SpawnPointToken& token
        = dynamic_cast<SpawnPointToken&>(patternToken(i));
      
      token.setSpawnNum(token.spawnNum() - spawnChange);
      }
      break; */
    case LaylaPatternTypes::setMap:
      {
      SetMapToken& token
        = *(dynamic_cast<SetMapToken*>(tokenCopy[i]));
      
      token.setMapNum(token.mapNum() - spawnChange);
      }
      break;
    case LaylaPatternTypes::setElevators:
      {
      SetElevatorsToken& token
        = *(dynamic_cast<SetElevatorsToken*>(tokenCopy[i]));
      
      if ((token.upDestination() != SetElevatorsToken::noDestination)
          && (token.upDestination() != SetElevatorsToken::levelExit)) {
        token.setUpDestination(token.upDestination() - spawnChange);
      }
      
      if ((token.downDestination() != SetElevatorsToken::noDestination)
          && (token.downDestination() != SetElevatorsToken::levelExit)) {
        token.setDownDestination(token.downDestination() - spawnChange);
      }
      }
      break;
    case LaylaPatternTypes::jumpToPosition:
      {
      JumpToPositionToken& token
        = *(dynamic_cast<JumpToPositionToken*>(tokenCopy[i]));
      
//      if (isInRange(token.offset(), corrStart, corrEnd)) {
      int amount = token.offset() + jumpChange;
      MiscMath::clamp<int>(amount, 0, patternData_.size() - 1);
      token.setOffset(amount);
//      }
      }
      break;
    default:
      break;
    }
  }
  
  // Remove items from original location
  patternData_.erase(srcPrev, srcIndex);
  
  // Correct tokens
  fixTokensAfterMoveErase(srcPrev, entryCount);
  
  // Insert items at new location
  patternData_.insert(insertPos, entryCount, NULL);
  for (int i = 0; i < tokenCopy.size(); i++) {
    patternData_.set(insertPos + i, tokenCopy[i]);
  }
  
  // Correct tokens
  fixTokensAfterMoveInsert(insertPos, entryCount);
  
  // Return the final index position of the pattern the node data was
  // copied to (which will either be the same if the copy was from left
  // to right, or increase by the amount of data copied if the copy was
  // from right to left)
  if (dstIndex < srcIndex) {
    return dstIndex + entryCount;
  }
  else {
    return dstIndex;
  }
}
  
int LaylaArea::addSpawn(int index) {
  patternData_.insert(index, numTokensInSpawnAdd, NULL);
  
  patternData_.set(index + 0, new SpawnPointToken());
  patternData_.set(index + 1, new SetMapToken());
  patternData_.set(index + 2,
                   new SetScrollingToken(1,
                          SetScrollingToken::scrollingEnabledValue));
  patternData_.set(index + 3,
                   new SetAreaToken(SetAreaToken::caveID,
                          0));
  
  fixTokensAfterInsert(index, numTokensInSpawnAdd);
  
  SpawnPointToken& spawnPointToken
    = dynamic_cast<SpawnPointToken&>(patternToken(index + 0));
  SetMapToken& setMapToken
    = dynamic_cast<SetMapToken&>(patternToken(index + 1));
  SetScrollingToken& setScrollingToken
    = dynamic_cast<SetScrollingToken&>(patternToken(index + 2));
  SetAreaToken& setAreaToken
    = dynamic_cast<SetAreaToken&>(patternToken(index + 3));
  
  // Try to inherit the most recent settings
  setMapToken.setMapNum(spawnPointToken.spawnNum());
  setMapToken.setParam2(findPreviousYFlag(index));
  setAreaToken.setAreaType(findPreviousAreaType(index));
  setAreaToken.setBackgroundMetatile(findPreviousBackgroundMetatile(index));
  setScrollingToken.setKillsToNextFood(findPreviousKillsToNextFood(index));
  
  return index + numTokensInSpawnAdd;
}
  
Tbyte LaylaArea::findPreviousAreaType(int index) {
  Tbyte areaType = SetAreaToken::caveID;
  for (int i = index; i >= 0; i--) {
    if (patternToken(i).type() == LaylaPatternTypes::setArea) {
      SetAreaToken& setAreaToken
        = dynamic_cast<SetAreaToken&>(patternToken(i));
      areaType = setAreaToken.areaType();
    }
  }
  return areaType;
}

Tbyte LaylaArea::findPreviousKillsToNextFood(int index) {
  Tbyte foodKills = 0;
  for (int i = index; i >= 0; i--) {
    if (patternToken(i).type() == LaylaPatternTypes::setScrolling) {
      SetScrollingToken& setScrollingToken
        = dynamic_cast<SetScrollingToken&>(patternToken(i));
      foodKills = setScrollingToken.killsToNextFood();
    }
  }
  return foodKills;
}

Tbyte LaylaArea::findPreviousBackgroundMetatile(int index) {
  Tbyte backgroundMetatile = 0;
  for (int i = index; i >= 0; i--) {
    if (patternToken(i).type() == LaylaPatternTypes::setArea) {
      SetAreaToken& setAreaToken
        = dynamic_cast<SetAreaToken&>(patternToken(i));
      backgroundMetatile = setAreaToken.backgroundMetatile();
    }
  }
  return backgroundMetatile;
}

Tbyte LaylaArea::findPreviousYFlag(int index) {
  Tbyte yFlag = 0;
  for (int i = index; i >= 0; i--) {
    if (patternToken(i).type() == LaylaPatternTypes::setMap) {
      SetMapToken& setMapToken
        = dynamic_cast<SetMapToken&>(patternToken(i));
      yFlag = setMapToken.param2();
    }
  }
  return yFlag;
}

void LaylaArea
    ::generatePatternAreaTypeGuesses(PatternPreviewAreaTypeCollection& dst,
                                     LaylaPatternDefinitionTable&
                                       patternDefinitions) {
  dst.clear();
  
  // We don't actually know which metatile set each pattern is intended
  // to use, so we have to guess heuristically.
  // Iterate over each pattern in the table.
  int lastAreaType = 0;
  for (int i = 0; i < patternDefinitions.numPatterns(); i++) {
    int areaType = 0;
          
    // Iterate over the current level, keeping track of the area type.
    // If we can find this pattern in the level, draw the preview using
    // that area type.
    bool done = false;
    for (int j = 0; j < numPatternTokens(); j++) {
      switch (patternToken(j).type()) {
      case LaylaPatternTypes::standardPattern:
      {
        StandardPatternToken& patternToken
          = dynamic_cast<StandardPatternToken&>(
              *(patternData_[j]));
        
        if (patternToken.patternNum() == i) {
          lastAreaType = areaType;
          
          done = true;
          break;
        }
      }
        break;
      // Switch area type when set area tokens are encountered
      case LaylaPatternTypes::setArea:
      {
        SetAreaToken& setAreaToken
          = dynamic_cast<SetAreaToken&>(
             *(patternData_[j]));
        areaType = setAreaToken.areaType();
      }
        break;
      default:
        break;
      }
      
      if (done) {
        break;
      }
    } // end for
    
    // If we couldn't find the pattern in the level, assume it's the same
    // as the previous
    if (!done) {
      areaType = lastAreaType;
    }
    
    dst.push_back(areaType);
  }
}
  
void LaylaArea
    ::fixReferencesAfterNewPatternAdded(int newPatternIndex) {
  for (int i = 0; i < numPatternTokens(); i++) {
    switch (patternToken(i).type()) {
    case LaylaPatternTypes::standardPattern:
    {
      StandardPatternToken& patternToken
        = dynamic_cast<StandardPatternToken&>(
            *(patternData_[i]));
      
      // Any standard pattern with a number greater than or equal
      // to the insert position needs to be incremented
      if (patternToken.patternNum() >= newPatternIndex) {
        patternToken.setPatternNum(patternToken.patternNum() + 1);
      }
    }
      break;
    default:
      break;
    }
  }
}
  
void LaylaArea
    ::fixReferencesAfterPatternRemoved(int patternIndex) {
  for (int i = 0;
       i < numPatternTokens();
       /* NO INCREMENT */) {
    switch (patternToken(i).type()) {
    case LaylaPatternTypes::standardPattern:
    {
      StandardPatternToken& patternToken
        = dynamic_cast<StandardPatternToken&>(
            *(patternData_[i]));
      
      // Any standard pattern with a number greater than
      // the insert position needs to be decremented
      if (patternToken.patternNum() > patternIndex) {
        patternToken.setPatternNum(patternToken.patternNum() - 1);
        ++i;
      }
      // Remove all uses of the removed pattern
      else if (patternToken.patternNum() == patternIndex) {
        deletePatternsAndNodesAt(i, 1);
//        patternData_.erase(i, i + 1);
        
        // Don't increment i
      }
      else {
        ++i;
      }
      
      
    }
      break;
    default:
      ++i;
      break;
    }
  }
}


};
