#include "editor/LonelyMetatileCacheSet.h"
#include "gamedata/LaylaMetatileSet.h"
#include "gamedata/LaylaStaticMetatiles.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaPatternToken.h"

namespace Lonely {


LonelyMetatileCacheSet::LonelyMetatileCacheSet() { };
  
LonelyMetatileCacheSet::LonelyMetatileCacheSet(
                       const LaylaMetatileSet& caveMetatiles,
                       const LaylaMetatileSet& baseMetatiles,
                       const LaylaMetatileSet& bossMetatiles,
                       const NesPatternTable& caveTiles,
                       const NesPatternTable& baseTiles,
                       const NesPatternTable& bossTiles,
                       const NesPaletteQuad& cavePalettes,
                       const NesPaletteQuad& basePalettes,
                       const NesPaletteQuad& bossPalettes)
  : caveCache_(caveMetatiles,
               caveTiles,
               cavePalettes),
    baseCache_(baseMetatiles,
               baseTiles,
               basePalettes),
    bossCache_(bossMetatiles,
               bossTiles,
               bossPalettes) { };

LonelyMetatileCache& LonelyMetatileCacheSet::caveCache() {
  return caveCache_;
}

LonelyMetatileCache& LonelyMetatileCacheSet::baseCache() {
  return baseCache_;
}

LonelyMetatileCache& LonelyMetatileCacheSet::bossCache() {
  return bossCache_;
}
  
void LonelyMetatileCacheSet::cacheMetatiles(
                    int levelNum,
                    LaylaMetatileSet& baseMetatiles,
                    LaylaStaticMetatiles& staticMetatiles,
                    LaylaGraphics& graphics,
                    LaylaPalettes& palettes) {
  
/*  (*this) = LonelyMetatileCacheSet(
    staticMetatiles.caveMetatiles().metatiles(),
    baseMetatiles,
    staticMetatiles.bossMetatiles().metatiles(),
    graphics.caveBackground(),
    graphics.baseBackground(),
    graphics.bossBackground(),
    palettes.generateCavePalette(levelNum).backgroundPalettes(),
    palettes.generateBasePalette(levelNum).backgroundPalettes(),
    palettes.generateBossPalette(levelNum).backgroundPalettes()); */
    
    caveCache_.cacheMetatiles(
      staticMetatiles.caveMetatiles().metatiles(),
      graphics.caveBackground(),
      palettes.generateCavePalette(levelNum).backgroundPalettes());
               
    baseCache_.cacheMetatiles(
      baseMetatiles,
      graphics.baseBackground(),
      palettes.generateBasePalette(levelNum).backgroundPalettes());
               
    bossCache_.cacheMetatiles(
      staticMetatiles.bossMetatiles().metatiles(),
      graphics.bossBackground(),
      palettes.generateBossPalette(levelNum).backgroundPalettes());
}
  
LonelyMetatileCache& LonelyMetatileCacheSet
  ::cacheFromAreaType(Tbyte areaType) {
  if (areaType & SetAreaToken::baseFlag) {
    return baseCache_;
  }
  else if (areaType & SetAreaToken::bossFlag) {
    return bossCache_;
  }
  else {
    return caveCache_;
  }
}


};
