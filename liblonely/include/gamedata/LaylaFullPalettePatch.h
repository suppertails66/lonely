#ifndef LAYLAFULLPALETTEPATCH_H
#define LAYLAFULLPALETTEPATCH_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "gamedata/LaylaPalettePatch.h"
#include "nes/NesPaletteQuad.h"
#include "nes/NesColorData.h"
#include "nes/NesColor.h"

namespace Lonely {


class LaylaFullPalettePatch {
public:
  LaylaFullPalettePatch();
  
  LaylaPalettePatch& backgroundPalettes();
  LaylaPalettePatch& spritePalettes();
  
  const LaylaPalettePatch& backgroundPalettes() const;
  const LaylaPalettePatch& spritePalettes() const;
  
protected:
  LaylaPalettePatch backgroundPalettes_;
  LaylaPalettePatch spritePalettes_;
  
};


};


#endif
