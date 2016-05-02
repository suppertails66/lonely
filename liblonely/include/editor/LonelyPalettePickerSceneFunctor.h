#ifndef LONELYPALETTEPICKERSCENEFUNCTOR_H
#define LONELYPALETTEPICKERSCENEFUNCTOR_H


#include "structs/IndexedPickerSceneFunctor.h"

namespace Lonely {


class LonelyPaletteView;

class LonelyPalettePickerSceneItemPicked
        : public IndexedPickerSceneItemPicked {
public:
  LonelyPalettePickerSceneItemPicked(LonelyPaletteView& paletteView__);
  
  virtual ~LonelyPalettePickerSceneItemPicked();
  
  virtual void operator()(int index);
protected:
  LonelyPaletteView* paletteView_;
};

class LonelyColorPickerSceneItemPicked
        : public IndexedPickerSceneItemPicked {
public:
  LonelyColorPickerSceneItemPicked(LonelyPaletteView& paletteView__);
  
  virtual ~LonelyColorPickerSceneItemPicked();
  
  virtual void operator()(int index);
protected:
  LonelyPaletteView* paletteView_;
};


};


#endif
