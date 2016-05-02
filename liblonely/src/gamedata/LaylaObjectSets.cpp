#include "gamedata/LaylaObjectSets.h"

namespace Lonely {



LonelyLaylaObjects::LonelyLaylaObject
    LaylaObjectSets::lonelyID(int setNum, int objNum) {
  switch (setNum) {
  case 0:
    return set0[objNum];
    break;
  case 1:
    return set1[objNum];
    break;
  case 2:
    return set2[objNum];
    break;
  case 3:
    return set3[objNum];
    break;
  default:
    return LonelyLaylaObjects::unk1;
    break;
  }
}
      
int LaylaObjectSets::objectSetToLevelNum(int num) {
  return num * objectToLevelMultiplier_;
}
    
const LaylaObjectSets::LonelyLaylaObjectSetMapping
    LaylaObjectSets::set0 =
  {
    LonelyLaylaObjects::unk1,
    LonelyLaylaObjects::unk2,
    LonelyLaylaObjects::fallingWeight,
    LonelyLaylaObjects::keyCard,
    LonelyLaylaObjects::trappedIris,
    LonelyLaylaObjects::undefined,
    LonelyLaylaObjects::dolgadol,
    LonelyLaylaObjects::jethro,
    LonelyLaylaObjects::vandol,
    LonelyLaylaObjects::ben,
    LonelyLaylaObjects::purim,
    LonelyLaylaObjects::soudchil,
    LonelyLaylaObjects::friendy,
    LonelyLaylaObjects::spagani,
    LonelyLaylaObjects::flippedSpagani,
    LonelyLaylaObjects::pogos
  };

const LaylaObjectSets::LonelyLaylaObjectSetMapping
    LaylaObjectSets::set1 =
  {
    LonelyLaylaObjects::unk1,
    LonelyLaylaObjects::unk2,
    LonelyLaylaObjects::fallingWeight,
    LonelyLaylaObjects::keyCard,
    LonelyLaylaObjects::trappedIris,
    LonelyLaylaObjects::undefined,
    LonelyLaylaObjects::envy,
    LonelyLaylaObjects::mirako,
    LonelyLaylaObjects::rogin,
    LonelyLaylaObjects::buldora,
    LonelyLaylaObjects::apara,
    LonelyLaylaObjects::at3,   // ????????????
    LonelyLaylaObjects::jondy,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none
  };

const LaylaObjectSets::LonelyLaylaObjectSetMapping
    LaylaObjectSets::set2 =
  {
    LonelyLaylaObjects::unk1,
    LonelyLaylaObjects::unk2,
    LonelyLaylaObjects::fallingWeight,
    LonelyLaylaObjects::keyCard,
    LonelyLaylaObjects::trappedIris,
    LonelyLaylaObjects::undefined,
    LonelyLaylaObjects::miichin,
    LonelyLaylaObjects::koan,
    LonelyLaylaObjects::at3,
    LonelyLaylaObjects::usumiil,
    LonelyLaylaObjects::shiilance,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none
  };

const LaylaObjectSets::LonelyLaylaObjectSetMapping
    LaylaObjectSets::set3 =
  {
    LonelyLaylaObjects::unk1,
    LonelyLaylaObjects::unk2,
    LonelyLaylaObjects::fallingWeight,
    LonelyLaylaObjects::keyCard,
    LonelyLaylaObjects::trappedIris,
    LonelyLaylaObjects::undefined,
    LonelyLaylaObjects::valketta,
    LonelyLaylaObjects::gramzi,
    LonelyLaylaObjects::bilganof,
    LonelyLaylaObjects::jethro,
    LonelyLaylaObjects::manitokha,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none,
    LonelyLaylaObjects::none
  };



}; 
