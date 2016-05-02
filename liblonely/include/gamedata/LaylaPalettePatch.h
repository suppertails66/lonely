#ifndef LAYLAPALETTEPATCH_H
#define LAYLAPALETTEPATCH_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "nes/NesPaletteQuad.h"
#include "nes/NesColorData.h"
#include "nes/NesColor.h"

namespace Lonely {


class LaylaPalettePatch {
public:
  LaylaPalettePatch();
  
  bool colorEnabled(int pos) const;
  void setColorEnabled(int pos, bool enabled);
  
  NesColor color(int pos) const;
  NesColor& colorRef(int pos);
  void setColor(int pos, NesColor color);
  
  void setAndEnableColor(int pos, NesColor color);
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  void apply(NesPaletteQuad& dst) const;
  
  static int numColorsPerPatch();
  
protected:
  NesPaletteQuad colors_;
  bool colorsEnabled_[NesPaletteQuad::size];
  
};


};


#endif
