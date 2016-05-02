#ifndef LONELYLAYLAMAPPING_H
#define LONELYLAYLAMAPPING_H


#include "gamedata/LaylaMetatile.h"
#include "structs/Tbyte.h"
#include "structs/TwoDByteArray.h"
#include "structs/Graphic.h"
#include "nes/NesPaletteQuad.h"
#include "nes/NesPatternTable.h"

namespace Lonely {


class LonelyLaylaMapping {
public:
  LonelyLaylaMapping();
  
  LonelyLaylaMapping(const Tbyte* src);
  
  TwoDByteArray mapping() const;
  void setMapping(TwoDByteArray mapping__);
  
  void draw(Graphic& dst,
            const NesPatternTable& tiles,
            const NesPaletteQuad& palettes,
            bool spriteTransparency = true) const;
  
  void drawInvalid(Graphic& dst) const;
  
  int w() const;
  int h() const;
  
  const static int invalidMappingDrawW = LaylaMetatile::width;
  const static int invalidMappingDrawH = LaylaMetatile::height;
            
protected:
  TwoDByteArray mapping_;
};


};


#endif
