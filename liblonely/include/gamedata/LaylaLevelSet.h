#ifndef LAYLALEVELSET_H
#define LAYLALEVELSET_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "structs/Tarray.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "nes/NesRom.h"
#include "gamedata/LaylaOffsetFile.h"
#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaObjectSet.h"
#include "gamedata/LaylaObjectSets.h"

namespace Lonely {


class LaylaLevelSet {
public:
//  const static int numLevels = 8;
  
  LaylaLevelSet();
                
  LaylaLevelSet(const NesRom& rom,
                const LaylaOffsetFile& offsets);
  
  void exportToRom(NesRom& rom,
                   const LaylaObjectSetCollection& objectSets) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  LaylaLevel& level(int levelNum);
  
  // Adding/removing a metatile from a global set (cave or boss) invalidates
  // every pattern in every level, so we need a function here to fix things
  // up afterwards
  void fixAfterMetatileQuantityChange(
    Tbyte areaType,
    int numMetatiles,
    int index);
    
  int numLevels();
protected:
//  const static int numLevels_ = 8;
  int numLevels_;
  const static int bankConversionBase_ = 6;
  const static int numSets_ = LaylaObjectSets::numObjectSets;
  const static int levelsPerBank_ = 2;
  const static int exportBankOffset_ = 3;

//  LaylaLevel levels_[numLevels_];
  Tarray<LaylaLevel> levels_;
};


};


#endif
