#include "editor/LonelySubtilePickerScene.h"
#include <cstdlib>

namespace Lonely {


const double LonelySubtilePickerScene::fixedZoom_(4.00);
  
LonelySubtilePickerScene::LonelySubtilePickerScene()
  : IndexedPickerScene(numSubtiles,
                       LaylaMetatile::widthInTiles,
                       NesTile::width,
                       NesTile::height),
    metatile_(NULL),
    tiles_(NULL),
    palettes_() {
  setZoom(fixedZoom_);
}
  
LonelySubtilePickerScene::LonelySubtilePickerScene(
                           LaylaMetatile& metatile__,
                           NesPatternTable& tiles__,
                           NesPaletteQuad palettes__)
  : IndexedPickerScene(numSubtiles,
                       LaylaMetatile::widthInTiles,
                       NesTile::width,
                       NesTile::height),
    metatile_(&metatile__),
    tiles_(&tiles__),
    palettes_(palettes__) {
  setZoom(fixedZoom_);
}

LonelySubtilePickerScene::~LonelySubtilePickerScene() {
  
}

void LonelySubtilePickerScene::drawSelectable(Graphic& dst,
                            int x, int y,
                            int selectableIndex) {
  if ((metatile_ == NULL)
      || (tiles_ == NULL)) {
    return;
  }
  
  Tbyte tileNum = 0;
  switch (selectableIndex) {
  case LaylaMetatile::ulIndex:
    tileNum = metatile_->upperLeft;
    break;
  case LaylaMetatile::urIndex:
    tileNum = metatile_->upperRight;
    break;
  case LaylaMetatile::llIndex:
    tileNum = metatile_->lowerLeft;
    break;
  case LaylaMetatile::lrIndex:
    tileNum = metatile_->lowerRight;
    break;
  default:
    break;
  }
  
  tiles_->tile(tileNum).drawPalettized(
          dst,
          x, y,
          palettes_.palette(metatile_->palette),
          false);
}


int LonelySubtilePickerScene
  ::indexOfSelectableAt(int selectableX, int selectableY) {
  if (selectableX == 0) {
    if (selectableY == 0) {
      // upper-left
      return LaylaMetatile::ulIndex;
    }
    else {
      // lower-left
      return LaylaMetatile::llIndex;
    }
  }
  else {
    if (selectableY == 0) {
      // upper-right
      return LaylaMetatile::urIndex;
    }
    else {
      // lower-right
      return LaylaMetatile::lrIndex;
    }
  }
}

void LonelySubtilePickerScene::changeMetatile(LaylaMetatile& metatile__) {
  metatile_ = &metatile__;
}

void LonelySubtilePickerScene::changeTiles(NesPatternTable& tiles__) {
  tiles_ = &tiles__;
}

void LonelySubtilePickerScene::changePalettes(NesPaletteQuad palettes__) {
  palettes_ = palettes__;
}


};
