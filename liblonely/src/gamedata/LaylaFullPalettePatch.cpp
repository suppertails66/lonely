#include "gamedata/LaylaFullPalettePatch.h"

namespace Lonely {


LaylaFullPalettePatch::LaylaFullPalettePatch() { };
  
LaylaPalettePatch& LaylaFullPalettePatch::backgroundPalettes() {
  return backgroundPalettes_;
}

LaylaPalettePatch& LaylaFullPalettePatch::spritePalettes() {
  return spritePalettes_;
}

const LaylaPalettePatch& LaylaFullPalettePatch::backgroundPalettes() const {
  return backgroundPalettes_;
}

const LaylaPalettePatch& LaylaFullPalettePatch::spritePalettes() const {
  return spritePalettes_;
}


}; 
