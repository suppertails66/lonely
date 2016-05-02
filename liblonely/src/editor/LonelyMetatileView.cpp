#include "editor/LonelyMetatileView.h"
#include <iostream>

namespace Lonely {


LonelyMetatileView::LonelyMetatileView(int levelNum__,
                     LaylaLevelSet& levelSet__,
                     LaylaLevel& level__,
                     LaylaGraphics& graphics__,
                     LaylaPalettes& palettes__,
                     LaylaStaticMetatiles& staticMetatiles__)
  : levelNum_(levelNum__),
    levelSet_(&levelSet__),
    level_(&level__),
    graphics_(&graphics__),
    palettes_(&palettes__),
    staticMetatiles_(&staticMetatiles__),
    activeMetatile_(NULL),
//    zoom_(1.00),
    metatilePickerZoom_(false),
    gridEnabled_(true),
    viewType_(MetatileViewTypes::visual),
    areaType_(SetAreaToken::caveID),
    pickedSubtileIndex_(0) {
  metatilePickerScene_.setItemPickedFunctor(
    *(new MetatileViewMetatilePicked(*this)));
  tilePickerScene_.setItemPickedFunctor(
    *(new MetatileViewTilePicked(*this)));
  subtilePickerScene_.setItemPickedFunctor(
    *(new MetatileViewSubtilePicked(*this)));
  
  changeLevel(levelNum_, *level_);
}
  
void LonelyMetatileView::changeLevel(int levelNum__, LaylaLevel& level__) {
  levelNum_ = levelNum__;
  level_ = &level__;
  cacheMetatiles();
  refreshMetatiles();
  refreshActiveMetatile();
  refreshTilePickerTiles();
  refreshScenes();
//  changeArea(areaType_);
}
  
void LonelyMetatileView::changeArea(Tbyte areaType__) {
  areaType_ = areaType__;
  
  refreshMetatiles();
  refreshActiveMetatile();
  refreshTilePickerTiles();
//  metatilePickerScene_ = LonelyMetatilePickerScene(
//                          metatileCaches_.cacheFromAreaType(areaType_));
  
}
  
void LonelyMetatileView::changeMetatile(int index) {
  subtilePickerScene_.pickIndex(0);
  
  refreshActiveMetatile();
  refreshTilePickerTiles();
}
  
void LonelyMetatileView::drawViews() {
  Graphic& metatileGraphic = metatileCache_.graphicAtSize(
    metatilePickerScene_.realW(),
    metatilePickerScene_.realH());
  metatileGraphic.clearOpaqueBlack();
  
  metatilePickerScene_.render(metatileGraphic,
                              0, 0);
  
  Graphic& tileGraphic = tileCache_.graphicAtSize(
    tilePickerScene_.realW(),
    tilePickerScene_.realH());
  tileGraphic.clearOpaqueBlack();
  
  tilePickerScene_.render(tileGraphic,
                          0, 0);
  
  Graphic& subtileGraphic = subtileCache_.graphicAtSize(
    subtilePickerScene_.realW(),
    subtilePickerScene_.realH());
  subtileGraphic.clearOpaqueBlack();
  
  subtilePickerScene_.render(subtileGraphic,
                          0, 0);
}
  
Graphic& LonelyMetatileView::metatilePickerView() {
  return metatileCache_.graphic();
}
  
Graphic& LonelyMetatileView::tilePickerView() {
  return tileCache_.graphic();
}
  
Graphic& LonelyMetatileView::subtilePickerView() {
  return subtileCache_.graphic();
}
  
void* LonelyMetatileView::metatilePickerCallbackObj() { 
  return (void*)(&metatilePickerScene_);
}
  
void* LonelyMetatileView::tilePickerCallbackObj() {
  return (void*)(&tilePickerScene_);
}
  
void* LonelyMetatileView::subtilePickerCallbackObj() {
  return (void*)(&subtilePickerScene_);
}
  
void LonelyMetatileView::cacheMetatiles() {
  metatileCaches_.cacheMetatiles(
    levelNum_,
    level_->baseMetatileSet(),
    *staticMetatiles_,
    *graphics_,
    *palettes_);
}
  
void LonelyMetatileView::refreshMetatiles() {
  metatilePickerScene_.changeMetatiles(
    metatileCaches_.cacheFromAreaType(areaType_));
}
  
void LonelyMetatileView::refreshTilePickerTiles() {
  tilePickerScene_.changeTiles(
    graphics_->areaBackground(areaType_));
  tilePickerScene_.changePalette(
    palettes_->generateAreaPalette(areaType_, levelNum_).backgroundPalettes());
  tilePickerScene_.changePaletteIndex(
    activeMetatile_->palette);
  tilePickerScene_.pickIndex(
    activeMetatile_->tileOfIndex(pickedSubtileIndex_));
}
  
void LonelyMetatileView::refreshActiveMetatile() {
  activeMetatile_ = 
    &(activeMetatileSet().metatile(metatilePickerScene_.pickedIndex()));
  
  pickedSubtileIndex_ = subtilePickerScene_.pickedIndex();
  subtilePickerScene_.changeMetatile(*activeMetatile_);
  subtilePickerScene_.changeTiles(graphics_->areaBackground(areaType_));
  subtilePickerScene_.changePalettes(
    palettes_->generateAreaPalette(areaType_, levelNum_).backgroundPalettes());
}
  
void LonelyMetatileView::refreshAfterActiveMetatileUpdate() {
  // Refresh tile picker palette
  refreshTilePickerTiles();
  // Recache metatiles with new palette
  cacheMetatiles();
  
  refreshMetatiles();
}
  
LaylaMetatileSet& LonelyMetatileView::activeMetatileSet() {
  // Base metatiles are stored in the level itself
  if (areaType_ & SetAreaToken::baseFlag) {
    return level_->baseMetatileSet();
  }
  // Cave and boss metatiles are stored in separate "static" storage
  else if (areaType_ & SetAreaToken::bossFlag) {
    return staticMetatiles_->bossMetatiles().metatiles();
  }
  else {
    return staticMetatiles_->caveMetatiles().metatiles();
  }
}
  
const LaylaMetatileSet& LonelyMetatileView::activeMetatileSet() const {
  // Base metatiles are stored in the level itself
  if (areaType_ & SetAreaToken::baseFlag) {
    return level_->baseMetatileSet();
  }
  // Cave and boss metatiles are stored in separate "static" storage
  else if (areaType_ & SetAreaToken::bossFlag) {
    return staticMetatiles_->bossMetatiles().metatiles();
  }
  else {
    return staticMetatiles_->caveMetatiles().metatiles();
  }
}
  
void LonelyMetatileView::refreshScenes() {
  if (metatilePickerZoom_) {
    metatilePickerScene_.setZoom(2.00);
  }
  else {
    metatilePickerScene_.setZoom(1.00);
  }
  
  metatilePickerScene_.setGridEnabled(gridEnabled_);
  tilePickerScene_.setGridEnabled(gridEnabled_);
  subtilePickerScene_.setGridEnabled(gridEnabled_);
  
  if (metatilePickerScene_.viewType() != viewType_) {
    metatilePickerScene_.changeViewType(viewType_);
  }
  
}
  
void LonelyMetatileView::metatilePickerItemPicked(int index) {
  changeMetatile(index);
}
  
void LonelyMetatileView::tilePickerItemPicked(int index) {
//  std::cout << index << std::endl;
  activeMetatile_->tileOfIndex(pickedSubtileIndex_) = index;

  // Recache metatiles to reflect changes to the active metatile
  cacheMetatiles();
  refreshMetatiles();
}
  
void LonelyMetatileView::subtilePickerItemPicked(int index) {
  pickedSubtileIndex_ = index;
  
  refreshTilePickerTiles();
}
  
/*double LonelyMetatileView::zoom() const {
  return zoom_;
}

void LonelyMetatileView::setZoom(double zoom__) {
  zoom_ = zoom__;
} */

bool LonelyMetatileView::metatilePickerZoom() const {
  return metatilePickerZoom_;
}

void LonelyMetatileView::setMetatilePickerZoom(bool metatilePickerZoom__) {
  metatilePickerZoom_ = metatilePickerZoom__;
  
  refreshScenes();
}

bool LonelyMetatileView::gridEnabled() const {
  return gridEnabled_;
}

void LonelyMetatileView::setGridEnabled(bool gridEnabled__) {
  gridEnabled_ = gridEnabled__;
  
  refreshScenes();
}
  
LaylaMetatile& LonelyMetatileView::activeMetatile() {
  return *activeMetatile_;
}
  
void LonelyMetatileView::changeActiveMetatilePalette(int index) {
  activeMetatile_->palette = index;
  
  refreshAfterActiveMetatileUpdate();
}

void LonelyMetatileView::changeActiveMetatileBehavior(
    LaylaMetatileBehaviors::LaylaMetatileBehavior behavior) {
  activeMetatile_->behavior = behavior;
  
  refreshAfterActiveMetatileUpdate();
}

void LonelyMetatileView::changeActiveMetatileTopSolid(bool solid) {
  activeMetatile_->topSolid = solid;
  
  refreshAfterActiveMetatileUpdate();
}

void LonelyMetatileView::changeActiveMetatileBottomSolid(bool solid) {
  activeMetatile_->bottomSolid = solid;
  
  refreshAfterActiveMetatileUpdate();
}
  
int LonelyMetatileView::removeActiveMetatile() {
  int index = metatilePickerScene_.pickedIndex();
  
  // Don't remove the last metatile in the set
  if (activeMetatileSet().size() <= 1) {
    return index;
  }
  
  activeMetatileSet().removeMetatile(index);
  
  // If removing from base, we only need to fix the patterns for the current
  // level. Otherwise, the removed metatile is global and we have to fix
  // everything.
  if (areaType_ & SetAreaToken::baseFlag) {
    level_->fixAfterMetatileQuantityChange(areaType_,
                                           -1,
                                           index);
  }
  else {
    levelSet_->fixAfterMetatileQuantityChange(areaType_,
                                            -1,
                                            index);
  }
  
  // Make sure everything we're concerned with is still valid
  
  // If the last metatile was deleted, move the target index to the
  // previous slot
  if (index == activeMetatileSet().size()) {
    --index;
    
    metatilePickerScene_.pickIndex(index);
  }
  
  cacheMetatiles();
  refreshMetatiles();
  
  // Load the new metatile at the old index
  changeMetatile(index);
  
  return index;
}

int LonelyMetatileView::addMetatile() {
  int index = metatilePickerScene_.pickedIndex();
  
  // check for limits?
  // ...
  
  // If last metatile is selected, insert at end; otherwise,
  // insert before the selected position
  if (index == activeMetatileSet().size() - 1) {
    ++index;
  }
  
  int newIndex = activeMetatileSet().addMetatile(index);
  
  // If removing from base, we only need to fix the patterns for the current
  // level. Otherwise, the removed metatile is global and we have to fix
  // everything.
  if (areaType_ & SetAreaToken::baseFlag) {
    level_->fixAfterMetatileQuantityChange(areaType_,
                                           1,
                                           index);
  }
  else {
    levelSet_->fixAfterMetatileQuantityChange(areaType_,
                                            1,
                                            index);
  }
  
  metatilePickerScene_.pickIndex(newIndex);
  cacheMetatiles();
  refreshMetatiles();
  
  // Load the new metatile
  changeMetatile(newIndex);
  
  return newIndex;
}
  
int LonelyMetatileView::minNumMetatiles() const {
  return 1;
}
  
int LonelyMetatileView::maxNumMetatiles() const {
  // Temporary upper limits until expanded metatile sets are supported
  if (areaType_ & SetAreaToken::baseFlag) {
    return 0x73;
  }
  else if (areaType_ & SetAreaToken::bossFlag) {
    return 62;
  }
  else {
    return 66;
  }
  
}
  
int LonelyMetatileView::numMetatiles() const {
  return activeMetatileSet().size();
}

MetatileViewTypes::MetatileViewType LonelyMetatileView::viewType() const {
  return viewType_;
}

void LonelyMetatileView::setViewType(
    MetatileViewTypes::MetatileViewType viewType__) {
  viewType_ = viewType__;
  
  refreshScenes();
}


};
