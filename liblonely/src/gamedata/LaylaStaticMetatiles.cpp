#include "gamedata/LaylaStaticMetatiles.h"
#include "util/StringConversion.h"

namespace Lonely {


LaylaStaticMetatiles::LaylaStaticMetatiles() { };
  
LaylaStaticMetatiles::LaylaStaticMetatiles(const NesRom& rom,
                const LaylaOffsetFile& offsets) {
  int numCave = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "NumCaveMetatiles"));
  Taddress caveEven = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "CaveEvenTilesTableOffset"));
  Taddress caveOdd = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "CaveOddTilesTableOffset"));
  Taddress caveAttribute = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "CaveAttributeTableOffset"));
  Taddress caveProperties = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "CavePropertiesTableOffset"));
  
  int numBoss = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "NumBossMetatiles"));
  Taddress bossEven = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "BossEvenTilesTableOffset"));
  Taddress bossOdd = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "BossOddTilesTableOffset"));
  Taddress bossAttribute = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "BossAttributeTableOffset"));
  Taddress bossProperties = StringConversion::stringToInt(
        offsets.ini().valueOfKey("MetatileDefinitionTableOffsets",
                                "BossPropertiesTableOffset"));
  
  caveMetatiles_ = LaylaStaticMetatileSet(rom,
                                    numCave,
                                    caveEven,
                                    caveOdd,
                                    caveAttribute,
                                    caveProperties);
  
  bossMetatiles_ = LaylaStaticMetatileSet(rom,
                                    numBoss,
                                    bossEven,
                                    bossOdd,
                                    bossAttribute,
                                    bossProperties);
}
  
void LaylaStaticMetatiles::exportToRom(NesRom& rom) const {
  caveMetatiles_.exportToRom(rom);
  bossMetatiles_.exportToRom(rom);
}
  
int LaylaStaticMetatiles::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LaylaStaticMetatiles,
                   0);
  
  caveMetatiles_.save(data);
  bossMetatiles_.save(data);
  
  return saver.finalize();
}
  
int LaylaStaticMetatiles::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);
  
  byteCount += caveMetatiles_.load(data + byteCount);
  byteCount += bossMetatiles_.load(data + byteCount);
  
  return byteCount;
}
  
LaylaStaticMetatileSet& LaylaStaticMetatiles::caveMetatiles() {
  return caveMetatiles_;
}

LaylaStaticMetatileSet& LaylaStaticMetatiles::bossMetatiles() {
  return bossMetatiles_;
}
  
const LaylaStaticMetatileSet& LaylaStaticMetatiles::caveMetatiles() const {
  return caveMetatiles_;
}

const LaylaStaticMetatileSet& LaylaStaticMetatiles::bossMetatiles() const {
  return bossMetatiles_;
}


};
