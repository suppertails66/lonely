#ifndef LONELYMETATILECACHE_H
#define LONELYMETATILECACHE_H


#include "gamedata/LaylaMetatileSet.h"
#include "nes/NesPatternTable.h"
#include "nes/NesPaletteQuad.h"
#include "structs/Graphic.h"
#include "structs/Tarray.h"

namespace Lonely {


typedef Tarray<Graphic> MetatileCacheCollection;
  
class LonelyMetatileCache {
public:
  LonelyMetatileCache();
                      
  LonelyMetatileCache(const LaylaMetatileSet& metatiles,
                      const NesPatternTable& tiles,
                      const NesPaletteQuad& palettes);
  
  int numMetatiles() const;
  
  Graphic& metatileVisual(int index);
  Graphic& metatileBehavior(int index);
  Graphic& metatileSolidity(int index);
  const Graphic& metatileVisual(int index) const;
  const Graphic& metatileBehavior(int index) const;
  const Graphic& metatileSolidity(int index) const;
  
  MetatileCacheCollection& metatilesVisual();
  MetatileCacheCollection& metatilesBehavior();
  MetatileCacheCollection& metatilesSolidity();
  const MetatileCacheCollection& metatilesVisual() const;
  const MetatileCacheCollection& metatilesBehavior() const;
  const MetatileCacheCollection& metatilesSolidity() const;
  
  void cacheMetatiles(const LaylaMetatileSet& metatiles,
                      const NesPatternTable& tiles,
                      const NesPaletteQuad& palettes);
protected:
  
  MetatileCacheCollection metatilesVisual_;
  MetatileCacheCollection metatilesBehavior_;
  MetatileCacheCollection metatilesSolidity_;
};


};


#endif
