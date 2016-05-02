#include "structs/StandardEditingScene.h"
#include "util/MiscMath.h"
#include <utility>
#include <algorithm>
#include <iostream>

namespace Lonely {


StandardEditingScene::StandardEditingScene()
  : itemPickedFunctor_(new IndexedPickerSceneItemPicked()),
    totalEditables_(0),
    editablesPerRow_(0),
    editableWidth_(0),
    editableHeight_(0),
    camX_(0),
    camY_(0),
    camW_(0),
    zoom_(1.00),
    gridEnabled_(true),
    grid_(editableWidth_,
               1,
               Tcolor(0x88, 0x88, 0x88, Tcolor::fullAlphaOpacity),
               0, 0,
               0, 0,
               false),
    activeTool_(pencil),
    mouseIsIn_(false) { };
    
  
StandardEditingScene::StandardEditingScene(int totalEditables__,
                                       int editablesPerRow__,
                                       int editableWidth__,
                                       int editableHeight__)
  : itemPickedFunctor_(new IndexedPickerSceneItemPicked()),
    totalEditables_(totalEditables__),
    editablesPerRow_(editablesPerRow__),
    editableWidth_(editableWidth__),
    editableHeight_(editableHeight__),
    camX_(0),
    camY_(0),
    camW_(0),
    camH_(0),
    zoom_(1.00),
    gridEnabled_(true),
    grid_(editableWidth_,
               1,
               Tcolor(0x88, 0x88, 0x88, Tcolor::fullAlphaOpacity),
               0, 0,
               0, 0,
               false),
    activeTool_(pencil),
    mouseIsIn_(false) { };
  
StandardEditingScene::~StandardEditingScene() {
  delete itemPickedFunctor_;
}
  
/*void StandardEditingScene::render(Graphic& dst,
                          Box srcbox,
                          double scale) {
  if ((nativeBuffer_.w() != srcbox.w())
      || (nativeBuffer_.h() != srcbox.h())) {
    nativeBuffer_ = Graphic(srcbox.w(), srcbox.h());
  }
                          
  renderNative(nativeBuffer_,
               srcbox);
  
  int scaledW = srcbox.w() * zoom_;
  int scaledH = srcbox.h() * zoom_;
  
  nativeBuffer_.scale(dst,
                       Box(0, 0,
                           scaledW,
                           scaledH));
      
  if (gridEnabled_) {
    grid_.render(dst,
                      Box(srcbox.x(),
                          srcbox.y(),
                          scaledW,
                          scaledH),
                      zoom_);
  }
  
  if ((highlightedEditableBoxEnabled_)) {
    dst.drawRectBorder(editableIndexToDrawX(highlightedEditableIndex_),
                       editableIndexToDrawY(highlightedEditableIndex_),
                       editableWidth_ * zoom_,
                       editableHeight_ * zoom_,
                       Tcolor(0xFF, 0x00, 0x00, Tcolor::fullAlphaOpacity),
                       2);
  }
  
  if ((pickedEditableBoxEnabled_)) {
    dst.drawRectBorder(editableIndexToDrawX(pickedEditableIndex_),
                       editableIndexToDrawY(pickedEditableIndex_),
                       editableWidth_ * zoom_,
                       editableHeight_ * zoom_,
                       Tcolor(0xFF, 0xFF, 0x00, Tcolor::fullAlphaOpacity),
                       2);
  } 
} */
                            
void StandardEditingScene::render(Graphic& dst,
                    int xOffset, int yOffset) {
  Graphic& graphic = cachedGraphic_.graphicAtSize(camW_, camH_);
  graphic.clearTransparentBlack();
  
  int baseDrawX = originDrawX();
  int drawX = baseDrawX;
  int drawY = originDrawY();
//  int baseEditableX = originEditableX();
  int endEditableX = limitEditableX();
  int endEditableY = limitEditableY();
//  int editableX = baseEditableX;
//  int editableY = originEditableY();
  
//  std::cout << baseDrawX << " " << drawX << " " << drawY << std::endl;
//  std::cout << endEditableX << " " << endEditableY << std::endl;
//  std::cout << originEditableX() << " " << originEditableY() << std::endl;
  
  for (int j = originEditableY(); j < endEditableY; j++) {
    // Draw each row if the current row getpos is valid;
    // otherwise, increment drawpos and move to next getpos
    if (j >= 0) {
      for (int i = originEditableX(); i < endEditableX; i++) {
        if ((i >= 0)) {
          // Don't draw editables that don't exist
          if (editableToPosIndex(i, j) >= totalEditables_) {
            continue;
          }
          
          drawEditable(graphic,
                       drawX, drawY,
                       indexOfEditableAt(i, j));
        }
        drawX += editableWidth_;
      }
      drawX = baseDrawX;
    }
    drawY += editableHeight_;
  }
  
  if (zoom_ == 1.00) {
    drawToolPreviewPre(graphic);
    
    if (gridEnabled_) {
      grid_.render(graphic,
                        Box(camX_, camY_,
                            graphic.w(), graphic.h()),
                        zoom_);
    }
    
    drawToolPreviewPost(graphic);
  
    dst.copy(graphic,
             Box(xOffset, yOffset, 0, 0),
             Graphic::noTransUpdate);
  }
  else {
    drawToolPreviewPre(graphic);
    
    Graphic scaledG(graphic.w() * zoom_, graphic.h() * zoom_);
    scaledG.scale(graphic);
    
    if (gridEnabled_) {
      grid_.render(scaledG,
                        Box(camX_, camY_,
                            scaledG.w(), scaledG.h()),
                        zoom_);
    }
    
    drawToolPreviewPost(scaledG);
  
    dst.copy(scaledG,
             Box(xOffset, yOffset, 0, 0),
             Graphic::noTransUpdate);
    
/*    dst.scaleAndCopy(graphic,
                     Box(xOffset, yOffset, camW_ * zoom_, camH_ * zoom_),
                     Graphic::noTransUpdate); */
  }
  
  
}
  
void StandardEditingScene::enterEventCallback(
  void* vThis) {
  ((StandardEditingScene*)(vThis))->enterEvent();
}

void StandardEditingScene::leaveEventCallback(
  void* vThis) {
  ((StandardEditingScene*)(vThis))->leaveEvent();
}

void StandardEditingScene::mouseMoveEventCallback(
  void* vThis, InputEventData event) {
  ((StandardEditingScene*)(vThis))->mouseMoveEvent(event);
}

void StandardEditingScene::mousePressEventCallback(
  void* vThis, InputEventData event) {
  ((StandardEditingScene*)(vThis))->mousePressEvent(event);
}

void StandardEditingScene::mouseReleaseEventCallback(
  void* vThis, InputEventData event) {
  ((StandardEditingScene*)(vThis))->mouseReleaseEvent(event);
}

void StandardEditingScene::mouseDoubleClickEventCallback(
  void* vThis, InputEventData event) {
  ((StandardEditingScene*)(vThis))->mouseDoubleClickEvent(event);
}
  
void StandardEditingScene::enterEvent() {
  mouseIsIn_ = true;
}

void StandardEditingScene::leaveEvent() {
  mouseIsIn_ = false;
}

void StandardEditingScene::mouseMoveEvent(InputEventData event) {
  processEvent(event);

//  int posIndex = drawPosToEditableIndex(
//                                  event.x(), event.y());
  
//  std::cout << event.x() << " " << event.y() << std::endl;
  
  switch (activeTool_) {
  case pencil:
    handlePencilMove(event);
    break;
  case clone:
    handleCloneMove(event);
    break;
  default:
    break;
  }
  
//  std::cout << editableX << " " << editableY << std::endl;
}

void StandardEditingScene::mousePressEvent(InputEventData event) {
  processEvent(event);
  
  switch (activeTool_) {
  case pencil:
    handlePencilPress(event);
    break;
  case clone:
    handleClonePress(event);
    break;
  default:
    break;
  }
}

void StandardEditingScene::mouseReleaseEvent(InputEventData event) {
  processEvent(event);
  
  switch (activeTool_) {
  case pencil:
    handlePencilRelease(event);
    break;
  case clone:
    handleCloneRelease(event);
    break;
  default:
    break;
  }
}

void StandardEditingScene::mouseDoubleClickEvent(InputEventData event) {
  processEvent(event);
  
  switch (activeTool_) {
  case pencil:
    handlePencilDoubleClick(event);
    break;
  case clone:
    handleCloneDoubleClick(event);
    break;
  default:
    break;
  }
}
  
void StandardEditingScene::processEvent(InputEventData& event) {
  morphEventToScene(event);
  
  // Ensure coordinates are within valid range
  if (event.x() < 0) {
    event.setX(0);
  }
  else if (event.x() >= nativeWidth()) {
    event.setX(nativeWidth() - 1);
  }
  
  if (event.y() < 0) {
    event.setY(0);
  }
  else if (event.y() >= nativeHeight()) {
    event.setY(nativeHeight() - 1);
  }
  
}

void StandardEditingScene::unprocessEvent(InputEventData& event) {
  unmorphEventFromScene(event);
}
  
int StandardEditingScene::nativeWidth() const {
  return editablesPerRow_ * editableWidth_;
}

int StandardEditingScene::nativeHeight() const {
  return editablesPerColumn() * editableHeight_;
}
  
void StandardEditingScene::morphEventToScene(InputEventData& event) {
  event.morphCoordinates(camX_,
                             camY_,
                             zoom_);
}

void StandardEditingScene::unmorphEventFromScene(InputEventData& event) {
  event.unmorphCoordinates(camX_,
                               camY_,
                               zoom_);
}
  
int StandardEditingScene::sceneToEditableX(int realX) {
  return (realX / editableWidth_);
}

int StandardEditingScene::sceneToEditableY(int realY) {
  return (realY / editableHeight_);
}

int StandardEditingScene::editableToSceneX(int editableX) {
  return (editableX * editableWidth_);
}

int StandardEditingScene::editableToSceneY(int editableY) {
  return (editableY * editableHeight_);
}

int StandardEditingScene::sceneToNativeDrawX(int sceneX) {
  return (sceneX - camX_);
}

int StandardEditingScene::sceneToNativeDrawY(int sceneY) {
  return (sceneY - camY_);
}

int StandardEditingScene::sceneToScaledDrawX(int sceneX) {
  return (sceneX - camX_) * zoom_;
}

int StandardEditingScene::sceneToScaledDrawY(int sceneY) {
  return (sceneY - camY_) * zoom_;
}

int StandardEditingScene::editableToPosIndex(int editableX, int editableY) {
  return (editableY * editablesPerRow_) + editableX;
}

int StandardEditingScene::editableToNativeDrawX(int editableX) {
  return sceneToNativeDrawX(editableToSceneX(editableX));
}

int StandardEditingScene::editableToNativeDrawY(int editableY) {
  return sceneToNativeDrawY(editableToSceneY(editableY));
}

int StandardEditingScene::editableToScaledDrawX(int editableX) {
  return sceneToScaledDrawX(editableToSceneX(editableX));
}

int StandardEditingScene::editableToScaledDrawY(int editableY) {
  return sceneToScaledDrawY(editableToSceneY(editableY));
}
  
int StandardEditingScene::editablesOfLeftOverdraw() const {
  return 1;
}

int StandardEditingScene::editablesOfRightOverdraw() const {
  return 1;
}

int StandardEditingScene::editablesOfTopOverdraw() const {
  return 1;
}

int StandardEditingScene::editablesOfBottomOverdraw() const {
  return 1;
}
  
bool StandardEditingScene::gridEnabled() const {
  return gridEnabled_;
}
 
void StandardEditingScene::setGridEnabled(bool gridEnabled__) {
  gridEnabled_ = gridEnabled__;
}

int StandardEditingScene::editablesPerColumn() const {
//  return (totalEditables_ / editablesPerRow_);
  return (totalEditables_ / editablesPerRow_)
    + (((totalEditables_ % editablesPerRow_) == 0)
      ? 0
      : 1);
}
  
int StandardEditingScene::camX() const {
  return camX_;
}

void StandardEditingScene::setCamX(int camX__) {
  camX_ = camX__;
}

int StandardEditingScene::camY() const {
  return camY_;
}

void StandardEditingScene::setCamY(int camY__) {
  camY_ = camY__;
}

int StandardEditingScene::camW() const {
  return camW_;
}

void StandardEditingScene::setCamW(int camW__) {
  camW_ = camW__;
}

int StandardEditingScene::camH() const {
  return camH_;
}

void StandardEditingScene::setCamH(int camH__) {
  camH_ = camH__;
}
  
double StandardEditingScene::zoom() const {
  return zoom_;
}

void StandardEditingScene::setZoom(double zoom__) {
  zoom_ = zoom__;
}
  
int StandardEditingScene::realPosToEditableIndex(int realX, int realY) {
  return ((realY / editableHeight_) * editablesPerRow_)
            + (realX / editableWidth_);
}

int StandardEditingScene::drawPosToEditableIndex(int drawX, int drawY) {
  return realPosToEditableIndex(drawX / zoom_,
                                  drawY / zoom_);
}

int StandardEditingScene::editableIndexToDrawX(int index) {
  return editableIndexToNativeDrawX(index) * zoom_;
}

int StandardEditingScene::editableIndexToDrawY(int index) {
  return editableIndexToNativeDrawY(index) * zoom_;
}

int StandardEditingScene::editableIndexToNativeDrawX(int index) {
  return (index % editablesPerRow_) * editableWidth_;
}

int StandardEditingScene::editableIndexToNativeDrawY(int index) {
  return (index / editablesPerRow_) * editableHeight_;
}
  
int StandardEditingScene::originDrawX() const {
  return (camX_ % editableWidth_)
          - (editablesOfLeftOverdraw() * editableWidth_);
}

int StandardEditingScene::originDrawY() const {
  return (camY_ % editableHeight_)
          - (editablesOfTopOverdraw() * editableHeight_);
}

int StandardEditingScene::originEditableX() const {
  int x = (camX_ / editableWidth_) - editablesOfLeftOverdraw();
  
//  if (x < 0) {
//    return 0;
//  }
//  else {
    return x;
//  }
}

int StandardEditingScene::originEditableY() const {
  int y = (camY_ / editableHeight_) - editablesOfTopOverdraw();
  
//  if (y < 0) {
//    return 0;
//  }
//  else {
    return y;
//  }
}

int StandardEditingScene::limitEditableX() const {
  int x = originEditableX()
            + (camW_ / editableWidth_)
            + (editablesOfRightOverdraw() * editableWidth_);
  
  if (x > editablesPerRow_) {
    return editablesPerRow_;
  }
  else {
    return x;
  }
}

int StandardEditingScene::limitEditableY() const {
  int y = originEditableY()
            + (camH_ / editableHeight_)
            + (editablesOfBottomOverdraw() * editableHeight_);
  
  if (y > editablesPerColumn()) {
    return editablesPerColumn();
  }
  else {
    return y;
  }
}
  
int StandardEditingScene::realEditableWidth() const {
  return editableWidth_ * zoom_;
}

int StandardEditingScene::realEditableHeight() const {
  return editableHeight_ * zoom_;
}
  
StandardEditingScene::Tool StandardEditingScene::activeTool() const {
  return activeTool_;
}
  
void StandardEditingScene::changeTool(Tool activeTool__) {
//  cloneTool_.mode = StandardCloneTool::waitingForSelectClick;

  activeTool_ = activeTool__;
}
  
int StandardEditingScene::pencilIndex() const {
  return pencilTool_.activeIndex;
}

void StandardEditingScene::setPencilIndex(int index) {
  pencilTool_.activeIndex = index;
}
  
void StandardEditingScene::setPencilChangedFunctor(
    IndexedPickerSceneItemPicked& itemPickedFunctor__) {
  delete itemPickedFunctor_;
  itemPickedFunctor_ = &itemPickedFunctor__;
}
  
void StandardEditingScene::drawToolPreviewPre(Graphic& dst) {
  if (!mouseIsIn_) {
    return;
  }

  switch (activeTool_) {
  case pencil:
    drawPencilPreviewPre(dst);
    break;
  case clone:
    drawClonePreviewPre(dst);
    break;
  default:
    break;
  }
}

void StandardEditingScene::drawToolPreviewPost(Graphic& dst) {
  if (!mouseIsIn_) {
    return;
  }

  switch (activeTool_) {
  case pencil:
    drawPencilPreviewPost(dst);
    break;
  case clone:
    drawClonePreviewPost(dst);
    break;
  default:
    break;
  }
}
  
void StandardEditingScene::drawPencilPreviewPre(Graphic& dst) {
  drawEditable(dst,
               editableToNativeDrawX(pencilTool_.editableX),
               editableToNativeDrawY(pencilTool_.editableY),
               pencilTool_.activeIndex);
}

void StandardEditingScene::drawPencilPreviewPost(Graphic& dst) {
  int baseX = editableToScaledDrawX(pencilTool_.editableX);
  int baseY = editableToScaledDrawY(pencilTool_.editableY);
  
  dst.drawRectBorder(baseX, baseY,
                     realEditableWidth(),
                     realEditableHeight(),
                     Tcolor(0xFF, 0xFF, 0x00),
                     2,
                     Graphic::noTransUpdate);
}
  
void StandardEditingScene::handlePencilMove(InputEventData event) {
  pencilTool_.editableX = sceneToEditableX(event.x());
  pencilTool_.editableY = sceneToEditableY(event.y());
  
  handlePencilPress(event);
}

void StandardEditingScene::handlePencilPress(InputEventData event) {
  int editableX = sceneToEditableX(event.x());
  int editableY = sceneToEditableY(event.y());
    
  // In case we got here from another tool, reset the picked location
  pencilTool_.editableX = editableX;
  pencilTool_.editableY = editableY;
  
  if (event.mouseLeftButton()) {
    if (event.control()) {
      activeTool_ = clone;
      handleClonePress(event);
    }
    else {
      overwriteEditableAt(editableX, editableY, pencilTool_.activeIndex);
    }
  }
  else if (event.mouseRightButton()) {
    pencilTool_.activeIndex = indexOfEditableAt(editableX, editableY);
    (*itemPickedFunctor_)(pencilTool_.activeIndex);
  }
}

void StandardEditingScene::handlePencilRelease(InputEventData event) {
  
}

void StandardEditingScene::handlePencilDoubleClick(InputEventData event) {
  
}
  
void StandardEditingScene::drawClonePreviewPre(Graphic& dst) {
  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForCloneClick:
      
      break;
    default:
      return;
      break;
  }
  
  int baseX = realCloneBaseX();
  int baseY = realCloneBaseY();
  int limitX = realCloneLimitX();
  int limitY = realCloneLimitY();
  
  for (int j = baseY; j < limitY; j++) {
    for (int i = baseX; i < limitX; i++) {
      drawEditable(dst,
                   editableToNativeDrawX(i), editableToNativeDrawY(j),
                   cloneTool_.copyData.data(i - baseX, j - baseY));
    }
  }
}

void StandardEditingScene::drawClonePreviewPost(Graphic& dst) {
  // Don't draw a border if nothing has been selected yet
  if (cloneTool_.mode == StandardCloneTool::waitingForSelectClick) {
    return;
  }

  Tcolor borderColor;
  
  int baseX = realCloneBaseX();
  int baseY = realCloneBaseY();
  int limitX = realCloneLimitX();
  int limitY = realCloneLimitY();

  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForSelectRelease:
      borderColor = Tcolor(0xFF, 0x00, 0x00);
      break;
    case StandardCloneTool::waitingForCloneClick:
      borderColor = Tcolor(0xFF, 0xFF, 0x00);
      break;
    default:
      break;
  }
  
  int drawX = editableToScaledDrawX(baseX);
  int drawY = editableToScaledDrawY(baseY);
  int drawW = editableToScaledDrawX(limitX) - drawX;
  int drawH = editableToScaledDrawY(limitY) - drawY;
  
  dst.drawRectBorder(drawX,
                     drawY,
                     drawW,
                     drawH,
                     borderColor,
                     2,
                     Graphic::noTransUpdate);
}

int StandardEditingScene::realCloneBaseX() {
  int baseX = cloneTool_.editableBaseX;
  int limitX = cloneTool_.editableDragX;

  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForCloneClick:
      limitX = baseX + cloneTool_.copyData.w() - 1;
      break;
    default:
      break;
  }
  
  if (limitX < baseX) {
    std::swap(baseX, limitX);
  }
  ++limitX;
  
  MiscMath::clamp(baseX, 0, editablesPerRow_);
  
  return baseX;
}

int StandardEditingScene::realCloneBaseY() {
  int baseY = cloneTool_.editableBaseY;
  int limitY = cloneTool_.editableDragY;

  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForCloneClick:
      limitY = baseY + cloneTool_.copyData.h() - 1;
      break;
    default:
      break;
  }
  
  if (limitY < baseY) {
    std::swap(baseY, limitY);
  }
  ++limitY;
  
  MiscMath::clamp(baseY, 0, editablesPerColumn());
  
  return baseY;
}

int StandardEditingScene::realCloneLimitX() {
  int baseX = cloneTool_.editableBaseX;
  int limitX = cloneTool_.editableDragX;

  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForCloneClick:
      limitX = baseX + cloneTool_.copyData.w() - 1;
      break;
    default:
      break;
  }
  
  if (limitX < baseX) {
    std::swap(baseX, limitX);
  }
  ++limitX;
  
  MiscMath::clamp(limitX, 0, editablesPerRow_);
  
  return limitX;
}

int StandardEditingScene::realCloneLimitY() {
  int baseY = cloneTool_.editableBaseY;
  int limitY = cloneTool_.editableDragY;

  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForCloneClick:
      limitY = baseY + cloneTool_.copyData.h() - 1;
      break;
    default:
      break;
  }
  
  if (limitY < baseY) {
    std::swap(baseY, limitY);
  }
  ++limitY;
  
  MiscMath::clamp(limitY, 0, editablesPerColumn());
  
  return limitY;
}

void StandardEditingScene::handleCloneMove(InputEventData event) {
  int editableX = sceneToEditableX(event.x());
  int editableY = sceneToEditableY(event.y());

  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForSelectRelease:
      cloneTool_.editableDragX = editableX;
      cloneTool_.editableDragY = editableY;
      break;
    case StandardCloneTool::waitingForCloneClick:
      cloneTool_.editableBaseX = editableX;
      cloneTool_.editableBaseY = editableY;
      
      if (event.mouseLeftButton()) {
        handleClonePress(event);
      }
      break;
    default:
      break;
  }
}

void StandardEditingScene::handleClonePress(InputEventData event) {
  int editableX = sceneToEditableX(event.x());
  int editableY = sceneToEditableY(event.y());

  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForSelectClick:
      if (event.mouseLeftButton()) {
        cloneTool_.mode = StandardCloneTool::waitingForSelectRelease;
        cloneTool_.editableBaseX = editableX;
        cloneTool_.editableBaseY = editableY;
        cloneTool_.editableDragX = editableX;
        cloneTool_.editableDragY = editableY;
      }
      else if (event.mouseRightButton())  {
        changeTool(pencil);
        handlePencilPress(event);
        return;
      }
      break;
    case StandardCloneTool::waitingForCloneClick:
      if (event.mouseLeftButton()) {
        if (event.control()) {
          cloneTool_.mode = StandardCloneTool::waitingForSelectClick;
          handleClonePress(event);
          return;
        }
      
        cloneTool_.editableBaseX = editableX;
        cloneTool_.editableBaseY = editableY;
        
        cloneTo(editableX, editableY);
//        cloneTool_.mode = StandardCloneTool::waitingForSelectRelease;
      }
      else if (event.mouseRightButton())  {
        changeTool(pencil);
        handlePencilPress(event);
        return;
      }
      break;
    default:
      break;
  }
}

void StandardEditingScene::handleCloneRelease(InputEventData event) {
  int editableX = sceneToEditableX(event.x());
  int editableY = sceneToEditableY(event.y());
  
  switch (cloneTool_.mode) {
    case StandardCloneTool::waitingForSelectRelease:
      {
      int baseX = realCloneBaseX();
      int baseY = realCloneBaseY();
      int limitX = realCloneLimitX();
      int limitY = realCloneLimitY();
      
      int width = limitX - baseX;
      int height = limitY - baseY;
      cloneTool_.copyData.resize(width, height);
      
      for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
          cloneTool_.copyData.data(i, j)
            = indexOfEditableAt(i + baseX, j + baseY);
        }
      }
      
      cloneTool_.editableBaseX = editableX;
      cloneTool_.editableBaseY = editableY;
      
      cloneTool_.mode = StandardCloneTool::waitingForCloneClick;
      }
      break;
    default:
      break;
  }
}

void StandardEditingScene::handleCloneDoubleClick(InputEventData event) {
  
}

void StandardEditingScene::cloneTo(int editableX, int editableY) {
  int baseX = realCloneBaseX();
  int baseY = realCloneBaseY();
  int limitX = realCloneLimitX();
  int limitY = realCloneLimitY();
  
  for (int j = baseY; j < limitY; j++) {
    for (int i = baseX; i < limitX; i++) {
      overwriteEditableAt(i, j,
                          cloneTool_.copyData.data(i - baseX, j - baseY));
    }
  }
}



}; 
