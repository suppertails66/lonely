#include "editor/LonelyPatternView.h"
#include "editor/LonelyMetatilePickerSceneFunctor.h"
#include <iostream>

namespace Lonely {


LonelyPatternView::LonelyPatternView(int levelNum__,
                     LaylaLevel& level__,
                     LaylaGraphics& graphics__,
                     LaylaPalettes& palettes__,
                     LaylaStaticMetatiles& staticMetatiles__)
  : patternEditingScene_(),
    patternCache_(0, 0),
    metatilePickerScene_(),
    metatileCache_(0, 0),
    levelNum_(levelNum__),
    patternNum_(0),
    level_(&level__),
    graphics_(&graphics__),
    palettes_(&palettes__),
    staticMetatiles_(&staticMetatiles__),
    zoom_(1.00),
    metatilePickerZoom_(false),
    gridEnabled_(true),
    viewType_(MetatileViewTypes::visual),
    grid_(LaylaMetatile::width,
          1,
          Tcolor(0x88, 0x88, 0x88, Tcolor::fullAlphaOpacity),
          0, 0,
          0, 0,
          false),
    activeAreaType_(0) {
  patternEditingScene_.setPencilChangedFunctor(
    *(new PatternViewMetatilePicked(*this)));
  metatilePickerScene_.setItemPickedFunctor(
    *(new PatternViewMetatilePicked(*this)));
  changeLevel(levelNum_, level__);
}

void LonelyPatternView::changeLevel(int levelNum__, LaylaLevel& level__) {
  levelNum_ = levelNum__;
  level_ = &level__;
  cacheMetatiles();
  regenerateDefaultAreaTypes();
  changePattern(0);
}
  
void LonelyPatternView::cacheMetatiles() {
  metatileCaches_.cacheMetatiles(
    levelNum_,
    level_->baseMetatileSet(),
    *staticMetatiles_,
    *graphics_,
    *palettes_);
}
  
void LonelyPatternView::reloadRefresh() {
  cacheMetatiles();
  reloadActivePattern();
}
  
void LonelyPatternView::drawViews() {
  Graphic& patternGraphic = patternCache_.graphicAtSize(
    patternEditingScene_.camW() * patternEditingScene_.zoom(),
    patternEditingScene_.camH() * patternEditingScene_.zoom());
  patternGraphic.clearOpaqueBlack();
  
  patternEditingScene_.render(patternGraphic,
                              0, 0);
  
  Graphic& metatileGraphic = metatileCache_.graphicAtSize(
    metatilePickerScene_.realW(),
    metatilePickerScene_.realH());
  metatileGraphic.clearOpaqueBlack();
  
  metatilePickerScene_.render(metatileGraphic,
                              0, 0);
  
  Graphic& objectsGraphic = objectsCache_.graphicAtSize(
    activePattern().realWidth() * zoom_,
    objectsGraphicHeight_ * zoom_);
  objectsGraphic.clear(Tcolor(0x00, 0x00, 0x00));
  
  level_->objectPatterns().drawPatternObjectOverlay(
    objectsGraphic,
    0, 0,
    zoom_,
    patternNum_,
    activePattern().numColumns(),
    level_->objectSetNum(),
    graphics_->areaSprites(activeAreaType_),
    palettes_->generateAreaPalette(activeAreaType_,
                                   levelNum_)
      .spritePalettes());
}

Graphic& LonelyPatternView::patternView() {
  return patternCache_.graphic();
}
  
Graphic& LonelyPatternView::metatilePickerView() {
  return metatileCache_.graphic();
}
  
Graphic& LonelyPatternView::objectsView() {
  return objectsCache_.graphic();
}
  
const LaylaObjectPatternTable& LonelyPatternView::objectPatterns() const {
  return level_->objectPatterns();
}
  
void LonelyPatternView::changePattern(int index) {
//  patternEditingScene_ = LonelyPatternEditingScene(
//                            level_->patternDefinitions().pattern(index),
//                            metatileCaches_.cacheFromAreaType(
//                              defaultAreaTypes_[index]));
  patternEditingScene_.changePattern(
    level_->patternDefinitions().pattern(index));
//  patternEditingScene_.setZoom(zoom_);
//  patternEditingScene_.setGridEnabled(gridEnabled_);
//  patternEditingScene_.changeViewType(viewType_);
  patternNum_ = index;
  
  changeMetatiles(defaultAreaTypes_[index]);
}
  
void LonelyPatternView::changeMetatiles(int areaType) {
  LonelyMetatileCache& cache = metatileCaches_.cacheFromAreaType(
                              areaType);
  patternEditingScene_.changeMetatiles(cache);
  metatilePickerScene_.changeMetatiles(cache);
  activeAreaType_ = areaType;
//  metatilePickerScene_.setGridEnabled(gridEnabled_);
//  metatilePickerScene_.changeViewType(viewType_);
  
//  std::cout << cache.numMetatiles() << " "
//    << patternEditingScene_.pencilIndex() << std::endl;

  // Ensure pencil tool index is within range
  if (patternEditingScene_.pencilIndex()
        >= cache.numMetatiles()) {
    patternEditingScene_.setPencilIndex(
      cache.numMetatiles() - 1);
    metatilePickerScene_.pickIndex(patternEditingScene_.pencilIndex());
  }
}
  
void LonelyPatternView::setPatternViewSize(int w__, int h__) {
  patternEditingScene_.setCamW(w__);
  patternEditingScene_.setCamH(h__);
}
  
void LonelyPatternView::setZoom(double zoom__) {
  zoom_ = zoom__;
  patternEditingScene_.setZoom(zoom_);
}
  
bool LonelyPatternView::gridEnabled() const {
  return gridEnabled_;
}

void LonelyPatternView::setGridEnabled(bool gridEnabled__) {
  gridEnabled_ = gridEnabled__;
  patternEditingScene_.setGridEnabled(gridEnabled__);
  metatilePickerScene_.setGridEnabled(gridEnabled__);
}

LaylaPattern& LonelyPatternView::activePattern() {
  return level_->patternDefinitions().pattern(patternNum_);
}

const LaylaPattern& LonelyPatternView::activePattern() const {
  return level_->patternDefinitions().pattern(patternNum_);
}
  
int LonelyPatternView::activePatternWidth() const {
  return activePattern().numColumns();
}
  
void LonelyPatternView::changeActivePatternWidth(int width) {
  activePattern().changeWidth(width);
  reloadActivePattern();
}
  
void LonelyPatternView::reloadActivePattern() {
  changePattern(patternNum_);
}
  
void* LonelyPatternView::patternViewCallbackObj() {
  return (void*)(&patternEditingScene_);
}
  
void* LonelyPatternView::metatilePickerCallbackObj() {
  return (void*)(&metatilePickerScene_);
}

MetatileViewTypes::MetatileViewType LonelyPatternView::viewType() const {
  return viewType_;
}
  
void LonelyPatternView::changeViewType(
    MetatileViewTypes::MetatileViewType viewType__) {
  viewType_ = viewType__;
  
  patternEditingScene_.changeViewType(viewType_);
  metatilePickerScene_.changeViewType(viewType_);
}
  
void LonelyPatternView::metatilePickerItemPicked(int index) {
  patternEditingScene_.changeTool(LonelyPatternEditingScene::pencil);
  patternEditingScene_.setPencilIndex(index);
  metatilePickerScene_.pickIndex(index);
}
  
bool LonelyPatternView::metatilePickerZoomEnabled() const {
  return metatilePickerZoom_;
}
  
void LonelyPatternView::setMetatilePickerZoomEnabled(bool enabled) {
  metatilePickerZoom_ = enabled;
  
  refreshMetatilePickerZoom();
}
  
void LonelyPatternView::refreshMetatilePickerZoom() {
  if (metatilePickerZoom_) {
    metatilePickerScene_.setZoom(2.00);
  }
  else {
    metatilePickerScene_.setZoom(1.00);
  }
}
  
void LonelyPatternView::regenerateDefaultAreaTypes() {
  // Guess the appropriate area type for each pattern
  defaultAreaTypes_.clear();
  level_->areaData().generatePatternAreaTypeGuesses(
      defaultAreaTypes_,
      level_->patternDefinitions());
}
  
Tbyte LonelyPatternView::activeAreaType() const {
  return activeAreaType_;
}
  
int LonelyPatternView::currentObjectPatternIndex() const {
  return level_->objectPatterns().indexOfPattern(patternNum_);
}

void LonelyPatternView::setCurrentObjectPatternIndex(int newIndex) {
  level_->objectPatterns().setIndexOfPattern(patternNum_, newIndex);
}
  
int LonelyPatternView::insertNewPattern() {
  // Insert pattern entry in pattern definition table
  level_->patternDefinitions().insertPattern(patternNum_);
  
  // Insert entry in pattern object index
  level_->objectPatterns().insertPatternIntoIndex(patternNum_);
  
  // Fix existing pattern references in area layout
  level_->areaData().fixReferencesAfterNewPatternAdded(patternNum_);
  
  // Regenerate default area types table
  regenerateDefaultAreaTypes();
  
  // Return new index position
  return patternNum_;
}

int LonelyPatternView::removePattern() {
  // Don't remove the last pattern
  if (level_->patternDefinitions().numPatterns() <= 1) {
    return 0;
  }
  
  // Remove pattern entry from pattern definition table
  level_->patternDefinitions().removePattern(patternNum_);
  
  // Remove entry from pattern object index
  level_->objectPatterns().removePatternFromIndex(patternNum_);
  
  // Fix existing pattern references in area layout
  level_->areaData().fixReferencesAfterPatternRemoved(patternNum_);
  
  // Regenerate default area types table
  regenerateDefaultAreaTypes();
  
  // Return new(?) index position
  int newPos = patternNum_;
  
  if (newPos >= level_->patternDefinitions().numPatterns()) {
    --newPos;
  }
  
  return newPos;
}
  
const NesPatternTable& LonelyPatternView::currentSpritePatternTable() const {
  return graphics_->areaSprites(activeAreaType_);
}

NesPaletteQuad LonelyPatternView::currentSpritePalettes() const {
  return palettes_->generateAreaPalette(activeAreaType_,
                                   levelNum_)
            .spritePalettes();
}
  
int LonelyPatternView::currentObjectSetNum() const {
  return level_->objectSetNum();
}

/*int LonelyPatternView::currentLevelNum() const {
  return levelNum_;
}

const LaylaGraphics& LonelyPatternView::graphics() const {
  return *graphics_;
}

const LaylaPalettes& LonelyPatternView::palettes() const {
  return *palettes_;
} */

StandardEditingScene::Tool LonelyPatternView::activeTool() const {
  return patternEditingScene_.activeTool();
}

void LonelyPatternView::changeActiveTool(StandardEditingScene::Tool tool) {
  patternEditingScene_.changeTool(tool);
}
  
bool LonelyPatternView::ignoreMetatileZero() const {
  return patternEditingScene_.ignoreMetatileZero();
}

void LonelyPatternView::setIgnoreMetatileZero(bool ignoreMetatileZero__) {
  patternEditingScene_.setIgnoreMetatileZero(ignoreMetatileZero__);
}
  
bool LonelyPatternView::inheritPreviousLayout() const {
  return activePattern().inheritPreviousLayout();
}

void LonelyPatternView::setInheritPreviousLayout(
          bool inheritPreviousLayout__) {
  activePattern().setInheritPreviousLayout(inheritPreviousLayout__);
}


}; 
