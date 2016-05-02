#ifndef LAYLASTATICMETATILES_H
#define LAYLASTATICMETATILES_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "nes/NesRom.h"
#include "gamedata/LaylaOffsetFile.h"
#include "gamedata/LaylaStaticMetatileSet.h"

namespace Lonely {


class LaylaStaticMetatiles {
public:
  LaylaStaticMetatiles();
  
  LaylaStaticMetatiles(const NesRom& rom,
                const LaylaOffsetFile& offsets);
  
  void exportToRom(NesRom& rom) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  LaylaStaticMetatileSet& caveMetatiles();
  LaylaStaticMetatileSet& bossMetatiles();
  
  const LaylaStaticMetatileSet& caveMetatiles() const;
  const LaylaStaticMetatileSet& bossMetatiles() const;
protected:
  LaylaStaticMetatileSet caveMetatiles_;
  LaylaStaticMetatileSet bossMetatiles_;
};


};


#endif
