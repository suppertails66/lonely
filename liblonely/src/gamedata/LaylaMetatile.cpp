#include "gamedata/LaylaMetatile.h"

namespace Lonely {


LaylaMetatile::LaylaMetatile()
  : upperLeft(0),
    upperRight(0),
    lowerLeft(0),
    lowerRight(0),
    palette(0),
    bottomSolid(false),
    topSolid(false),
    behavior(LaylaMetatileBehaviors::none),
    behaviorFlag2(false),
    behaviorFlag3(false) { };
  
void LaylaMetatile::setProperties(Tbyte properties) {
  bottomSolid = (properties & bottomSolidMask_) != 0;
  topSolid = (properties & topSolidMask_) != 0;
  behaviorFlag2 = (properties & behaviorFlag2Mask_) != 0;
  behaviorFlag3 = (properties & behaviorFlag3Mask_) != 0;
  behavior = static_cast<LaylaMetatileBehaviors::LaylaMetatileBehavior>
                ((properties & behaviorMask_) >> behaviorShift_);
}
  
int LaylaMetatile::save(Tstring& data) const {
  int byteCount = 0;
  
  byteCount += SaveHelper::saveInt(data, upperLeft,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, upperRight,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, lowerLeft,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, lowerRight,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, palette,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, bottomSolid,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, topSolid,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, behavior,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, behaviorFlag2,
                                   ByteSizes::uint8Size);
  byteCount += SaveHelper::saveInt(data, behaviorFlag3,
                                   ByteSizes::uint8Size);
  
  return byteCount;
}

int LaylaMetatile::load(const Tbyte* data) {
  int byteCount = 0;
  
  upperLeft = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  upperRight = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  lowerLeft = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  lowerRight = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  palette = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  bottomSolid = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  topSolid = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  behavior = static_cast<LaylaMetatileBehaviors::LaylaMetatileBehavior>
                      (LoadHelper::loadInt(data, byteCount,
                          ByteSizes::uint8Size));
  behaviorFlag2 = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  behaviorFlag3 = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  
  return byteCount;
}

Tbyte LaylaMetatile::packProperties() const {
  Tbyte packed = 0;
  
  if (bottomSolid) {
    packed |= bottomSolidMask_;
  }
  
  if (topSolid) {
    packed |= topSolidMask_;
  }
  
  if (behaviorFlag2) {
    packed |= behaviorFlag2Mask_;
  }
  
  if (behaviorFlag3) {
    packed |= behaviorFlag3Mask_;
  }
  
  packed |= (behavior << behaviorShift_);
  
  return packed;
}
  
void LaylaMetatile::drawVisual(Graphic& dst,
                const NesPatternTable& tiles,
                const NesPaletteQuad& palettes) const {
  dst.resize(width, height);
  dst.clear();
  
  const NesPalette& nesPalette = palettes.palette(palette);
  
  drawFourSubtiles(dst, upperLeft, upperRight, lowerLeft, lowerRight,
                   tiles,
                   nesPalette);
}
  
void LaylaMetatile::drawBehavior(Graphic& dst,
                  const NesPatternTable& tiles,
                  const NesPaletteQuad& palettes) const {
  dst.resize(width, height);
  dst.clear();
  
  switch (behavior) {
  case LaylaMetatileBehaviors::none:
    {
    dst.clear(Tcolor(0xFF, 0xFF, 0xFF, Tcolor::fullAlphaOpacity));
    
    Tcolor fill = Tcolor(0x00, 0x00, 0x00, Tcolor::fullAlphaOpacity);
    // damaging
    if (behaviorFlag2 && behaviorFlag3) {
      fill = Tcolor(0xFF, 0x00, 0x00, Tcolor::fullAlphaOpacity);
    }
    
    if (bottomSolid) {
      dst.fillRect(0, 8,
                   16, 8,
                   fill);
    }
    
    if (topSolid) {
      dst.fillRect(0, 0,
                   16, 8,
                   fill);
    }
    }
    break;
  case LaylaMetatileBehaviors::falling:
    dst.clear(Tcolor(0xFF, 0x44, 0x44, Tcolor::fullAlphaOpacity));
    dst.drawLine(8, 0,
                 8, 16,
                 Tcolor(0x00, 0x00, 0xFF, Tcolor::fullAlphaOpacity),
                 1);
    dst.drawLine(8, 16,
                 0, 8,
                 Tcolor(0x00, 0x00, 0xFF, Tcolor::fullAlphaOpacity),
                 1);
    dst.drawLine(8, 16,
                 16, 8,
                 Tcolor(0x00, 0x00, 0xFF, Tcolor::fullAlphaOpacity),
                 1);
    break;
  case LaylaMetatileBehaviors::elevator:
    // Warp elevator
    if (behaviorFlag3) {
      dst.clear(Tcolor(0x00, 0x00, 0xFF, Tcolor::fullAlphaOpacity));
      dst.drawLine(8, 0,
                   8, 16,
                   Tcolor(0x00, 0xFF, 0x00, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 16,
                   2, 10,
                   Tcolor(0x00, 0xFF, 0x00, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 16,
                   14, 10,
                   Tcolor(0x00, 0xFF, 0x00, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 0,
                   2, 6,
                   Tcolor(0x00, 0xFF, 0x00, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 0,
                   14, 6,
                   Tcolor(0x00, 0xFF, 0x00, Tcolor::fullAlphaOpacity),
                   1);
    }
    // "Local" elevator
    else {
      dst.clear(Tcolor(0x88, 0x88, 0xFF, Tcolor::fullAlphaOpacity));
      dst.drawLine(8, 4,
                   8, 12,
                   Tcolor(0x88, 0xFF, 0x88, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 12,
                   4, 10,
                   Tcolor(0x88, 0xFF, 0x88, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 12,
                   12, 10,
                   Tcolor(0x88, 0xFF, 0x88, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 4,
                   4, 6,
                   Tcolor(0x88, 0xFF, 0x88, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(8, 4,
                   12, 6,
                   Tcolor(0x88, 0xFF, 0x88, Tcolor::fullAlphaOpacity),
                   1);
    }
    break;
  case LaylaMetatileBehaviors::concealedEmpty:
    dst.clear(Tcolor(0x88, 0x88, 0x88, Tcolor::fullAlphaOpacity));
    break;
  case LaylaMetatileBehaviors::concealedSkate:
    drawFourSubtiles(dst, 0x36, 0x46, 0x37, 0x47,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedPistol:
    drawFourSubtiles(dst, 0xE2, 0xF2, 0xE3, 0xF3,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedMachineGun:
    drawFourSubtiles(dst, 0xE4, 0xF4, 0xE5, 0xF5,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedGrenade:
    drawFourSubtiles(dst, 0xE0, 0xF0, 0xE1, 0xF1,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedFlamethrower:
    drawFourSubtiles(dst, 0xEA, 0xFA, 0xEB, 0xFB,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedAxe:
    drawFourSubtiles(dst, 0xE8, 0xF8, 0xE9, 0xF9,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedBazooka:
    drawFourSubtiles(dst, 0xE6, 0xF6, 0xE7, 0xF7,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedSaber:
    drawFourSubtiles(dst, 0xEC, 0xFC, 0xED, 0xFD,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedShield:
    drawFourSubtiles(dst, 0xEE, 0xFE, 0xEF, 0xFF,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::concealedSpecial:
    drawFourSubtiles(dst, 0x38, 0x48, 0x39, 0x49,
                     tiles,
                     palettes.palette(0));
    break;
  case LaylaMetatileBehaviors::destructible:
    dst.clear(Tcolor(0x00, 0xFF, 0x00, Tcolor::fullAlphaOpacity));
    // Damaging
    if (behaviorFlag2 && behaviorFlag3) {
      dst.drawLine(0, 0,
                   16, 16,
                   Tcolor(0xFF, 0x00, 0x00, Tcolor::fullAlphaOpacity),
                   1);
      dst.drawLine(16, 0,
                   0, 16,
                   Tcolor(0xFF, 0x00, 0x00, Tcolor::fullAlphaOpacity),
                   1);
    }
    break;
  case LaylaMetatileBehaviors::vanishing:
//    dst.clear(Tcolor(0x00, 0xFF, 0x00, Tcolor::fullAlphaOpacity));
    for (int j = 0; j < height; j += 2) {
      for (int i = 0; i < width; i += 2) {
        if (j % 4) {
          if (!(i % 4)) {
            continue;
          }
        }
        else {
          if ((i % 4)) {
            continue;
          }
        }
        
        dst.fillRect(i, j,
                2, 2,
                Tcolor(0x00, 0x00, 0x00, Tcolor::fullAlphaOpacity));
      }
    }
    break;
  default:
    
    break;
  }
}

void LaylaMetatile::drawSolidity(Graphic& dst,
                  const NesPatternTable& tiles,
                  const NesPaletteQuad& palettes) const {
  dst.resize(width, height);
  dst.clear();
  
//  dst.clear(Tcolor(0xFF, 0xFF, 0xFF, Tcolor::fullAlphaOpacity));
  
  Tcolor fill = Tcolor(0x00, 0x00, 0x00, Tcolor::fullAlphaOpacity);
  // damaging
//  if (behaviorFlag2 && behaviorFlag3) {
//    fill = Tcolor(0xFF, 0x00, 0x00, Tcolor::fullAlphaOpacity);
//  }
  
  if (bottomSolid) {
    dst.fillRect(0, 8,
                 16, 8,
                 fill);
  }
  
  if (topSolid) {
    dst.fillRect(0, 0,
                 16, 8,
                 fill);
  }
}
  
void LaylaMetatile::drawSubtile(Graphic& dst,
                 int x,
                 int y,
                 const NesTile& tile,
                 const NesPalette& nesPalette) {
  Graphic tileGraphic(NesTile::width, NesTile::height);
  tile.drawPalettized(tileGraphic, nesPalette, false);
  dst.copy(tileGraphic,
           Box(x, y, 0, 0));
}
  
void LaylaMetatile::drawFourSubtiles(Graphic& dst,
                 Tbyte ul,
                 Tbyte ur,
                 Tbyte ll,
                 Tbyte lr,
                 const NesPatternTable& tiles,
                 const NesPalette& nesPalette) {
  drawSubtile(dst,
              0, 0,
              tiles.tile(ul),
              nesPalette);
  drawSubtile(dst,
              NesTile::width, 0,
              tiles.tile(ur),
              nesPalette);
  drawSubtile(dst,
              0, NesTile::height,
              tiles.tile(ll),
              nesPalette);
  drawSubtile(dst,
              NesTile::width, NesTile::height,
              tiles.tile(lr),
              nesPalette);
}
  
Tbyte& LaylaMetatile::tileOfIndex(int index) {
  switch (index) {
  case ulIndex:
    return upperLeft;
    break;
  case urIndex:
    return upperRight;
    break;
  case llIndex:
    return lowerLeft;
    break;
  case lrIndex:
  default:
    return lowerRight;
    break;
  }
}


};
