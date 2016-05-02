#ifndef NESCOLORDATA_H
#define NESCOLORDATA_H


#include "nes/NesPaletteQuad.h"
#include "structs/Tbyte.h"

namespace Lonely {


class NesColorData {
public:
  const static int size = NesPaletteQuad::size * 2;
  const static int numBackgroundColors = 16;
  const static int numSpriteColors = 16;

  NesColorData();
  
  NesPaletteQuad& backgroundPalettes();
  NesPaletteQuad& spritePalettes();
  
  const NesPaletteQuad& backgroundPalettes() const;
  const NesPaletteQuad& spritePalettes() const;
  
  void readFullPaletteSet(const Tbyte* src);
  
  int readFromData(const Tbyte* src);
  int writeToData(Tbyte* dst) const;
  
  NesColor color(int index) const;
  void setColor(int index, NesColor color);
protected:
  NesPaletteQuad backgroundPalettes_;
  NesPaletteQuad spritePalettes_;
  
  void readPaletteQuad(NesPaletteQuad& dst,
                   const Tbyte* src);
};


};


#endif
