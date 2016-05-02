#ifndef NESCOLORPICKERSCENE_H
#define NESCOLORPICKERSCENE_H


#include "structs/IndexedPickerScene.h"
#include "nes/NesColor.h"

namespace Lonely {


class NesColorPickerScene : public IndexedPickerScene {
public:
  NesColorPickerScene(int colorBoxesPerRow__ = defaultColorBoxesPerRow_,
                      int colorBoxWidth__ = defaultColorBoxWidth_,
                      int colorBoxHeight__ = defaultColorBoxHeight_);
  
  virtual ~NesColorPickerScene();
  
  virtual void drawSelectable(Graphic& dst,
                              int x, int y,
                              int selectableIndex);
  
  virtual int indexOfSelectableAt(int selectableX, int selectableY);
protected:
  const static int defaultColorBoxesPerRow_ = 16;
  const static int defaultColorBoxWidth_ = 16;
  const static int defaultColorBoxHeight_ = 16;
  
};


};


#endif
