#ifndef NESPALETTEQUAD_H
#define NESPALETTEQUAD_H


#include "nes/NesPalette.h"

namespace Lonely {


/**
 * A group of four NesPalettes.
 */
class NesPaletteQuad {
public:
  const static int size = NesPalette::size * 4;

  NesPaletteQuad();
  
  int numPalettes() const;
  
  NesPalette& palette(int index);
  const NesPalette& palette(int index) const;
  
  NesColor color(int pos) const;
  NesColor& colorRef(int pos);
  void setColor(int pos, NesColor color);
  
  int readFromData(const char* src);
  int readFromData(const unsigned char* src);
  
  int writeToData(char* dst) const;
  int writeToData(unsigned char* dst) const;
  
protected:
  const static int numPalettes_ = 4;
  
  NesPalette palettes_[numPalettes_];
  
};


};


#endif
