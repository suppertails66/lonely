#include "editor/LonelyPalettePickerSceneFunctor.h"
#include "editor/LonelyPaletteView.h"

namespace Lonely {


LonelyPalettePickerSceneItemPicked
  ::LonelyPalettePickerSceneItemPicked(LonelyPaletteView& paletteView__)
  : paletteView_(&paletteView__) { };

LonelyPalettePickerSceneItemPicked
  ::~LonelyPalettePickerSceneItemPicked() {
  
}

void LonelyPalettePickerSceneItemPicked
  ::operator()(int index) {
  paletteView_->palettePicked(index);
}

LonelyColorPickerSceneItemPicked
  ::LonelyColorPickerSceneItemPicked(LonelyPaletteView& paletteView__)
  : paletteView_(&paletteView__) { };

LonelyColorPickerSceneItemPicked
  ::~LonelyColorPickerSceneItemPicked() {
  
}

void LonelyColorPickerSceneItemPicked
  ::operator()(int index) {
  paletteView_->colorPicked(index);
}


};
