#include "gamedata/LaylaObjectPatternTable.h"
#include "gamedata/LaylaMetatile.h"
#include "gamedata/LaylaObjectSets.h"
#include "gamedata/LonelyLaylaMappings.h"
#include "gamedata/LaylaPatternToken.h"
#include "structs/AddressTableReader.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "nes/NesTile.h"
#include "nes/UxRomBanking.h"
#include "exception/NotEnoughSpaceException.h"
#include <iostream>

namespace Lonely {


LaylaObjectPatternTable::LaylaObjectPatternTable() { };
  
LaylaObjectPatternTable::LaylaObjectPatternTable(const Tbyte* src,
                          int numEntries,
                          Taddress baseAddress,
                          int dataSize) {
  readFromData(src,
               numEntries,
               baseAddress,
               dataSize);
}
  
LaylaObjectPatternTable::~LaylaObjectPatternTable() {
  patterns_.deleteData();
}
  
LaylaObjectPatternTable
    ::LaylaObjectPatternTable(const LaylaObjectPatternTable& src) {
  cloneFrom(src);
}

LaylaObjectPatternTable& LaylaObjectPatternTable
    ::operator=(const LaylaObjectPatternTable& src) {
  cloneFrom(src);
  
  return *this;
}
  
void LaylaObjectPatternTable::readFromData(const Tbyte* src,
                  int numEntries,
                  Taddress baseAddress,
                  int dataSize) {
      
  patterns_.deleteData();
  patterns_.clear();
  patternIndices_.clear();
  patternIndices_.assign(numEntries, 0);
                  
  Tarray<Taddress> rawIndexTable;
  
  int indexSize = numEntries * ByteSizes::uint16Size;
  
  // Read table of raw index addresses
  AddressTableReader::readAddressTable(rawIndexTable,
                                       src,
                                       numEntries,
                                       ByteSizes::uint16Size,
                                       EndiannessTypes::little);
  src += indexSize;
  
  // Read object data
  Taddress dataBase = baseAddress + indexSize;
  int total = 0;
  while (total < dataSize) {
    // Update index entries corresponding to this position
    Taddress bankedAddress = UxRomBanking::directToBankedAddressMovable(
                                dataBase + total);
    for (int i = 0; i < rawIndexTable.size(); i++) {
      if (rawIndexTable[i] == bankedAddress) {
        // Index entry points to token we are about to read
        patternIndices_[i] = patterns_.size();
      }
    }
  
    Tbyte next = *src;
    
    // Expanded spawn
    if ((next & expandedSpawnMask_) != 0) {
      patterns_.push_back(new LaylaObjectExpandedSpawnToken(src));
    }
    // Delay
    else if ((next & delayMask_) != 0) {
      patterns_.push_back(new LaylaObjectDelayToken(src));
    }
    // Standard spawn
    else {
      patterns_.push_back(new LaylaObjectStandardSpawnToken(src));
    }
    
    int amount = patterns_[patterns_.size() - 1]->size();
    total += amount;
    src += amount;
  }
}
  
void LaylaObjectPatternTable::writeToData(Tbyte* dst,
                 Taddress baseAddress,
                 int maxSize) const {
  int totalIndexSize = patternIndices_.size() * ByteSizes::uint16Size;
  int totalExportSize = totalIndexSize + totalPatternDataSize();
  
  if (totalExportSize > maxSize) {
    throw NotEnoughSpaceException(TALES_SRCANDLINE,
                                  "LaylaObjectPatternTable::writeToData()",
                                  totalExportSize);
  }
  
  Tarray<Taddress> exportAddresses(patternIndices_.size());
  Taddress dataBaseAddress = baseAddress + totalIndexSize;
  
  // Write base for index
  Tbyte* indexDst = dst;
  // Write location for pattern data
  Tbyte* dataDst = dst + totalIndexSize;
  
  // Write the pattern data
  int total = 0;
  for (int i = 0; i < patterns_.size(); i++) {
    // Fill in any index addresses corresponding to this position
    for (int j = 0; j < patternIndices_.size(); j++) {
      if (patternIndices_[j] == i) {
        exportAddresses[j] =
          UxRomBanking::directToBankedAddressMovable(
            dataBaseAddress + total);
      }
    }
    
    int amount = patterns_[i]->writeToData(dataDst);
    
    total += amount;
    dataDst += amount;
  }
  
  // Write the index
  AddressTableReader::writeAddressTable(
      indexDst,
      exportAddresses);
}
  
int LaylaObjectPatternTable::save(Tstring& data) const {
  int byteCount = 0;
  
  // Number of tokens
  byteCount += SaveHelper::saveInt(data, patterns_.size(),
                                   ByteSizes::uint32Size);
  
  // Tokens
  for (int i = 0; i < patterns_.size(); i++) {
    byteCount += patterns_[i]->save(data);
  }
  
  // Number of object token positions
  byteCount += SaveHelper::saveInt(data, patternIndices_.size(),
                                   ByteSizes::uint32Size);
  
  // Object token positions
  for (int i = 0; i < patternIndices_.size(); i++) {
    byteCount += SaveHelper::saveInt(data, patternIndices_[i],
                                   ByteSizes::uint16Size);
  }
  
  return byteCount;
}

int LaylaObjectPatternTable::load(const Tbyte* data) {
  int byteCount = 0;
  
  patterns_.deleteData();
  patterns_.clear();
  patternIndices_.clear();
  
  // Number of tokens
  patterns_.resize(LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size));
  
  // Tokens
  for (int i = 0; i < patterns_.size(); i++) {
    LaylaObjectPatternToken* token
      = LaylaObjectPatternToken::loadAndAllocateToken(
          data + byteCount);
    patterns_.set(i, token);
    byteCount += token->size() + LaylaObjectPatternToken::loadOverhead;
  }
  
  // Number of object token positions
  patternIndices_.resize(LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size));
  
  // Object token positions
  for (int i = 0; i < patternIndices_.size(); i++) {
    patternIndices_[i] = LoadHelper::loadInt(data, byteCount,
                              ByteSizes::uint16Size);
  }
  
  return byteCount;
}

int LaylaObjectPatternTable::numPatternTokens() const {
  return patterns_.size();
}

LaylaObjectPatternToken& LaylaObjectPatternTable
    ::patternToken(int index) {
  return *(patterns_[index]);
}

const LaylaObjectPatternToken& LaylaObjectPatternTable
    ::patternToken(int index) const {
  return *(patterns_[index]);
}

int LaylaObjectPatternTable::totalPatternDataSize() const {
  int total = 0;
  for (int i = 0; i < patterns_.size(); i++) {
    total += patterns_[i]->size();
  }
  
  return total;
}
  
void LaylaObjectPatternTable::drawPatternObjectOverlay(Graphic& dst,
                              int x, int y,
                              double zoom,
                              int patternIndex,
                              int patternMetatileWidth,
                              int objectSetNum,
                              const NesPatternTable& patternTable,
                              const NesPaletteQuad& palettes) {
                              
  int drawX = x;
  int drawY = y;
  int pos = patternIndices_[patternIndex];
  int remaining = patternMetatileWidth * LaylaMetatile::widthInTiles;
  while ((remaining > 0)
         && (pos < patterns_.size())) {
    switch (patternToken(pos).type()) {
    case LaylaObjectPatternTypes::delay:
      {
      LaylaObjectDelayToken& token
        = dynamic_cast<LaylaObjectDelayToken&>(patternToken(pos));
      
      // Advance drawing and source positions
      drawX += (token.delayAmount() * LaylaMetatile::width) * zoom;
      // Account for delay amount being in metatiles rather than tiles
      remaining -= (token.delayAmount()
            * (LaylaMetatile::width / NesTile::width));
      }
      break;
    case LaylaObjectPatternTypes::standardSpawn:
      {
      LaylaObjectStandardSpawnToken& token
        = dynamic_cast<LaylaObjectStandardSpawnToken&>(patternToken(pos));
      
      drawObjectAndAdvance(dst,
                           drawX, drawY,
                           zoom,
                           objectSetNum,
                           token.objectType(),
                           patternTable,
                           palettes,
                           token.continueInterpreting(),
                           remaining);
      }
      break;
    case LaylaObjectPatternTypes::expandedSpawn:
      {
//      std::cout << patternIndex << std::endl;
      LaylaObjectExpandedSpawnToken& token
        = dynamic_cast<LaylaObjectExpandedSpawnToken&>(patternToken(pos));
      
      // THIS IS WRONG, FIX IT
      int objY = y - token.realY();
      
      for (int i = 0; i < token.realSpawnCount(); i++) {
        drawObjectAndAdvance(dst,
                             drawX, objY,
                             zoom,
                             objectSetNum,
                             token.objectType(),
                             patternTable,
                             palettes,
                             token.continueInterpreting(),
                             remaining);
        objY += LaylaMetatile::height;
      }
      }
      break;
    default:
      
      break;
    }
    
    ++pos;
  }
  
  // If we went past the end of the data, mark the remainder of the
  // pattern (this will probably cause errors if exported to ROM)
  if (remaining > 0) {
    int remainingW = (remaining * NesTile::width * zoom);
    
    dst.fillRect(drawX, y,
                 remainingW, LaylaMetatile::height * zoom,
                 Tcolor(0xFF, 0x00, 0x00),
                 Graphic::noTransUpdate);
  }
}

void LaylaObjectPatternTable::drawBossOverlay(Graphic& dst,
                              int x, int y,
                              double zoom,
                              int bossID,
                              int objectSetNum,
                              const NesPatternTable& patternTable,
                              const NesPaletteQuad& palettes) {
  // Check for boss validity for current level set and draw invalid
  // mapping indicator if so
  // ...
  
  
  // Draw mapping
  LonelyLaylaMapping mapping
    = LonelyLaylaMappings::lonelyBossMapping(
        static_cast<LaylaBosses::LaylaBoss>(bossID));
  
  Graphic sprite;
  mapping.draw(sprite,
               patternTable,
               palettes,
               true);
  
  drawSprite(dst, sprite,
             x, y,
             zoom);
  
/*  drawSpriteAndAdvance(dst,
                       x, y,
                       zoom,
                       objectSetNum,
                       token.objectType(),
                       patternTable,
                       palettes,
                       token.continueInterpreting(),
                       remaining); */
}
  
Tstring LaylaObjectPatternTable::descriptionString(int index) const {
  return patternToken(index).descriptionString();
}
  
void LaylaObjectPatternTable::cloneFrom(const LaylaObjectPatternTable& src) {
  patterns_.deleteData();
  patterns_.clear();
  for (int i = 0; i < src.patterns_.size(); i++) {
    patterns_.push_back(src.patterns_[i]->cloneNew());
  }
  
  patternIndices_ = src.patternIndices_;
}
  
void LaylaObjectPatternTable::drawObjectAndAdvance(Graphic& dst,
                          int& drawX, int& drawY,
                          double zoom,
                          int objectSetNum,
                          Tbyte localObjectID,
                          const NesPatternTable& patternTable,
                          const NesPaletteQuad& palettes,
                          bool continueInterpreting,
                          int& remaining) const {
  LonelyLaylaObjects::LonelyLaylaObject lonelyObjectID
    = LaylaObjectSets::lonelyID(objectSetNum, localObjectID);
  LonelyLaylaMapping mapping
    = LonelyLaylaMappings::lonelyObjectMapping(lonelyObjectID);
  
  Graphic sprite;
  mapping.draw(sprite,
               patternTable,
               palettes,
               true);
  
  drawSprite(dst, sprite,
             drawX, drawY,
             zoom);
  
  // Advance drawing and source positions.
  // If the continue interpreting flag is set, then there is no gap between
  // this pattern and the next one. If not, the next object event will not
  // be processed until the next tile to the right is loaded, causing a
  // gap of one tile.
  if (!continueInterpreting) {
    drawX += (NesTile::width) * zoom;
    remaining -= 1;
  }
}
  
void LaylaObjectPatternTable::drawSprite(Graphic& dst,
                          Graphic& src,
                          int drawX, int drawY,
                          double zoom) const {
  if (zoom == 1.00) {
    dst.blit(src,
             Box(drawX, drawY, 0, 0),
             Graphic::transUpdate);
  }
  else {
    dst.scaleAndBlit(src,
                     Box(drawX, drawY,
                         src.w() * zoom, src.h() * zoom),
                     Graphic::transUpdate);
  }
}
  
int LaylaObjectPatternTable::indexOfPattern(int patternNum) const {
  return patternIndices_[patternNum];
}
  
void LaylaObjectPatternTable::setIndexOfPattern(int patternNum,
                       int newIndex) {
  patternIndices_[patternNum] = newIndex;
}
  
void LaylaObjectPatternTable::insertPatternIntoIndex(int index) {
  patternIndices_.insert(patternIndices_.begin() + index,
                         0);
}
  
void LaylaObjectPatternTable::removePatternFromIndex(int index) {
  patternIndices_.erase(patternIndices_.begin() + index);
}
  
int LaylaObjectPatternTable::removeObjectPattern(int index) {
  if (patterns_.size() <= 1) {
    return index;
  }

  patterns_.deleteElement(index);
  patterns_.erase(index, index + 1);
  
  // Decrement referencing entries in index
  for (int i = 0; i < patternIndices_.size(); i++) {
    // If the last entry was deleted, anything referencing it
    // must be shifted left
    if (index == patterns_.size()) {
      if (patternIndices_[i] >= index) {
        --(patternIndices_[i]);
      }
    }
    // In all other cases, a new entry fills the old position
    else if (patternIndices_[i] > index) {
      --(patternIndices_[i]);
    }
  }
  
  if (index == patterns_.size()) {
    return index - 1;
  }
  else {
    return index;
  }
}
  
int LaylaObjectPatternTable
    ::addObjectPattern(int index, LaylaObjectPatternToken& token) {
  for (int i = 0; i < patternIndices_.size(); i++) {
    if (patternIndices_[i] >= index) {
      ++(patternIndices_[i]);
    }
  }
  
  patterns_.insert(index, &token);
  
  return index;
}
  
void LaylaObjectPatternTable
    ::swapItems(int first, int second) {
  LaylaObjectPatternToken* temp = patterns_[first];
  patterns_.set(first, patterns_[second]);
  patterns_.set(second, temp);
  
  for (int i = 0; i < patternIndices_.size(); i++) {
    if (patternIndices_[i] == first) {
      patternIndices_[i] = second;
    }
    else if (patternIndices_[i] == second) {
      patternIndices_[i] = first;
    }
  }
}
  
void LaylaObjectPatternTable::drawEntryIcon(
                   Graphic& dst,
                   int entryIndex,
                   int objectSetNum,
                   const NesPatternTable& patternTable,
                   const NesPaletteQuad& palettes) const {
  dst = Graphic(LaylaMetatile::width * 2, LaylaMetatile::height * 2);
  dst.clear(Tcolor(0x00, 0x00, 0x00));

  // start from centerpoint
  int drawX = LaylaMetatile::width;
  int drawY = LaylaMetatile::height;
  int remaining = 255;
  
  int pos = entryIndex;
  
//  for (int pos = 0; pos < numPatternTokens(); pos++) {
    switch (patternToken(pos).type()) {
    case LaylaObjectPatternTypes::delay:
      {
      const LaylaObjectDelayToken& token
        = dynamic_cast<const LaylaObjectDelayToken&>(patternToken(pos));
      }
      break;
    case LaylaObjectPatternTypes::standardSpawn:
    case LaylaObjectPatternTypes::expandedSpawn:
      {
      const LaylaObjectStandardSpawnToken& token
        = dynamic_cast<const LaylaObjectStandardSpawnToken&>(
            patternToken(pos));
      
      drawObjectIcon(dst,
                     token.objectType(),
                     objectSetNum,
                     patternTable,
                     palettes);
      
      }
      break;
/*    case LaylaObjectPatternTypes::expandedSpawn:
      {
      LaylaObjectExpandedSpawnToken& token
        = dynamic_cast<LaylaObjectExpandedSpawnToken&>(patternToken(pos));
      
      }
      break; */
    default:
      
      break;
    }
//  }
}
  
void LaylaObjectPatternTable::drawEntryIcon(Graphic& dst,
                   int entryIndex,
                   int levelNum,
                   int objectSetNum,
                   const LaylaGraphics& graphics,
                   const LaylaPalettes& palettes) const {
  int pos = entryIndex;
  Tbyte areaType = SetAreaToken::caveID;
  
  // If an object token is at this index, look up its "correct" area type
  switch (patternToken(pos).type()) {
  case LaylaObjectPatternTypes::standardSpawn:
  case LaylaObjectPatternTypes::expandedSpawn:
    {
    const LaylaObjectStandardSpawnToken& token
      = dynamic_cast<const LaylaObjectStandardSpawnToken&>(
          patternToken(pos));
    
    LonelyLaylaObjects::LonelyLaylaObject lonelyObjectID
      = LaylaObjectSets::lonelyID(objectSetNum, token.objectType());
    areaType = LonelyLaylaMappings::areaTypeOfObject(lonelyObjectID);
    }
    break;
  default:
    break;
  }
  
  const NesPatternTable& patternTable
    = graphics.areaSprites(areaType);
  NesPaletteQuad spritePalettes
    = palettes.generateAreaPalette(areaType, levelNum).spritePalettes();
  
  drawEntryIcon(dst,
                entryIndex,
                objectSetNum,
                patternTable,
                spritePalettes);
}
  
void LaylaObjectPatternTable::drawObjectIcon(Graphic& dst,
                   int levelNum,
                   int objectNum,
                   int objectSetNum,
                   const LaylaGraphics& graphics,
                   const LaylaPalettes& palettes) const {
  
  LonelyLaylaObjects::LonelyLaylaObject lonelyObjectID
    = LaylaObjectSets::lonelyID(objectSetNum, objectNum);
  Tbyte areaType = LonelyLaylaMappings::areaTypeOfObject(lonelyObjectID);
  
  const NesPatternTable& patternTable
    = graphics.areaSprites(areaType);
  NesPaletteQuad spritePalettes
    = palettes.generateAreaPalette(areaType, levelNum).spritePalettes();
  
  drawObjectIcon(dst,
                objectNum,
                objectSetNum,
                patternTable,
                spritePalettes);
}
  
void LaylaObjectPatternTable::drawObjectIcon(Graphic& dst,
                   int objectNum,
                   int objectSetNum,
                   const NesPatternTable& patternTable,
                   const NesPaletteQuad& palettes) const {
  dst = Graphic(LaylaMetatile::width * 2, LaylaMetatile::height * 2);
  dst.clear(Tcolor(0x00, 0x00, 0x00));

  // start from centerpoint
  int drawX = LaylaMetatile::width;
  int drawY = LaylaMetatile::height;
  int remaining = 255;
  
  LonelyLaylaObjects::LonelyLaylaObject lonelyObjectID
    = LaylaObjectSets::lonelyID(objectSetNum, objectNum);
  LonelyLaylaMapping mapping
    = LonelyLaylaMappings::lonelyObjectMapping(lonelyObjectID);
  
  // Center graphic
  if ((mapping.w() == 0)
      || (mapping.h() == 0)) {
    // using magic here -- need some constants for invalid mappings
    drawX -= LonelyLaylaMapping::invalidMappingDrawW / 2;
    drawY -= LonelyLaylaMapping::invalidMappingDrawH / 2;
  }
  else {
    drawX -= mapping.w() / 2 * NesTile::width;
    drawY -= mapping.h() / 2 * NesTile::height;
  }
  
  drawObjectAndAdvance(dst,
                       drawX, drawY,
                       1.00,
                       objectSetNum,
                       objectNum,
                       patternTable,
                       palettes,
                       false,
                       remaining);
}


};
