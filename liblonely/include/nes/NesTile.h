#ifndef NESTILE_H
#define NESTILE_H


#include "structs/Tbyte.h"
#include "structs/TwoDByteArray.h"
#include "structs/Graphic.h"
#include "nes/NesPalette.h"

namespace Lonely {


class NesTile {
public:
  const static int width = 8;
  const static int height = 8;
  const static int size = 16;
  
  NesTile();
  
  int fromUncompressedData(const Tbyte* src);
  
  int toUncompressedData(Tbyte* dst) const;
  
  Tbyte data(int x, int y) const;
  
  void setData(int x, int y, Tbyte d);
  
  void drawGrayscale(Graphic& dst,
                     int xOffset, int yOffset,
                     bool transparency = false) const;
  
  void drawGrayscale(Graphic& dst,
                     bool transparency = false) const;
  
  void drawPalettized(Graphic& dst,
                      int xOffset, int yOffset,
                      const NesPalette& src,
                      bool transparency = false) const;
  
  void drawPalettized(Graphic& dst,
                      const NesPalette& src,
                      bool transparency = false) const;
  
  void swapColors(Tbyte first, Tbyte second);
protected:
  const static int planeByteOffset_ = 8;

  TwoDByteArray data_;
};


};


#endif
