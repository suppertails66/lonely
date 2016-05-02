#ifndef STANDARDEDITINGSCENE_H
#define STANDARDEDITINGSCENE_H


#include "structs/LayeredGraphicScene.h"
#include "structs/InputEventData.h"
#include "structs/GridGraphicSceneLayer.h"
#include "structs/CachedGraphic.h"
#include "structs/StandardPencilTool.h"
#include "structs/StandardCloneTool.h"
#include "structs/IndexedPickerSceneFunctor.h"

namespace Lonely {


/**
 * Abstract base class for a "standard" editing graphical area.
 * The underlying model is a series of fixed-size "editables" laid out in a
 * grid. In the default implementation, editables are assumed to have integer
 * IDs numbered from zero.
 * The base class provides generic default implementations of several standard
 * editing tools:
 * - Pencil tool: A basic "pencil" tool. Right-clicking an editable selects
 *   it as the "active" editable, and left-clicking another editable overwrites
 *   the clicked editable with the active one.
 * - Area copy tool: Allows a rectangular area of editables to be copied
 *   and pasted to another location.
 */
class StandardEditingScene/* : public LayeredGraphicScene*/ {
public:
  enum Tool {
    none,
    pencil,
    clone
  };

  StandardEditingScene();
  
  StandardEditingScene(int totalEditables__,
                     int editablesPerRow__,
                     int editableWidth__,
                     int editableHeight__);
  
  virtual ~StandardEditingScene();
  
/*  virtual void render(Graphic& dst,
                      Box srcbox,
                      double scale);
  
  virtual void renderNative(Graphic& dst,
                            Box srcbox) =0; */
                            
  virtual void render(Graphic& dst,
                      int xOffset, int yOffset);
  
  virtual void drawEditable(Graphic& dst,
                              int x, int y,
                              int editableIndex) =0;
  
  virtual int indexOfEditableAt(int editableX, int editableY) =0;
  
  virtual void overwriteEditableAt(int editableX, int editableY,
                                   int newIndex) =0;
  
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
  
  bool gridEnabled() const;
  void setGridEnabled(bool gridEnabled__);
  
  int camX() const;
  void setCamX(int camX__);
  int camY() const;
  void setCamY(int camY__);
  int camW() const;
  void setCamW(int camW__);
  int camH() const;
  void setCamH(int camH__);
  
  double zoom() const;
  void setZoom(double zoom__);
  
  int nativeWidth() const;
  int nativeHeight() const;
  
  Tool activeTool() const;
  void changeTool(Tool activeTool__);
  
  int pencilIndex() const;
  void setPencilIndex(int index);
  
  void setPencilChangedFunctor(IndexedPickerSceneItemPicked&
    itemPickedFunctor__);
  
//  double sceneScale();
//  void setSceneScale(double sceneScale__);
protected:
  virtual void enterEvent();
  virtual void leaveEvent();
  virtual void mouseMoveEvent(InputEventData event);
  virtual void mousePressEvent(InputEventData event);
  virtual void mouseReleaseEvent(InputEventData event);
  virtual void mouseDoubleClickEvent(InputEventData event);
  
  virtual int editablesOfLeftOverdraw() const;
  virtual int editablesOfRightOverdraw() const;
  virtual int editablesOfTopOverdraw() const;
  virtual int editablesOfBottomOverdraw() const;
  
  int editablesPerColumn() const;
  
  int realPosToEditableIndex(int realX, int realY);
  int drawPosToEditableIndex(int drawX, int drawY);
  int editableIndexToDrawX(int index);
  int editableIndexToDrawY(int index);
  int editableIndexToNativeDrawX(int index);
  int editableIndexToNativeDrawY(int index);
  
  int sceneToEditableX(int realX);
  int sceneToEditableY(int realY);
  int editableToSceneX(int editableX);
  int editableToSceneY(int editableY);
  int sceneToNativeDrawX(int sceneX);
  int sceneToNativeDrawY(int sceneY);
  int sceneToScaledDrawX(int sceneX);
  int sceneToScaledDrawY(int sceneY);
  int editableToNativeDrawX(int editableX);
  int editableToNativeDrawY(int editableY);
  int editableToScaledDrawX(int editableX);
  int editableToScaledDrawY(int editableY);
  int editableToPosIndex(int editableX, int editableY);
  
  void processEvent(InputEventData& event);
  void unprocessEvent(InputEventData& event);
  
  void morphEventToScene(InputEventData& event);
  void unmorphEventFromScene(InputEventData& event);
  
  int originDrawX() const;
  int originDrawY() const;
  int originEditableX() const;
  int originEditableY() const;
  int limitEditableX() const;
  int limitEditableY() const;
  
  int realEditableWidth() const;
  int realEditableHeight() const;
  
//  Graphic nativeBuffer_;
  
  IndexedPickerSceneItemPicked* itemPickedFunctor_;
  
  int totalEditables_;
  int editablesPerRow_;
  int editableWidth_;
  int editableHeight_;
  
  int camX_;
  int camY_;
  int camW_;
  int camH_;
  
  double zoom_;
  
  CachedGraphic cachedGraphic_;
  
  bool gridEnabled_;
  
  GridGraphicSceneLayer grid_;
  
  Tool activeTool_;
  bool mouseIsIn_;
  StandardPencilTool pencilTool_;
  StandardCloneTool cloneTool_;
  
  virtual void drawToolPreviewPre(Graphic& dst);
  virtual void drawToolPreviewPost(Graphic& dst);
  
  virtual void drawPencilPreviewPre(Graphic& dst);
  virtual void drawPencilPreviewPost(Graphic& dst);
  virtual void handlePencilMove(InputEventData event);
  virtual void handlePencilPress(InputEventData event);
  virtual void handlePencilRelease(InputEventData event);
  virtual void handlePencilDoubleClick(InputEventData event);
  
  virtual void drawClonePreviewPre(Graphic& dst);
  virtual void drawClonePreviewPost(Graphic& dst);
  virtual void handleCloneMove(InputEventData event);
  virtual void handleClonePress(InputEventData event);
  virtual void handleCloneRelease(InputEventData event);
  virtual void handleCloneDoubleClick(InputEventData event);
  void cloneTo(int editableX, int editableY);
  int realCloneBaseX();
  int realCloneBaseY();
  int realCloneLimitX();
  int realCloneLimitY();
  
};


};


#endif 
