#include "gamedata/LaylaMetatileSet.h"
#include "exception/NotEnoughSpaceException.h"
#include <iostream>

namespace Lonely {


LaylaMetatileSet::LaylaMetatileSet()
  : metatiles_(0) { };
  
LaylaMetatileSet::LaylaMetatileSet(const NesRom& rom,
                 int numMetatiles,
                 Taddress evenTilesTable,
                 Taddress oddTilesTable,
                 Taddress attributesTable,
                 Taddress propertiesTable)
  : metatiles_(0) {
  readFromData(rom,
               numMetatiles,
               evenTilesTable,
               oddTilesTable,
               attributesTable,
               propertiesTable);
}
                   
void LaylaMetatileSet::readFromData(const NesRom& rom,
                  int numMetatiles,
                  Taddress evenTilesTable,
                  Taddress oddTilesTable,
                  Taddress attributesTable,
                  Taddress propertiesTable) {
  metatiles_.resize(numMetatiles);
  
  for (int i = 0; i < numMetatiles; i++) {
    metatiles_[i].upperLeft
      = fetchUpperLeftTile(rom.directRead(evenTilesTable), i);
    metatiles_[i].lowerLeft
      = fetchLowerLeftTile(rom.directRead(evenTilesTable), i);
    metatiles_[i].upperRight
      = fetchUpperRightTile(rom.directRead(oddTilesTable), i);
    metatiles_[i].lowerRight
      = fetchLowerRightTile(rom.directRead(oddTilesTable), i);
    metatiles_[i].palette
      = fetchAttribute(rom.directRead(attributesTable), i);
    metatiles_[i].setProperties(
        fetchProperties(rom.directRead(propertiesTable), i));
  }
}
                   
void LaylaMetatileSet::writeToData(NesRom& rom,
                 int maxNumMetatiles,
                 Taddress evenTilesTable,
                 Taddress oddTilesTable,
                 Taddress attributesTable,
                 Taddress propertiesTable) const {
  if (metatiles_.size() > maxNumMetatiles) {
    throw NotEnoughSpaceException(TALES_SRCANDLINE,
                                  "LaylaMetatileSet::writeToData() const",
                                  metatiles_.size());
  }
  
  for (int i = 0; i < metatiles_.size(); i++) {
    writeEvenTiles(rom.directWrite(evenTilesTable),
                   metatiles_[i],
                   i);
    writeOddTiles(rom.directWrite(oddTilesTable),
                   metatiles_[i],
                   i);
    writeProperties(rom.directWrite(propertiesTable),
                   metatiles_[i],
                   i);
    writeAttribute(rom.directWrite(attributesTable),
                   metatiles_[i],
                   i);
  }
}
  
int LaylaMetatileSet::save(Tstring& data) const {
  int byteCount = 0;
  
  // Number of metatiles
  byteCount += SaveHelper::saveInt(data, metatiles_.size(),
                                   ByteSizes::uint32Size);
  
  // Metatiles
  for (int i = 0; i < metatiles_.size(); i++) {
    byteCount += metatiles_[i].save(data);
  }
  
  return byteCount;
}

int LaylaMetatileSet::load(const Tbyte* data) {
  int byteCount = 0;
  
  // Number of metatiles
  metatiles_.resize(LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size));
  
  // Metatiles
  for (int i = 0; i < metatiles_.size(); i++) {
    byteCount += metatiles_[i].load(data + byteCount);
  }
  
  return byteCount;
}

int LaylaMetatileSet::size() const {
  return metatiles_.size();
}

LaylaMetatile& LaylaMetatileSet::metatile(int index) {
  return metatiles_[index];
}

const LaylaMetatile& LaylaMetatileSet::metatile(int index) const {
  return metatiles_[index];
}
  
void LaylaMetatileSet::removeMetatile(int index) {
  metatiles_.erase(metatiles_.begin() + index);
}
  
int LaylaMetatileSet::addMetatile(int index) {
  metatiles_.insert(metatiles_.begin() + index,
                    LaylaMetatile());
  
  return index;
}

Tbyte LaylaMetatileSet::fetchUpperLeftTile(const Tbyte* evenTilesTable,
                           int index) {
  return *(evenTilesTable + (index * 2));
}
  
Tbyte LaylaMetatileSet::fetchLowerLeftTile(const Tbyte* evenTilesTable,
                         int index) {
  return *(evenTilesTable + (index * 2) + 1);
}

Tbyte LaylaMetatileSet::fetchUpperRightTile(const Tbyte* oddTilesTable,
                         int index) {
  return *(oddTilesTable + (index * 2));
}

Tbyte LaylaMetatileSet::fetchLowerRightTile(const Tbyte* oddTilesTable,
                         int index) {
  return *(oddTilesTable + (index * 2) + 1);
}
  
Tbyte LaylaMetatileSet::fetchProperties(const Tbyte* propertiesTable,
                      int index) {
  return *(propertiesTable + index);
}
  
Tbyte LaylaMetatileSet::fetchAttribute(const Tbyte* attributesTable,
                     int index) {
  int realIndex = (index / 4);
  Tbyte packed = *(attributesTable + realIndex);
  
  int shift = (index % 4) * 2;
  Tbyte mask = (0x03 << shift);
  return (packed & mask) >> shift;
}
                       
void LaylaMetatileSet::writeEvenTiles(Tbyte* evenTilesTable,
                           const LaylaMetatile& metatile,
                           int index) {
  *(evenTilesTable + (index * 2)) = metatile.upperLeft;
  *(evenTilesTable + (index * 2) + 1) = metatile.lowerLeft;
}
                     
void LaylaMetatileSet::writeOddTiles(Tbyte* oddTilesTable,
                           const LaylaMetatile& metatile,
                           int index) {
  *(oddTilesTable + (index * 2)) = metatile.upperRight;
  *(oddTilesTable + (index * 2) + 1) = metatile.lowerRight;
}
                     
void LaylaMetatileSet::writeProperties(Tbyte* propertiesTable,
                            const LaylaMetatile& metatile,
                            int index) {
  *(propertiesTable + index) = metatile.packProperties();
}
                     
void LaylaMetatileSet::writeAttribute(Tbyte* attributeTable,
                           const LaylaMetatile& metatile,
                           int index) {
  int realIndex = (index / 4);
  int shift = (index % 4) * 2;
  
  // Clear the bits we are about to re-set
  Tbyte clearMask = (0x03 << shift) ^ 0xFF;
  *(attributeTable + realIndex) &= clearMask;
  
  *(attributeTable + realIndex) |= metatile.palette << shift;
}


};
