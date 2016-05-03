#include "nes/NesTile.h"
#include <iostream>

namespace Lonely {


NesTile::NesTile()
  : data_(width, height) { };
  
Tbyte NesTile::data(int x, int y) const {
  return data_.data(x, y);
}
  
void NesTile::setData(int x, int y, Tbyte d) {
  data_.setDataClipped(x, y, d);
}
  
int NesTile::fromUncompressedData(const Tbyte* src) {

  for (int j = 0; j < height; j++) {
    // Row data from plane 1 (low bit)
    Tbyte b1 = *src;
    // Row data from plane 2 (high bit)
    Tbyte b2 = *(src + planeByteOffset_);
    
    for (int i = 0; i < width; i++) {
      Tbyte d = 0;
      
      int shift = (width - i - 1);
      
      Tbyte mask = (0x01 << shift);
      
      // Extract bits from planes and combine into a single byte
      d |= (b1 & mask) >> shift;
      
      // negative shift widths aren't supported? really?
      if (shift > 0) {
        d |= (b2 & mask) >> (shift - 1);
      }
      else {
        d |= (b2 & mask) << -(shift - 1);
      }
      
      data_.data(i, j) = d;
    }
    
    ++src;
  }
  
  return size;
}
  
int NesTile::toUncompressedData(Tbyte* dst) const {

  for (int j = 0; j < height; j++) {
    // Row data for plane 1 (low bit)
    Tbyte* b1 = dst;
    // Row data for plane 2 (high bit)
    Tbyte* b2 = dst + planeByteOffset_;
    
    (*b1) = 0;
    (*b2) = 0;
    
    for (int i = 0; i < width; i++) {
      Tbyte d = data_.data(i, j);
      
      int shift = (width - i - 1);
      
      Tbyte mask = (0x01 << shift);
      
      if ((d & 0x01) != 0) {
        (*b1) |= mask;
      }
      
      if ((d & 0x02) != 0) {
        (*b2) |= mask;
      }
    }
    
    ++dst;
  }
  
  return size;
}
  
void NesTile::drawGrayscale(Graphic& dst,
                            int xOffset, int yOffset,
                            bool transparency) const {
//  dst.clear(Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity));
  
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      Tbyte c = data_.data(i, j) << 6;
      
      Tcolor realcolor(c, c, c, Tcolor::fullAlphaOpacity);
      
      if (transparency && (c == 0)) {
        realcolor.setA(Tcolor::fullAlphaTransparency);
      }
      
      // background?
      // ...
      
      dst.setPixel(i + xOffset, j + yOffset,
                   realcolor);
    }
  }
}
  
void NesTile::drawGrayscale(Graphic& dst,
                            bool transparency) const {
  if ((dst.w() != width) || (dst.h() != height)) {
    dst = Graphic(width, height);
  }
  
  drawGrayscale(dst, 0, 0, transparency);
}
  
void NesTile::drawPalettized(Graphic& dst,
                    int xOffset, int yOffset,
                    const NesPalette& src,
                    bool transparency) const {
//  dst.clear(Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity));
  
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      Tbyte index = data_.data(i, j);
      NesColor color = src.color(index);
      Tcolor realcolor = color.realColor();
      
//      realcolor.setA(Tcolor::fullAlphaOpacity);
      if (transparency && (index == 0)) {
        realcolor.setA(Tcolor::fullAlphaTransparency);
      }
      
      // background?
      // ...
      
      dst.setPixel(i + xOffset, j + yOffset,
                   realcolor);
    }
  }
}
  
void NesTile::drawPalettized(Graphic& dst,
                    const NesPalette& src,
                    bool transparency) const {
  if ((dst.w() != width) || (dst.h() != height)) {
    dst = Graphic(width, height);
  }
  
//  dst.clear(Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity));
  
  drawPalettized(dst, 0, 0, src, transparency);
}
  
void NesTile::swapColors(Tbyte first, Tbyte second) {
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      if (data(i, j) == first) {
        setData(i, j, second);
      }
      else if (data(i, j) == second) {
        setData(i, j, first);
      }
    }
  }
}


};
