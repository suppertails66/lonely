#include "gamedata/LonelyLaylaMapping.h"
#include "gamedata/LaylaMetatile.h"
#include "util/StringConversion.h"
#include <iostream>

namespace Lonely {


LonelyLaylaMapping::LonelyLaylaMapping() { };

LonelyLaylaMapping::LonelyLaylaMapping(const Tbyte* src) {
  int w = (*(src++)) * 2;
  int h = (*(src++));
  
  mapping_.resize(w, h);
  
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i += 2) {
      mapping_.data(i, j) = *(src++);
      mapping_.data(i + 1, j) = *(src++);
    }
  }
}

TwoDByteArray LonelyLaylaMapping::mapping() const {
  return mapping_;
}

void LonelyLaylaMapping::setMapping(TwoDByteArray mapping__) {
  mapping_ = mapping__;
}
  
void LonelyLaylaMapping::draw(Graphic& dst,
          const NesPatternTable& tiles,
          const NesPaletteQuad& palettes,
          bool spriteTransparency) const {
  if ((mapping_.w() == 0)
      || (mapping_.h() == 0)) {
    drawInvalid(dst);
    return;
  }
          
  dst = Graphic(mapping_.w() / 2 * NesTile::width,
                mapping_.h() * NesTile::height);
  dst.clear(Tcolor(255, 255, 255, Tcolor::fullAlphaOpacity));
  
  for (int j = 0; j < mapping_.h(); j++) {
    for (int i = 0; i < mapping_.w(); i += 2) {
      Tbyte index = mapping_.data(i, j);
      Tbyte attributes = mapping_.data(i + 1, j);
      
//      std::cout << StringConversion::intToString(
//                      (int)index,
//                      StringConversion::baseHex) << std::endl;
      
      int palette = attributes & 0x03;
      
      Graphic tile;
      tiles.tile(index).drawPalettized(tile,
                                       palettes.palette(palette),
                                       spriteTransparency);
      
      if (attributes & 0x40) {
        tile.flipHorizontal();
      }
      
      if (attributes & 0x80) {
        tile.flipVertical();
      }
      
      dst.copy(tile,
               Box(i / 2  * NesTile::width,
                   j  * NesTile::height,
                   NesTile::width,
                   NesTile::height));
    }
  }
}
  
void LonelyLaylaMapping::drawInvalid(Graphic& dst) const {
  dst = Graphic(LaylaMetatile::width, LaylaMetatile::height);
  dst.clear();
  dst.drawLine(3, 3,
                  LaylaMetatile::width - 1, LaylaMetatile::height - 1,
                  Tcolor(0xFF, 0x00, 0x00),
                  2,
                  Graphic::transUpdate);
  dst.drawLine(LaylaMetatile::width - 1, 1,
                  4, LaylaMetatile::height - 4,
                  Tcolor(0xFF, 0x00, 0x00),
                  2,
                  Graphic::transUpdate);
}

int LonelyLaylaMapping::w() const {
  return mapping_.w() / 2;
}

int LonelyLaylaMapping::h() const {
  return mapping_.h();
}


};
