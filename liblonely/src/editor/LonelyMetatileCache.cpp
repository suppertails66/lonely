#include "editor/LonelyMetatileCache.h"
#include <iostream>

namespace Lonely {


LonelyMetatileCache::LonelyMetatileCache() { };
  
LonelyMetatileCache::LonelyMetatileCache(const LaylaMetatileSet& metatiles,
                      const NesPatternTable& tiles,
                      const NesPaletteQuad& palettes) {
//  std::cout << metatiles.size() << std::endl;
  cacheMetatiles(metatiles, tiles, palettes);
}
  
int LonelyMetatileCache::numMetatiles() const {
  return metatilesVisual_.size();
}

Graphic& LonelyMetatileCache::metatileVisual(int index) {
  return metatilesVisual_[index];
}

Graphic& LonelyMetatileCache::metatileBehavior(int index) {
  return metatilesBehavior_[index];
}

Graphic& LonelyMetatileCache::metatileSolidity(int index) {
  return metatilesSolidity_[index];
}

const Graphic& LonelyMetatileCache::metatileVisual(int index) const {
  return metatilesVisual_[index];
}

const Graphic& LonelyMetatileCache::metatileBehavior(int index) const {
  return metatilesBehavior_[index];
}

const Graphic& LonelyMetatileCache::metatileSolidity(int index) const {
  return metatilesSolidity_[index];
}

MetatileCacheCollection& LonelyMetatileCache::metatilesVisual() {
  return metatilesVisual_;
}

MetatileCacheCollection& LonelyMetatileCache::metatilesBehavior() {
  return metatilesBehavior_;
}

MetatileCacheCollection& LonelyMetatileCache::metatilesSolidity() {
  return metatilesSolidity_;
}

const MetatileCacheCollection& LonelyMetatileCache::metatilesVisual() const {
  return metatilesVisual_;
}

const MetatileCacheCollection& LonelyMetatileCache::metatilesBehavior() const {
  return metatilesBehavior_;
}

const MetatileCacheCollection& LonelyMetatileCache::metatilesSolidity() const {
  return metatilesSolidity_;
}
  
void LonelyMetatileCache::cacheMetatiles(const LaylaMetatileSet& metatiles,
                    const NesPatternTable& tiles,
                    const NesPaletteQuad& palettes) {
  metatilesVisual_.resize(metatiles.size());
  metatilesBehavior_.resize(metatiles.size());
  metatilesSolidity_.resize(metatiles.size());
  for (int i = 0; i < metatiles.size(); i++) {
    Graphic g;
    metatiles.metatile(i).drawVisual(g,
                             tiles,
                             palettes);
    metatilesVisual_[i] = g;
    
    metatiles.metatile(i).drawBehavior(g,
                             tiles,
                             palettes);
    metatilesBehavior_[i] = g;
    
    metatiles.metatile(i).drawSolidity(g,
                             tiles,
                             palettes);
    metatilesSolidity_[i] = g;
  }
}


};
