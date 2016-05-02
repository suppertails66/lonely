#include "nes/NesColorData.h"

namespace Lonely {


NesColorData::NesColorData() { };

NesPaletteQuad& NesColorData::backgroundPalettes() {
  return backgroundPalettes_;
}

NesPaletteQuad& NesColorData::spritePalettes() {
  return spritePalettes_;
}

const NesPaletteQuad& NesColorData::backgroundPalettes() const {
  return backgroundPalettes_;
}

const NesPaletteQuad& NesColorData::spritePalettes() const {
  return spritePalettes_;
}
  
void NesColorData::readFullPaletteSet(const Tbyte* src) {
  // Backgrounds
  readPaletteQuad(backgroundPalettes_, src);
  src += NesPaletteQuad::size;
  
  // Sprites
  readPaletteQuad(spritePalettes_, src);
  src += NesPaletteQuad::size;
}
  
int NesColorData::readFromData(const Tbyte* src) {
  readFullPaletteSet(src);
  
  return size;
}

int NesColorData::writeToData(Tbyte* dst) const {
  dst += backgroundPalettes_.writeToData(dst);
  dst += spritePalettes_.writeToData(dst);
  
  return size;
}
  
void NesColorData::readPaletteQuad(NesPaletteQuad& dst,
                 const Tbyte* src) {
  for (int i = 0; i < dst.numPalettes(); i++) {
    NesPalette& pal = dst.palette(i);
    for (int j = 0; j < pal.numColors(); j++) {
      pal.setColor(j, *(src++));
    }
  }
}
  
NesColor NesColorData::color(int index) const {
  if (index < NesPaletteQuad::size) {
    return backgroundPalettes_.color(index);
  }
  else {
    return spritePalettes_.color(index - NesPaletteQuad::size);
  }
}

void NesColorData::setColor(int index, NesColor color) {
  if (index < NesPaletteQuad::size) {
    backgroundPalettes_.setColor(index, color);
  }
  else {
    spritePalettes_.setColor(index - NesPaletteQuad::size, color);
  }
}


};
