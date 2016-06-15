#include "editor/LonelyPatternEditingScene.h"
#include "gamedata/LaylaMetatile.h"
#include <cstdlib>

namespace Lonely {


LonelyPatternEditingScene::LonelyPatternEditingScene()
  : StandardEditingScene(0,
                         0,
                         LaylaMetatile::width,
                         LaylaMetatile::height),
    pattern_(NULL),
    metatiles_(NULL),
    viewType_(MetatileViewTypes::visual),
    ignoreMetatileZero_(false) { };

LonelyPatternEditingScene::LonelyPatternEditingScene(
                            LaylaPattern& pattern__,
                            LonelyMetatileCache& metatiles__)
  : StandardEditingScene(pattern__.numMetatiles(),
                         pattern__.numColumns(),
                         LaylaMetatile::width,
                         LaylaMetatile::height),
    pattern_(&pattern__),
    metatiles_(&metatiles__),
    viewType_(MetatileViewTypes::visual) {
  camW_ = pattern_->realWidth();
  camH_ = pattern_->realHeight();
}
  
void LonelyPatternEditingScene
  ::drawEditable(Graphic& dst,
                 int x, int y,
                 int editableIndex) {
  if (ignoreMetatileZero_ && (editableIndex == 0)) {
    return;
  }
  
  dst.copy(metatileGraphic(editableIndex),
           Box(x, y, 0, 0),
           Graphic::noTransUpdate);
}

int LonelyPatternEditingScene
  ::indexOfEditableAt(int editableX, int editableY) {
  return pattern_->metatileID(editableX, editableY);
}

void LonelyPatternEditingScene
  ::overwriteEditableAt(int editableX, int editableY,
                                 int newIndex) {
  if (ignoreMetatileZero_ && (newIndex == 0)) {
    return;
  }
  
  pattern_->setMetatileID(editableX, editableY, newIndex);
}
  
void LonelyPatternEditingScene
  ::changePattern(LaylaPattern& pattern__) {
  pattern_ = &pattern__;
  totalEditables_ = pattern_->numMetatiles();
  editablesPerRow_ = pattern_->numColumns();
  camW_ = pattern_->realWidth();
  camH_ = pattern_->realHeight();
}
  
void LonelyPatternEditingScene
  ::changeMetatiles(LonelyMetatileCache& metatiles__) {
  metatiles_ = &metatiles__;
}
  
void LonelyPatternEditingScene
  ::changeViewType(MetatileViewTypes::MetatileViewType viewType__) {
  viewType_ = viewType__;
}

bool LonelyPatternEditingScene
  ::ignoreMetatileZero() const {
  return ignoreMetatileZero_;
}
  
void LonelyPatternEditingScene
  ::setIgnoreMetatileZero(bool ignoreMetatileZero__) {
  ignoreMetatileZero_ = ignoreMetatileZero__;
}
  
Graphic& LonelyPatternEditingScene
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
