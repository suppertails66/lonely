#include "editor/LonelyMetatilePickerScene.h"
#include "gamedata/LaylaMetatile.h"
#include <cstdlib>

namespace Lonely {


const double LonelyMetatilePickerScene::fixedZoom_(1.00);
  
LonelyMetatilePickerScene::LonelyMetatilePickerScene()
  : IndexedPickerScene(0,
                       metatilesPerRow_,
                       LaylaMetatile::width,
                       LaylaMetatile::height),
    metatiles_(NULL),
    viewType_(MetatileViewTypes::visual) {
  zoom_ = fixedZoom_;
}
  
LonelyMetatilePickerScene::LonelyMetatilePickerScene(
  LonelyMetatileCache& metatiles__)
  : IndexedPickerScene(metatiles__.numMetatiles(),
                       metatilesPerRow_,
                       LaylaMetatile::width,
                       LaylaMetatile::height),
    metatiles_(&metatiles__),
    viewType_(MetatileViewTypes::visual) {
  zoom_ = fixedZoom_;
}
  
LonelyMetatilePickerScene::~LonelyMetatilePickerScene() {
  
}
  
void LonelyMetatilePickerScene::drawSelectable(Graphic& dst,
                            int x, int y,
                            int selectableIndex) {
  dst.copy(metatileGraphic(selectableIndex),
           Box(x, y, 0, 0),
           Graphic::noTransUpdate);
}

int LonelyMetatilePickerScene::indexOfSelectableAt(
    int selectableX, int selectableY) {
  return (selectableY * metatilesPerRow_) + selectableX;
}
  
MetatileViewTypes::MetatileViewType LonelyMetatilePickerScene
  ::viewType() const {
  return viewType_;
}

void LonelyMetatilePickerScene
  ::changeViewType(MetatileViewTypes::MetatileViewType viewType__) {
  viewType_ = viewType__;
}
  
void LonelyMetatilePickerScene
  ::changeMetatiles(LonelyMetatileCache& metatiles__) {
  metatiles_ = &metatiles__;
  totalSelectables_ = metatiles_->numMetatiles();
  
  // Ensure selected index is still valid
  if (pickedSelectableIndex_ >= totalSelectables_) {
    pickedSelectableIndex_ = totalSelectables_ - 1;
  }
}
  
Graphic& LonelyMetatilePickerScene
  ::metatileGraphic(int index) {
  switch (viewType_) {
  case MetatileViewTypes::visual:
    return metatiles_->metatileVisual(index);
    break;
  case MetatileViewTypes::behavior:
    return metatiles_->metatileBehavior(index);
    break;
  case MetatileViewTypes::solidity:
    return metatiles_->metatileSolidity(index);
    break;
  default:
    break;
  }
}


};
