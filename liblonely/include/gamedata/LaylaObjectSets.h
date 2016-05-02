#ifndef LAYLAOBJECTSETS_H
#define LAYLAOBJECTSETS_H


#include "gamedata/LonelyLaylaObject.h"

namespace Lonely {


class LaylaObjectSets {
public:
  const static int numObjectSets = 4;
  const static int objectsPerSet = 0x10;

  static LonelyLaylaObjects::LonelyLaylaObject
      lonelyID(int setNum, int objNum);
      
  static int objectSetToLevelNum(int num);
protected:
  typedef LonelyLaylaObjects::LonelyLaylaObject
    LonelyLaylaObjectSetMapping[objectsPerSet];
    
  const static int objectToLevelMultiplier_ = 2;
    
  const static LonelyLaylaObjectSetMapping
      set0;
  const static LonelyLaylaObjectSetMapping
      set1;
  const static LonelyLaylaObjectSetMapping
      set2;
  const static LonelyLaylaObjectSetMapping
      set3;
};


};


#endif
