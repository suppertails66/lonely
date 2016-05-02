#ifndef INDEXEDPICKERSCENE_H
#define INDEXEDPICKERSCENE_H


#include "structs/LayeredGraphicScene.h"
#include "structs/InputEventData.h"
#include "structs/GridGraphicSceneLayer.h"
#include "structs/CachedGraphic.h"
#include "structs/IndexedPickerSceneFunctor.h"

namespace Lonely {


class IndexedPickerScene/* : public LayeredGraphicScene*/ {
public:
  IndexedPickerScene(int totalSelectables__,
                     int selectablesPerRow__,
                     int selectableWidth__,
                     int selectableHeight__);
  
  virtual ~IndexedPickerScene();
  
/*  virtual void render(Graphic& dst,
                      Box srcbox,
                      double scale); */
                            
  virtual void render(Graphic& dst,
                      int xOffset, int yOffset);
  
  virtual void renderNative(Graphic& dst);
  
  virtual void drawSelectable(Graphic& dst,
                              int x, int y,
                              int selectableIndex) =0;
  
  virtual int indexOfSelectableAt(int selectableX, int selectableY) =0;
                              
  virtual bool indexIsSelectable(int selectableIndex);
  
  static void enterEventCallback(
    void* vThis);
  static void leaveEventCallback(
    void* vThis);
  static void mouseMoveEventCallback(
    void* vThis, InputEventData event);
  static void mousePressEventCallback(
    void* vThis, InputEventData event);
  static void mouseReleaseEventCallback(
    void* vThis, InputEventData event);
  static void mouseDoubleClickEventCallback(
    void* vThis, InputEventData event);
  
  virtual void enterEvent();
  virtual void leaveEvent();
  virtual void mouseMoveEvent(InputEventData event);
  virtual void mousePressEvent(InputEventData event);
  virtual void mouseReleaseEvent(InputEventData event);
  virtual void mouseDoubleClickEvent(InputEventData event);
  
  int highlightedIndex();
  int pickedIndex();
  
  void clearHighlightedBox();
  void clearPickedBox();
  
  void enableHighlightedBox();
  void enablePickedBox();
  
  void pickIndex(int index);
  void highlightIndex(int index);
  
  bool gridEnabled() const;
  void setGridEnabled(bool gridEnabled__);
  
  double zoom();
  void setZoom(double zoom__);
  
  int realW() const;
  int realH() const;
  
  void setItemPickedFunctor(IndexedPickerSceneItemPicked&
    itemPickedFunctor__);
  
  int totalSelectables() const;
protected:
  int selectablesPerColumn() const;
  
  int nativeW() const;
  int nativeH() const;
  
  int realPosToSelectableIndex(int realX, int realY);
  int drawPosToSelectableIndex(int drawX, int drawY);
  int selectableIndexToDrawX(int index);
  int selectableIndexToDrawY(int index);
  int selectableIndexToNativeDrawX(int index);
  int selectableIndexToNativeDrawY(int index);
  
  IndexedPickerSceneItemPicked* itemPickedFunctor_;
  
  CachedGraphic nativeBuffer_;
  
  int totalSelectables_;
  int selectablesPerRow_;
  int selectableWidth_;
  int selectableHeight_;
  
  double zoom_;

  bool highlightedSelectableBoxEnabled_;
  int highlightedSelectableIndex_;

  bool pickedSelectableBoxEnabled_;
  int pickedSelectableIndex_;
  
  bool gridEnabled_;
  
  GridGraphicSceneLayer grid_;
};


};


#endif
