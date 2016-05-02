#include "editor/LonelyTilePickerScene.h"
#include <cstdlib>
#include <iostream>

namespace Lonely {


const double LonelyTilePickerScene::fixedZoom_(2.00);
  
LonelyTilePickerScene::LonelyTilePickerScene()
  : IndexedPickerScene(0,
                       tilesPerRow_,
                       NesTile::width,
                       NesTile::height),
    tiles_(NULL),
    palette_(),
    paletteIndex_(0) {
  setZoom(fixedZoom_);
}
  
LonelyTilePickerScene::LonelyTilePickerScene(
    NesPatternTable& tiles__,
    NesPaletteQuad palette__,
    int paletteIndex__)
  : IndexedPickerScene(tiles_->numTiles,
                       tilesPerRow_,
                       NesTile::width,
                       NesTile::height),
    tiles_(&tiles__),
    palette_(palette__),
    paletteIndex_(paletteIndex__) {
  setZoom(fixedZoom_);
}
  
LonelyTilePickerScene::~LonelyTilePickerScene() {
  
}

void LonelyTilePickerScene::drawSelectable(Graphic& dst,
                            int x, int y,
                            int selectableIndex) {
//  std::cout << selectableIndex << std::endl;
  
//  Graphic g;

/*  tiles_->tile(selectableIndex).drawGrayscale(
      dst,
      x, y,
      false); */
  
  tiles_->tile(selectableIndex).drawPalettized(
      dst,
      x, y,
      palette_.palette(paletteIndex_),
      false);
}

int LonelyTilePickerScene
  ::indexOfSelectableAt(int selectableX, int selectableY) {
  return (selectableY * tilesPerRow_) + selectableX;
}

void LonelyTilePickerScene
  ::LonelyTilePickerScene::changeTiles(NesPatternTable& tiles__) {
  tiles_ = &tiles__;
  totalSelectables_ = tiles_->numTiles;
}

void LonelyTilePickerScene
  ::LonelyTilePickerScene::changePalette(NesPaletteQuad palette__) {
  palette_ = palette__;
}

void LonelyTilePickerScene
  ::changePaletteIndex(int paletteIndex__) {
  paletteIndex_ = paletteIndex__;
}


};
