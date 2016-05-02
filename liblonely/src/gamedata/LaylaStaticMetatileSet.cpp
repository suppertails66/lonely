#include "gamedata/LaylaStaticMetatileSet.h"
#include <iostream>

namespace Lonely {


LaylaStaticMetatileSet::LaylaStaticMetatileSet()
  : numMetatiles_(0),
    evenTilesTable_(0),
    oddTilesTable_(0),
    attributesTable_(0),
    propertiesTable_(0) { };
  
LaylaStaticMetatileSet::LaylaStaticMetatileSet(const NesRom& rom,
                 int numMetatiles__,
                 Taddress evenTilesTable__,
                 Taddress oddTilesTable__,
                 Taddress attributesTable__,
                 Taddress propertiesTable__)
  : numMetatiles_(numMetatiles__),
    evenTilesTable_(evenTilesTable__),
    oddTilesTable_(oddTilesTable__),
    attributesTable_(attributesTable__),
    propertiesTable_(propertiesTable__),
    metatiles_(rom,
               numMetatiles__,
               evenTilesTable__,
               oddTilesTable__,
               attributesTable__,
               propertiesTable__) { };

void LaylaStaticMetatileSet::exportToRom(NesRom& rom) const {
  metatiles_.writeToData(rom,
                         numMetatiles_,
                         evenTilesTable_,
                         oddTilesTable_,
                         attributesTable_,
                         propertiesTable_);
}

int LaylaStaticMetatileSet::save(Tstring& data) const {
  int byteCount = 0;
  
  SaveHelper::saveInt(data, numMetatiles_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, evenTilesTable_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, oddTilesTable_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, attributesTable_,
                      ByteSizes::uint32Size);
  SaveHelper::saveInt(data, propertiesTable_,
                      ByteSizes::uint32Size);
  
  byteCount += metatiles_.save(data);
  
  return byteCount;
}

int LaylaStaticMetatileSet::load(const Tbyte* data) {
  int byteCount = 0;
  
  numMetatiles_ = LoadHelper::loadInt(data, byteCount,
                                      ByteSizes::uint32Size);
  evenTilesTable_ = LoadHelper::loadInt(data, byteCount,
                                      ByteSizes::uint32Size);
  oddTilesTable_ = LoadHelper::loadInt(data, byteCount,
                                      ByteSizes::uint32Size);
  attributesTable_ = LoadHelper::loadInt(data, byteCount,
                                      ByteSizes::uint32Size);
  propertiesTable_ = LoadHelper::loadInt(data, byteCount,
                                      ByteSizes::uint32Size);
  
  byteCount += metatiles_.load(data + byteCount);
  
  return byteCount;
}
  
LaylaMetatileSet& LaylaStaticMetatileSet::metatiles() {
  return metatiles_;
}


};
