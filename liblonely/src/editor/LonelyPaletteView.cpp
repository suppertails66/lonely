#include "editor/LonelyPaletteView.h"
#include "editor/LonelyPalettePickerSceneFunctor.h"

namespace Lonely {


LonelyPaletteView::LonelyPaletteView(int levelNum__,
                    LaylaLevel& level__,
                    LaylaPalettes& palettes__)
  : levelNum_(levelNum__),
    level_(&level__),
    palettes_(&palettes__),
    activePalette_(standardPaletteBackground) {
  colorPickerScene_.setItemPickedFunctor(
    *(new LonelyColorPickerSceneItemPicked(*this)));
  palettePickerScene_.setItemPickedFunctor(
    *(new LonelyPalettePickerSceneItemPicked(*this)));
  
  changeLevel(levelNum__, level__);
}
  
const Tstring LonelyPaletteView::nameOfEditablePalette(
    EditablePalette editablePalette) {
  switch (editablePalette) {
  case standardPaletteBackground :
    return "Standard palette (BG)";
    break;
  case standardPaletteSprites:
    return "Standard palette (sprites)";
    break;
  case bossPaletteBackground:
    return "Boss palette (BG)";
    break;
  case bossPaletteSprites:
    return "Boss palette (sprites)";
    break;
  case caveReplacementBackgroundPalette:
    return "Cave patch (BG)";
    break;
  case baseReplacementBackgroundPalette:
    return "Base patch (BG)";
    break;
  case bossReplacementSpritePalette:
    return "Boss patch (sprites)";
    break;
  case paletteCycleState1:
    return "Cycling palette [1]";
    break;
  case paletteCycleState2:
    return "Cycling palette [2]";
    break;
  case paletteCycleState3:
    return "Cycling palette [3]";
    break;
  case paletteCycleState4:
    return "Cycling palette [4]";
    break;
  case laylaFadeState1:
    return "Layla fade [1]";
    break;
  case laylaFadeState2:
    return "Layla fade [2]";
    break;
  case laylaFadeState3:
    return "Layla fade [3]";
    break;
  case laylaFadeState4:
    return "Layla fade [4]";
    break;
  case laylaFadeState5:
    return "Layla fade [5]";
    break;
  case laylaFadeState6:
    return "Layla fade [6]";
    break;
  case lowHpCycle:
    return "Low HP cycle";
    break;
  case regularHpCycle:
    return "Regular HP cycle";
    break;
  case specialFlashCycle:
    return "Special flash cycle";
    break;
  default:
    return "***UNKNOWN***";
    break;
  }
}
  
void LonelyPaletteView::changeLevel(int levelNum__, LaylaLevel& level__) {
  levelNum_ = levelNum__;
  level_ = &level__;
  
  changePalette(activePalette_);
}
  
void LonelyPaletteView::changePalette(EditablePalette activePalette__) {
  switch (activePalette__) {
  case standardPaletteBackground:
    palettePickerScene_.loadNesPaletteQuad(
        palettes_->standardPalette().backgroundPalettes());
    break;
  case standardPaletteSprites:
    palettePickerScene_.loadNesPaletteQuad(
        palettes_->standardPalette().spritePalettes());
    break;
  case bossPaletteBackground:
    palettePickerScene_.loadNesPaletteQuad(
        palettes_->bossPalette().backgroundPalettes());
    break;
  case bossPaletteSprites:
    palettePickerScene_.loadNesPaletteQuad(
        palettes_->bossPalette().spritePalettes());
    break;
  case caveReplacementBackgroundPalette:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->caveReplacementBackgroundPalette(
                    palettes_->cavePatchIndex(levelNum_)));
    break;
  case baseReplacementBackgroundPalette:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->baseReplacementBackgroundPalette(
                    palettes_->basePatchIndex(levelNum_)));
    break;
  case bossReplacementSpritePalette:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->bossReplacementSpritePalette(
                    palettes_->bossPatchIndex(levelNum_)));
    break;
  case paletteCycleState1:
    palettePickerScene_.loadLaylaPalettePatch(
        level_->paletteCyclePatches()[0]);
    break;
  case paletteCycleState2:
    palettePickerScene_.loadLaylaPalettePatch(
        level_->paletteCyclePatches()[1]);
    break;
  case paletteCycleState3:
    palettePickerScene_.loadLaylaPalettePatch(
        level_->paletteCyclePatches()[2]);
    break;
  case paletteCycleState4:
    palettePickerScene_.loadLaylaPalettePatch(
        level_->paletteCyclePatches()[3]);
    break;
  case laylaFadeState1:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->laylaFadeSpritePalette(0));
    break;
  case laylaFadeState2:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->laylaFadeSpritePalette(1));
    break;
  case laylaFadeState3:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->laylaFadeSpritePalette(2));
    break;
  case laylaFadeState4:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->laylaFadeSpritePalette(3));
    break;
  case laylaFadeState5:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->laylaFadeSpritePalette(4));
    break;
  case laylaFadeState6:
    palettePickerScene_.loadLaylaPalettePatch(
        palettes_->laylaFadeSpritePalette(5));
    break;
  case lowHpCycle:
    palettePickerScene_.loadLaylaPalettePatch(
        level_->lowHpCyclePatches()[0]);
    break;
  case regularHpCycle:
    palettePickerScene_.loadLaylaPalettePatch(
        level_->regularHpCyclePatches()[0]);
    break;
  case specialFlashCycle:
    palettePickerScene_.loadLaylaPalettePatch(
        level_->specialFlashCyclePatches()[0]);
    break;
  default:
    break;
  }
  
  // Refresh selected color in color picker
  palettePicked(palettePickerScene_.pickedIndex());
  
  activePalette_ = activePalette__;
}
  
void LonelyPaletteView::drawViews() {
  Graphic& colorPickerGraphic = colorPickerCache_.graphicAtSize(
                                      colorPickerScene_.realW(),
                                      colorPickerScene_.realH());
  colorPickerScene_.render(colorPickerGraphic, 0, 0);
  
  Graphic& palettePickerGraphic = palettePickerCache_.graphicAtSize(
                                      palettePickerScene_.realW(),
                                      palettePickerScene_.realH());
  palettePickerScene_.render(palettePickerGraphic, 0, 0);
}
  
Graphic& LonelyPaletteView::colorPickerView() {
  return colorPickerCache_.graphic();
}

Graphic& LonelyPaletteView::palettePickerView() {
  return palettePickerCache_.graphic();
}
  
void* LonelyPaletteView::colorPickerCallbackObj() {
  return (void*)(&colorPickerScene_);
}
  
void* LonelyPaletteView::palettePickerCallbackObj() {
  return (void*)(&palettePickerScene_);
}
  
void LonelyPaletteView::colorPicked(int index) {
  palettePickerScene_.changeColorAtPickedIndex((Tbyte)(index));
}

void LonelyPaletteView::palettePicked(int index) {
//  palettePickerScene_.changeColorAtPickedIndex(index);
  colorPickerScene_.pickIndex(
    palettePickerScene_.colorAtPickedIndex().nativeValue());
}


};
