#ifndef LAYLAMETATILE_H
#define LAYLAMETATILE_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "structs/Graphic.h"
#include "nes/NesPaletteQuad.h"
#include "nes/NesPatternTable.h"
#include "gamedata/LaylaMetatileBehavior.h"

namespace Lonely {


class LaylaMetatile {
public:
  const static int width = 16;
  const static int height = 16;
  const static int widthInTiles = 2;
  const static int heightInTiles = 2;
  
  // Integral identifiers for each subtile for indexing purposes
  const static int ulIndex = 0;
  const static int urIndex = 1;
  const static int llIndex = 2;
  const static int lrIndex = 3;

  LaylaMetatile();
  
  void setProperties(Tbyte properties);
  Tbyte packProperties() const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  void drawVisual(Graphic& dst,
                  const NesPatternTable& tiles,
                  const NesPaletteQuad& palettes) const;
  
  void drawBehavior(Graphic& dst,
                  const NesPatternTable& tiles,
                  const NesPaletteQuad& palettes) const;
  
  void drawSolidity(Graphic& dst,
                  const NesPatternTable& tiles,
                  const NesPaletteQuad& palettes) const;
  
  static void drawSubtile(Graphic& dst,
                   int x,
                   int y,
                   const NesTile& tile,
                   const NesPalette& nesPalette);
  
  static void drawFourSubtiles(Graphic& dst,
                   Tbyte ul,
                   Tbyte ur,
                   Tbyte ll,
                   Tbyte lr,
                   const NesPatternTable& tiles,
                   const NesPalette& nesPalette);
  
  Tbyte& tileOfIndex(int index);
  
  Tbyte upperLeft;
  Tbyte upperRight;
  Tbyte lowerLeft;
  Tbyte lowerRight;
  
  Tbyte palette;
  
  bool bottomSolid;
  bool topSolid;
  LaylaMetatileBehaviors::LaylaMetatileBehavior behavior;
  bool behaviorFlag2;
  bool behaviorFlag3;
                   
protected:
  const static Tbyte topSolidMask_ = 0x01;
  const static Tbyte bottomSolidMask_ = 0x02;
  const static Tbyte behaviorFlag2Mask_ = 0x04;
  const static Tbyte behaviorFlag3Mask_ = 0x08;
  const static Tbyte behaviorMask_ = 0xF0;
  const static Tbyte behaviorShift_ = 4;
};


};


#endif
