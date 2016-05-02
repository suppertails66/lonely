#ifndef LAYLASTATICMETATILESET_H
#define LAYLASTATICMETATILESET_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "gamedata/LaylaMetatileSet.h"

namespace Lonely {


class LaylaStaticMetatileSet {
public:
  LaylaStaticMetatileSet();
  
  LaylaStaticMetatileSet(const NesRom& rom,
                   int numMetatiles__,
                   Taddress evenTilesTable__,
                   Taddress oddTilesTable__,
                   Taddress attributesTable__,
                   Taddress propertiesTable__);
  
  void exportToRom(NesRom& rom) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  LaylaMetatileSet& metatiles();
protected:
  int numMetatiles_;
  Taddress evenTilesTable_;
  Taddress oddTilesTable_;
  Taddress attributesTable_;
  Taddress propertiesTable_;
  
  LaylaMetatileSet metatiles_;
  
};


};


#endif
