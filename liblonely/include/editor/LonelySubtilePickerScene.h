#ifndef LONELYSUBTILESPICKERSCENE_H
#define LONELYSUBTILESPICKERSCENE_H


#include "structs/IndexedPickerScene.h"
#include "gamedata/LaylaMetatile.h"
#include "nes/NesPatternTable.h"
#include "nes/NesPaletteQuad.h"

namespace Lonely {


class LonelySubtilePickerScene : public IndexedPickerScene {
public:
  LonelySubtilePickerScene();
  
  LonelySubtilePickerScene(LaylaMetatile& metatile__,
                           NesPatternTable& tiles__,
                           NesPaletteQuad palettes__);
  
  virtual ~LonelySubtilePickerScene();
  
  virtual void drawSelectable(Graphic& dst,
                              int x, int y,
                              int selectableIndex);
  
  virtual int indexOfSelectableAt(int selectableX, int selectableY);
  
  void changeMetatile(LaylaMetatile& metatile__);
  void changeTiles(NesPatternTable& tiles__);
  void changePalettes(NesPaletteQuad palettes__);
protected:
  const static double fixedZoom_;

  const static int numSubtiles = 4;

  LaylaMetatile* metatile_;
  NesPatternTable* tiles_;
  NesPaletteQuad palettes_;
}; 


};


#endif
