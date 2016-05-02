#ifndef LONELYTILEPICKERSCENE_H
#define LONELYTILEPICKERSCENE_H


#include "structs/IndexedPickerScene.h"
#include "nes/NesPatternTable.h"
#include "nes/NesPaletteQuad.h"

namespace Lonely {


class LonelyTilePickerScene : public IndexedPickerScene {
public:
  LonelyTilePickerScene();
  
  LonelyTilePickerScene(NesPatternTable& tiles__,
                        NesPaletteQuad palette__,
                        int paletteIndex__);
  
  virtual ~LonelyTilePickerScene();
  
  virtual void drawSelectable(Graphic& dst,
                              int x, int y,
                              int selectableIndex);
  
  virtual int indexOfSelectableAt(int selectableX, int selectableY);
  
  void changeTiles(NesPatternTable& tiles__);
  void changePalette(NesPaletteQuad palette__);
  void changePaletteIndex(int paletteIndex__);
protected:
  const static int tilesPerRow_ = 16;
  const static double fixedZoom_;
  
  NesPatternTable* tiles_;
  NesPaletteQuad palette_;
  int paletteIndex_;
  
};


};


#endif
