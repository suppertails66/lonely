#ifndef LAYLABOSS_H
#define LAYLABOSS_H


#include "structs/Tstring.h"

namespace Lonely {


class LaylaBosses {
public:
  const static int numBosses = 5;
  enum LaylaBoss {
    bamou       = 0x00,      // goblin boss
    esperdra    = 0x01,      // dragon boss
    batear      = 0x02,      // flying slug boss
    taratab     = 0x03,      // ceiling boss
    dinobares   = 0x04       // dinosaur boss
  };
  
  static Tstring nameOfBoss(LaylaBoss boss);
  
  static bool bossIsInObjectSet(LaylaBoss boss,
                                int objectSetNum);
protected:
  
};


};


#endif
