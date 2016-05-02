#include "gamedata/LonelyLaylaObject.h"

namespace Lonely {


Tstring LonelyLaylaObjects::nameOfObject(LonelyLaylaObject object) {
  switch (object) {
  case none:
    return "None";
    break;
  case unk1:
    return "Unknown 1";
    break;
  case unk2:
    return "Unknown 2";
    break;
  case fallingWeight:
    return "Unknown 3";
    break;
  case keyCard:
    return "Key card";
    break;
  case trappedIris:
    return "Trapped Iris";
    break;
  case undefined:
    return "Undefined";
    break;
  case purim:
    return "Purim";
    break;
  case ben:
    return "Ben";
    break;
  case spagani:
    return "Spagani";
    break;
  case flippedSpagani:
    return "Spagani (rev)";
    break;
  case friendy:
    return "Friendy";
    break;
  case jethro:
    return "Jethro";
    break;
  case apara:
    return "Apara";
    break;
  case envy:
    return "Envy";
    break;
  case jondy:
    return "Jondy";
    break;
  case mirako:
    return "Mirako";
    break;
  case miichin:
    return "Miichin";
    break;
  case shiilance:
    return "Shiilance";
    break;
  case koan:
    return "Koan";
    break;
  case bilganof:
    return "Bilganof";
    break;
  case soudchil:
    return "Soudchil";
    break;
  case pogos:
    return "Pogos";
    break;
  case vandol:
    return "Vandol";
    break;
  case dolgadol:
    return "Dolgadol";
    break;
  case buldora:
    return "Buldora";
    break;
  case rogin:
    return "Rogin";
    break;
  case at3:
    return "AT3";
    break;
  case torso:
    return "Torso";
    break;
  case usumiil:
    return "Usumiil";
    break;
  case valketta:
    return "Valketta";
    break;
  case gramzi:
    return "Gramzi";
    break;
  case manitokha:
    return "Manitokha";
    break;
  default:
    return "***UNKNOWN***";
    break;
  }
}


};
