#ifndef LAYLAGRAPHICS_H
#define LAYLAGRAPHICS_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tarray.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "nes/NesRom.h"
#include "nes/NesTile.h"
#include "nes/NesPatternTable.h"
#include "gamedata/LaylaOffsetFile.h"

namespace Lonely {


class LaylaGraphics {
public:
  LaylaGraphics();
  
  LaylaGraphics(const NesRom& rom,
                const LaylaOffsetFile& offsets);
  
  void exportToRom(NesRom& rom) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  NesPatternTable& caveSprites();
  NesPatternTable& caveBackground();
  NesPatternTable& baseSprites();
  NesPatternTable& baseBackground();
  NesPatternTable& bossSprites();
  NesPatternTable& bossBackground();
  NesPatternTable& titleSprites();
  NesPatternTable& titleBackground();
  
  const NesPatternTable& caveSprites() const;
  const NesPatternTable& caveBackground() const;
  const NesPatternTable& baseSprites() const;
  const NesPatternTable& baseBackground() const;
  const NesPatternTable& bossSprites() const;
  const NesPatternTable& bossBackground() const;
  const NesPatternTable& titleSprites() const;
  const NesPatternTable& titleBackground() const;
  
  NesPatternTable& areaBackground(Tbyte areaNum);
  NesPatternTable& areaSprites(Tbyte areaNum);
  
  const NesPatternTable& areaBackground(Tbyte areaNum) const;
  const NesPatternTable& areaSprites(Tbyte areaNum) const;
protected:
  const static char* offsetFileGraphicsName_;
  
  Taddress caveSpritesOffset_;
  Taddress caveBackgroundOffset_;
  Taddress baseSpritesOffset_;
  Taddress baseBackgroundOffset_;
  Taddress bossSpritesOffset_;
  Taddress bossBackgroundOffset_;
  Taddress titleSpritesOffset_;
  Taddress titleBackgroundOffset_;
  
  NesPatternTable caveSprites_;
  NesPatternTable caveBackground_;
  NesPatternTable baseSprites_;
  NesPatternTable baseBackground_;
  NesPatternTable bossSprites_;
  NesPatternTable bossBackground_;
  NesPatternTable titleSprites_;
  NesPatternTable titleBackground_;
  
  
};


};


#endif
