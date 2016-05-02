#include "editor/NesColorPickerScene.h"
#include <iostream>

namespace Lonely {


NesColorPickerScene::NesColorPickerScene(
                      int colorBoxesPerRow__,
                      int colorBoxWidth__,
                      int colorBoxHeight__)
  : IndexedPickerScene(NesColor::numNativeColors(),
                       colorBoxesPerRow__,
                       colorBoxWidth__,
                       colorBoxHeight__) { };
  
NesColorPickerScene::~NesColorPickerScene() {
  
}

void NesColorPickerScene::drawSelectable(Graphic& dst,
                            int x, int y,
                            int selectableIndex) {
  Tcolor color = NesColor(selectableIndex).realColor();
  
  dst.fillRect(x, y,
               selectableWidth_, selectableHeight_,
               color,
               Graphic::noTransUpdate);
}

int NesColorPickerScene
  ::indexOfSelectableAt(int selectableX, int selectableY) {
  return (selectableY * selectablesPerRow_)
                    + selectableX;
}


};
