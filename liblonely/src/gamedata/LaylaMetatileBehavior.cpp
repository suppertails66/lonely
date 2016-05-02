#include "gamedata/LaylaMetatileBehavior.h"

namespace Lonely {


Tstring LaylaMetatileBehaviors
  ::nameOfBehavior(LaylaMetatileBehavior behavior) {
  switch (behavior) {
  case none:
    return "None";
    break;
  case falling:
    return "Falling object";
    break;
  case elevator:
    return "Elevator";
    break;
  case concealedEmpty:
    return "Crate: empty";
    break;
  case concealedSkate:
    return "Crate: skate";
    break;
  case concealedPistol:
    return "Crate: pistol";
    break;
  case concealedMachineGun:
    return "Crate: machine gun";
    break;
  case concealedGrenade:
    return "Crate: grenade";
    break;
  case concealedFlamethrower:
    return "Crate: flamethrower";
    break;
  case concealedAxe:
    return "Crate: axe";
    break;
  case concealedBazooka:
    return "Crate: bazooka";
    break;
  case concealedSaber:
    return "Crate: saber";
    break;
  case concealedShield:
    return "Crate: shield";
    break;
  case concealedSpecial:
    return "Crate: special";
    break;
  case destructible:
    return "Destructible";
    break;
  case vanishing:
    return "Vanishing";
    break;
  default:
    break;
  }
}


};
