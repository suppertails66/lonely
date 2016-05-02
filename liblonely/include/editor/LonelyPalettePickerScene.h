#ifndef LONELYPALETTEPICKERSCENE_H
#define LONELYPALETTEPICKERSCENE_H


#include "structs/IndexedPickerScene.h"
#include "nes/NesColor.h"
#include "nes/NesPaletteQuad.h"
#include "gamedata/LaylaPalettePatch.h"

namespace Lonely {


class LonelyPalettePickerScene : public IndexedPickerScene {
public:
  LonelyPalettePickerScene(int colorBoxesPerRow__ = defaultColorBoxesPerRow_,
                      int colorBoxWidth__ = defaultColorBoxWidth_,
                      int colorBoxHeight__ = defaultColorBoxHeight_);
  
  virtual ~LonelyPalettePickerScene();
  
  virtual void drawSelectable(Graphic& dst,
                              int x, int y,
                              int selectableIndex);
  
  virtual int indexOfSelectableAt(int selectableX, int selectableY);
                              
  virtual bool indexIsSelectable(int selectableIndex);
  
  void loadNesPaletteQuad(NesPaletteQuad& paletteQuad);
  
  void loadLaylaPalettePatch(LaylaPalettePatch& laylaPalettePatch);
  
  NesColor colorAtPickedIndex();
  void changeColorAtPickedIndex(Tbyte color);
protected:
  const static int numDisplayedColors_ = 16;
  const static int defaultColorBoxesPerRow_ = 16;
  const static int defaultColorBoxWidth_ = 16;
  const static int defaultColorBoxHeight_ = 16;
  
  NesColor* colors_[numDisplayedColors_];
  bool colorsEnabled_[numDisplayedColors_];
  
  void tryToEnsurePickedIndexIsValid();
};


};


#endif
