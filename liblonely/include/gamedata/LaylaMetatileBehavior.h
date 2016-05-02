#ifndef LAYLAMETATILEBEHAVIOR_H
#define LAYLAMETATILEBEHAVIOR_H


#include "structs/Tstring.h"

namespace Lonely {


class LaylaMetatileBehaviors {
public:
  const static int numMetatileBehaviors = 0x10;
  
  enum LaylaMetatileBehavior {
    none                    = 0x0,
    falling                 = 0x1,
    elevator                = 0x2,
    concealedEmpty          = 0x3,
    concealedSkate          = 0x4,
    concealedPistol         = 0x5,
    concealedMachineGun     = 0x6,
    concealedGrenade        = 0x7,
    concealedFlamethrower   = 0x8,
    concealedAxe            = 0x9,
    concealedBazooka        = 0xA,
    concealedSaber          = 0xB,
    concealedShield         = 0xC,
    concealedSpecial        = 0xD,
    destructible            = 0xE,
    vanishing               = 0xF
  };
  
  static Tstring nameOfBehavior(LaylaMetatileBehavior behavior);
protected:
  
};


};


#endif
