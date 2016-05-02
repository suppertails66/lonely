#ifndef LONELYMETATILECACHESET_H
#define LONELYMETATILECACHESET_H


#include "editor/LonelyMetatileCache.h"

namespace Lonely {


class LaylaMetatileSet;
class LaylaStaticMetatiles;
class LaylaGraphics;
class LaylaPalettes;

class LonelyMetatileCacheSet {
public:
  LonelyMetatileCacheSet();
  
  LonelyMetatileCacheSet(const LaylaMetatileSet& caveMetatiles,
                         const LaylaMetatileSet& baseMetatiles,
                         const LaylaMetatileSet& bossMetatiles,
                         const NesPatternTable& caveTiles,
                         const NesPatternTable& baseTiles,
                         const NesPatternTable& bossTiles,
                         const NesPaletteQuad& cavePalettes,
                         const NesPaletteQuad& basePalettes,
                         const NesPaletteQuad& bossPalettes);
  
  LonelyMetatileCache& caveCache();
  LonelyMetatileCache& baseCache();
  LonelyMetatileCache& bossCache();
  
  void cacheMetatiles(int levelNum,
                      LaylaMetatileSet& baseMetatiles,
                      LaylaStaticMetatiles& staticMetatiles,
                      LaylaGraphics& graphics,
                      LaylaPalettes& palettes);
  
  LonelyMetatileCache& cacheFromAreaType(Tbyte areaType);
                      
protected:
  LonelyMetatileCache caveCache_;
  LonelyMetatileCache baseCache_;
  LonelyMetatileCache bossCache_;
};


};


#endif
