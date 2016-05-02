#include "editor/LonelyPalettePickerScene.h"
#include <cstdlib>

namespace Lonely {


LonelyPalettePickerScene::LonelyPalettePickerScene(
        int colorBoxesPerRow__,
        int colorBoxWidth__,
        int colorBoxHeight__)
  : IndexedPickerScene(numDisplayedColors_,
                       colorBoxesPerRow__,
                       colorBoxWidth__,
                       colorBoxHeight__) {
  for (int i = 0; i < numDisplayedColors_; i++) {
    colors_[i] = NULL;
    colorsEnabled_[i] = false;
  }
}
  
LonelyPalettePickerScene::~LonelyPalettePickerScene() {
  
}

void LonelyPalettePickerScene::drawSelectable(Graphic& dst,
                            int x, int y,
                            int selectableIndex) {
  if (selectableIndex >= numDisplayedColors_) {
    return;
  }
                            
  if (colorsEnabled_[selectableIndex]) {
    if (colors_[selectableIndex] != NULL) {
      dst.fillRect(x, y,
                   selectableWidth_, selectableHeight_,
                   colors_[selectableIndex]->realColor(),
                   Graphic::noTransUpdate);
    }
  }
  else {
    dst.fillRect(x, y,
                 selectableWidth_, selectableHeight_,
                 Tcolor(0xFF, 0xFF, 0xFF),
                 Graphic::noTransUpdate);
    dst.drawLine(x, y,
                 x + selectableWidth_, y + selectableHeight_,
                 Tcolor(0xFF, 0x00, 0x00),
                 1,
                 Graphic::noTransUpdate);
    dst.drawLine(x + selectableWidth_, y,
                 x, y + selectableHeight_,
                 Tcolor(0xFF, 0x00, 0x00),
                 1,
                 Graphic::noTransUpdate);
  }
}

int LonelyPalettePickerScene
  ::indexOfSelectableAt(int selectableX, int selectableY) {
  return selectableX;
}
                              
bool LonelyPalettePickerScene
  ::indexIsSelectable(int selectableIndex) {
  if (selectableIndex >= numDisplayedColors_) {
    return false;
  }
  
  return colorsEnabled_[selectableIndex];
}
  
void LonelyPalettePickerScene
  ::loadNesPaletteQuad(NesPaletteQuad& paletteQuad) {
  for (int i = 0; i < numDisplayedColors_; i++) {
    colors_[i] = &(paletteQuad.colorRef(i));
    colorsEnabled_[i] = true;
  }
  
  tryToEnsurePickedIndexIsValid();
}

void LonelyPalettePickerScene
  ::loadLaylaPalettePatch(LaylaPalettePatch& laylaPalettePatch) {
  for (int i = 0; i < numDisplayedColors_; i++) {
    colorsEnabled_[i] = laylaPalettePatch.colorEnabled(i);
    if (laylaPalettePatch.colorEnabled(i)) {
      colors_[i] = &(laylaPalettePatch.colorRef(i));
    }
    else {
      colors_[i] = NULL;
    }
  }
  
  tryToEnsurePickedIndexIsValid();
}
  
void LonelyPalettePickerScene
  ::tryToEnsurePickedIndexIsValid() {
  // Try to ensure a valid index is selected
  if (!(indexIsSelectable(pickedIndex()))) {
    for (int i = 0; i < totalSelectables(); i++) {
      if (indexIsSelectable(i)) {
        pickIndex(i);
        break;
      }
    }
  }
}
  
NesColor LonelyPalettePickerScene
  ::colorAtPickedIndex() {
  if (!pickedSelectableBoxEnabled_) {
    return NesColor();
  }
  
  if ((!colorsEnabled_[pickedIndex()])
      || (colors_[pickedIndex()] == NULL)) {
    return NesColor();
  }
  
  return *(colors_[pickedIndex()]);
}
  
void LonelyPalettePickerScene
  ::changeColorAtPickedIndex(Tbyte color) {
  if (!pickedSelectableBoxEnabled_) {
    return;
  }
  
  if ((!colorsEnabled_[pickedIndex()])
      || (colors_[pickedIndex()] == NULL)) {
    return;
  }
  
  colors_[pickedIndex()]->setNativeValue(color);
}


};
