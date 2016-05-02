#ifndef LONELYPALETTEVIEW_H
#define LONELYPALETTEVIEW_H


#include "editor/NesColorPickerScene.h"
#include "editor/LonelyPalettePickerScene.h"
#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaPalettes.h"
#include "structs/CachedGraphic.h"
#include "structs/Tstring.h"

namespace Lonely {


class LonelyPaletteView {
public:
  const static int numEditablePalettes = 20;
  enum EditablePalette {
    standardPaletteBackground,
    standardPaletteSprites,
    bossPaletteBackground,
    bossPaletteSprites,
    caveReplacementBackgroundPalette,
    baseReplacementBackgroundPalette,
    bossReplacementSpritePalette,
    paletteCycleState1,
    paletteCycleState2,
    paletteCycleState3,
    paletteCycleState4,
    laylaFadeState1,
    laylaFadeState2,
    laylaFadeState3,
    laylaFadeState4,
    laylaFadeState5,
    laylaFadeState6,
    lowHpCycle,
    regularHpCycle,
    specialFlashCycle
  };
  
  const static Tstring nameOfEditablePalette(
    EditablePalette editablePalette);

  LonelyPaletteView(int levelNum__,
                    LaylaLevel& level__,
                    LaylaPalettes& palettes__);
  
  void changeLevel(int levelNum__, LaylaLevel& level__);
  
  void changePalette(EditablePalette activePalette__);
  
  void drawViews();
  
  Graphic& colorPickerView();
  Graphic& palettePickerView();
  
  void* colorPickerCallbackObj();
  
  void* palettePickerCallbackObj();
  
  void colorPicked(int index);
  
  void palettePicked(int index);
protected:
  int levelNum_;
  LaylaLevel* level_;
  LaylaPalettes* palettes_;
  
  NesColorPickerScene colorPickerScene_;
  CachedGraphic colorPickerCache_;
  
  LonelyPalettePickerScene palettePickerScene_;
  CachedGraphic palettePickerCache_;
  
  EditablePalette activePalette_;
};


};


#endif
