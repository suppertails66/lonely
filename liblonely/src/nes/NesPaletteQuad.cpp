#include "nes/NesPaletteQuad.h"
#include "exception/OutOfRangeIndexException.h"
#include <iostream>

namespace Lonely {


NesPaletteQuad::NesPaletteQuad() { };
  
int NesPaletteQuad::numPalettes() const {
  return numPalettes_;
}

NesPalette& NesPaletteQuad::palette(int index) {
  if ((index < 0) || (index >= numPalettes_)) {
    throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                   "NesPaletteQuad::palette(int)",
                                   index);
  }
  
  return palettes_[index];
}

const NesPalette& NesPaletteQuad::palette(int index) const {
  if ((index < 0) || (index >= numPalettes_)) {
    throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                   "NesPaletteQuad::palette(int) const",
                                   index);
  }
  
  return palettes_[index];
}
  
NesColor NesPaletteQuad::color(int pos) const {
  int paletteIndex = (pos / numPalettes_);
  int colorIndex = (pos % numPalettes_);
  
  return palettes_[paletteIndex].color(colorIndex);
}

NesColor& NesPaletteQuad::colorRef(int pos) {
  int paletteIndex = (pos / numPalettes_);
  int colorIndex = (pos % numPalettes_);
  
  return palettes_[paletteIndex].colorRef(colorIndex);
}

void NesPaletteQuad::setColor(int pos, NesColor color) {
  int paletteIndex = (pos / numPalettes_);
  int colorIndex = (pos % numPalettes_);
  
  palettes_[paletteIndex].setColor(colorIndex, color);
}
  
int NesPaletteQuad::readFromData(const char* src) {
  for (int i = 0; i < size; i++) {
    setColor(i, NesColor(*(src++)));
  }

  return size;
}

int NesPaletteQuad::readFromData(const unsigned char* src) {
  return readFromData((const char*)(src));
}
  
int NesPaletteQuad::writeToData(char* dst) const {
  for (int i = 0; i < size; i++) {
    *(dst++) = color(i).nativeValue();
  }
  
  return size;
}

int NesPaletteQuad::writeToData(unsigned char* dst) const {
  return writeToData((char*)(dst));
}


};
