#include "gamedata/LaylaPalettePatch.h"
#include <iostream>

namespace Lonely {


LaylaPalettePatch::LaylaPalettePatch() {
  for (int i = 0; i < NesPaletteQuad::size; i++) {
    colorsEnabled_[i] = false;
  }
}
  
bool LaylaPalettePatch::colorEnabled(int pos) const {
  return colorsEnabled_[pos];
}

void LaylaPalettePatch::setColorEnabled(int pos, bool enabled) {
  colorsEnabled_[pos] = enabled;
}

NesColor LaylaPalettePatch::color(int pos) const {
  return colors_.color(pos);
}

NesColor& LaylaPalettePatch::colorRef(int pos) {
  return colors_.colorRef(pos);
}

void LaylaPalettePatch::setColor(int pos, NesColor color) {
  colors_.setColor(pos, color);
}
  
void LaylaPalettePatch::setAndEnableColor(int pos, NesColor color) {
  setColor(pos, color);
  setColorEnabled(pos, true);
}
  
int LaylaPalettePatch::save(Tstring& data) const {
  int byteCount = 0;
  
  Tbyte buffer[NesPaletteQuad::size];
  
  byteCount += colors_.writeToData(buffer);
  data += Tstring((char*)(buffer), NesPaletteQuad::size);
  
//  std::cout << "out: " << std::endl;
  for (int i = 0; i < NesPaletteQuad::size; i++) {
    buffer[i] = (Tbyte)(colorsEnabled_[i]);
//    std::cout << colorsEnabled_[i] << " " << (int)(buffer[i]) << std::endl;
  }
  byteCount += NesPaletteQuad::size;
  data += Tstring((char*)(buffer), NesPaletteQuad::size);
  
  return byteCount;
}

int LaylaPalettePatch::load(const Tbyte* data) {
  int byteCount = 0;
  
  byteCount += colors_.readFromData(data + byteCount);
  
//  std::cout << "in: " << std::endl;
  for (int i = 0; i < NesPaletteQuad::size; i++) {
//    std::cout << (bool)(*(data + byteCount + i)) << " "
//      << colorsEnabled_[i] << std::endl;
    colorsEnabled_[i] = (bool)(*(data + byteCount + i));
  }
  byteCount += NesPaletteQuad::size;
  
  return byteCount;
}
  
void LaylaPalettePatch::apply(NesPaletteQuad& dst) const {
  for (int i = 0; i < NesPaletteQuad::size; i++) {
    if (colorsEnabled_[i]) {
      dst.setColor(i, colors_.color(i));
    }
  }
}
  
int LaylaPalettePatch::numColorsPerPatch() {
  return NesPaletteQuad::size;
}


};
