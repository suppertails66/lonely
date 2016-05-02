#ifndef LAYLAHPARRAY_H
#define LAYLAHPARRAY_H


#include "gamedata/LaylaObjectSets.h"
#include "structs/Tbyte.h"
#include "structs/Tstring.h"

namespace Lonely {


class LaylaHpArray {
public:
  const static int numEntries = LaylaObjectSets::objectsPerSet;
  
  LaylaHpArray();
  
  int readFromData(const Tbyte* src);
  
  int writeToData(Tbyte* dst) const;
  
  int save(Tstring& dst) const;
  
  int load(const Tbyte* src);
  
  Tbyte& hp(int num);
  
  const Tbyte& hp(int num) const;
protected:
  Tbyte hps_[numEntries];
  
};


};


#endif
