#include "gamedata/LaylaBoss.h"

namespace Lonely {


Tstring LaylaBosses::nameOfBoss(LaylaBoss boss) {
  switch (boss) {
    case bamou:
      return "Bamou";
      break;
    case esperdra:
      return "Esperdra";
      break;
    case batear:
      return "Batear";
      break;
    case taratab:
      return "Taratab";
      break;
    case dinobares:
      return "Dinobares";
      break;
  default:
    return "***UNKNOWN***";
    break;
  }
}
  
bool LaylaBosses::bossIsInObjectSet(LaylaBoss boss,
                                    int objectSetNum) {
  switch (objectSetNum) {
  case 0:
    switch (boss) {
    case bamou:
    case batear:
      return true;
      break;
    default:
      return false;
      break;
    }
    break;
  case 1:
    switch (boss) {
    case esperdra:
    case batear:
      return true;
      break;
    default:
      return false;
      break;
    }
    break;
  case 2:
    switch (boss) {
    case bamou:
    case taratab:
      return true;
      break;
    default:
      return false;
      break;
    }
    break;
  case 3:
    switch (boss) {
    case dinobares:
      return true;
      break;
    default:
      return false;
      break;
    }
    break;
  default:
    return false;
    break;
  }
}


};
