#ifndef LONELYMETATILEPICKERSCENE_H
#define LONELYMETATILEPICKERSCENE_H


#include "structs/IndexedPickerScene.h"
#include "editor/LonelyMetatileCache.h"
#include "editor/MetatileViewType.h"

namespace Lonely {


class LonelyMetatilePickerScene : public IndexedPickerScene {
public:
  LonelyMetatilePickerScene();
  
  LonelyMetatilePickerScene(LonelyMetatileCache& metatiles__);
  
  virtual ~LonelyMetatilePickerScene();
  
  virtual void drawSelectable(Graphic& dst,
                              int x, int y,
                              int selectableIndex);
  
  virtual int indexOfSelectableAt(int selectableX, int selectableY);
  
  MetatileViewTypes::MetatileViewType viewType() const;
  void changeViewType(MetatileViewTypes::MetatileViewType viewType__);
  
  void changeMetatiles(LonelyMetatileCache& metatiles__);
protected:
  const static int metatilesPerRow_ = 8;
  const static double fixedZoom_;
  
  Graphic& metatileGraphic(int index);

  LonelyMetatileCache* metatiles_;
  
  MetatileViewTypes::MetatileViewType viewType_;
};


};


#endif
