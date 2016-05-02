#include "structs/IndexedPickerScene.h"
#include "util/MiscMath.h"

namespace Lonely {


IndexedPickerScene::IndexedPickerScene(int totalSelectables__,
                                       int selectablesPerRow__,
                                       int selectableWidth__,
                                       int selectableHeight__)
  : itemPickedFunctor_(new IndexedPickerSceneItemPicked()),
    totalSelectables_(totalSelectables__),
    selectablesPerRow_(selectablesPerRow__),
    selectableWidth_(selectableWidth__),
    selectableHeight_(selectableHeight__),
    zoom_(1.00),
    highlightedSelectableBoxEnabled_(false),
    highlightedSelectableIndex_(0),
    pickedSelectableBoxEnabled_(true),
    pickedSelectableIndex_(0),
    gridEnabled_(true),
    grid_(selectableWidth_,
               1,
               Tcolor(0x88, 0x88, 0x88, Tcolor::fullAlphaOpacity),
               0, 0,
               0, 0,
               false) { };
  
IndexedPickerScene::~IndexedPickerScene() {
  delete itemPickedFunctor_;
}
  
void IndexedPickerScene::render(Graphic& dst,
                      int xOffset, int yOffset) {
/*  if ((nativeBuffer_.w() != srcbox.w())
      || (nativeBuffer_.h() != srcbox.h())) {
    nativeBuffer_ = Graphic(srcbox.w(), srcbox.h());
  } */
  
  Graphic& graphic = nativeBuffer_.graphicAtSize(nativeW(), nativeH());
                          
  renderNative(graphic);
  
  int scaledW = graphic.w() * zoom_;
  int scaledH = graphic.h() * zoom_;
  
//  nativeBuffer_.scale(dst,
//                       Box(0, 0,
//                           scaledW,
//                           scaledH));
  
  dst.scaleAndCopy(graphic,
                   Box(xOffset, yOffset, scaledW, scaledH),
                   Graphic::noTransUpdate);
      
  if (gridEnabled_) {
    grid_.render(dst,
                      Box(xOffset,
                          yOffset,
                          scaledW,
                          scaledH),
                      zoom_);
  }
  
  if ((highlightedSelectableBoxEnabled_)) {
    dst.drawRectBorder(selectableIndexToDrawX(highlightedSelectableIndex_)
                         + xOffset,
                       selectableIndexToDrawY(highlightedSelectableIndex_)
                         + yOffset,
                       selectableWidth_ * zoom_,
                       selectableHeight_ * zoom_,
                       Tcolor(0xFF, 0x00, 0x00, Tcolor::fullAlphaOpacity),
                       2);
  }
  
  if ((pickedSelectableBoxEnabled_)) {
    dst.drawRectBorder(selectableIndexToDrawX(pickedSelectableIndex_)
                         + xOffset,
                       selectableIndexToDrawY(pickedSelectableIndex_)
                         + yOffset,
                       selectableWidth_ * zoom_,
                       selectableHeight_ * zoom_,
                       Tcolor(0xFF, 0xFF, 0x00, Tcolor::fullAlphaOpacity),
                       2);
  }
}
  
void IndexedPickerScene::renderNative(Graphic& dst) {
  dst.clearTransparentBlack();
  
  int drawX = 0;
  int drawY = 0;
  int total = 0;
  for (int j = 0; j < selectablesPerColumn(); j++) {
    for (int i = 0; i < selectablesPerRow_; i++) {
      if (total >= totalSelectables_) {
        break;
      }
    
      drawSelectable(dst,
                     drawX, drawY,
                     indexOfSelectableAt(i, j));
      
      drawX += selectableWidth_;
      ++total;
    }
    
    drawX = 0;
    drawY += selectableHeight_;
  }
}
                              
bool IndexedPickerScene::indexIsSelectable(int selectableIndex) {
  return true;
}

void IndexedPickerScene::enterEventCallback(
  void* vThis) {
  ((IndexedPickerScene*)(vThis))->enterEvent();
}

void IndexedPickerScene::leaveEventCallback(
  void* vThis) {
  ((IndexedPickerScene*)(vThis))->leaveEvent();
}

void IndexedPickerScene::mouseMoveEventCallback(
  void* vThis, InputEventData event) {
  ((IndexedPickerScene*)(vThis))->mouseMoveEvent(event);
}

void IndexedPickerScene::mousePressEventCallback(
  void* vThis, InputEventData event) {
  ((IndexedPickerScene*)(vThis))->mousePressEvent(event);
}

void IndexedPickerScene::mouseReleaseEventCallback(
  void* vThis, InputEventData event) {
  ((IndexedPickerScene*)(vThis))->mouseReleaseEvent(event);
}

void IndexedPickerScene::mouseDoubleClickEventCallback(
  void* vThis, InputEventData event) {
  ((IndexedPickerScene*)(vThis))->mouseDoubleClickEvent(event);
}
  
void IndexedPickerScene::enterEvent() {
  highlightedSelectableBoxEnabled_ = true;
}

void IndexedPickerScene::leaveEvent() {
  highlightedSelectableBoxEnabled_ = false;
}

void IndexedPickerScene::mouseMoveEvent(InputEventData eventData) {
  int posIndex = drawPosToSelectableIndex(
                                  eventData.x(), eventData.y());
  
  if ((posIndex >= totalSelectables_)
        || (posIndex < 0)) {
//    highlightedSelectableBoxEnabled_ = false;
    return;
//    pickedSelectableIndex_ = totalSelectables_ - 1;
  }
  
  if ((!indexIsSelectable(posIndex))) {
    highlightedSelectableBoxEnabled_ = false;
  }
  else {
    highlightedSelectableBoxEnabled_ = true;
  }
  
  highlightedSelectableIndex_ = drawPosToSelectableIndex(
                                  eventData.x(), eventData.y());
}

void IndexedPickerScene::mousePressEvent(InputEventData eventData) {
  if (!eventData.mouseLeftButton()) {
    return;
  }
  
  int posIndex = drawPosToSelectableIndex(
                                  eventData.x(), eventData.y());
  
  if ((posIndex >= totalSelectables_)
        || (posIndex < 0)
        || (!indexIsSelectable(posIndex))) {
    return;
//    pickedSelectableIndex_ = totalSelectables_ - 1;
  }
  
  pickedSelectableBoxEnabled_ = true;
  pickedSelectableIndex_ = posIndex;
  (*itemPickedFunctor_)(posIndex);
}

void IndexedPickerScene::mouseReleaseEvent(InputEventData eventData) {
  
}

void IndexedPickerScene::mouseDoubleClickEvent(InputEventData eventData) {
  
}
  
int IndexedPickerScene::highlightedIndex() {
  return highlightedSelectableIndex_;
}

int IndexedPickerScene::pickedIndex() {
  return pickedSelectableIndex_;
}
  
void IndexedPickerScene::clearHighlightedBox() {
  highlightedSelectableBoxEnabled_ = false;
}

void IndexedPickerScene::clearPickedBox() {
  pickedSelectableBoxEnabled_ = false;
}
  
void IndexedPickerScene::enableHighlightedBox() {
  highlightedSelectableBoxEnabled_ = true;
}

void IndexedPickerScene::enablePickedBox() {
  pickedSelectableBoxEnabled_ = true;
}
  
void IndexedPickerScene::pickIndex(int index) {
  pickedSelectableBoxEnabled_ = true;
  pickedSelectableIndex_ = index;
}

void IndexedPickerScene::highlightIndex(int index) {
  highlightedSelectableBoxEnabled_ = true;
  highlightedSelectableIndex_ = index;
}
  
bool IndexedPickerScene::gridEnabled() const {
  return gridEnabled_;
}
 
void IndexedPickerScene::setGridEnabled(bool gridEnabled__) {
  gridEnabled_ = gridEnabled__;
}

int IndexedPickerScene::selectablesPerColumn() const {
  // Account for "trailing" items on the last row when the items
  // don't fill the entire space
  return (totalSelectables_ / selectablesPerRow_)
    + (((totalSelectables_ % selectablesPerRow_) == 0)
      ? 0
      : 1);
}
  
double IndexedPickerScene::zoom() {
  return zoom_;
}

void IndexedPickerScene::setZoom(double zoom__) {
  zoom_ = zoom__;
}
  
int IndexedPickerScene::realPosToSelectableIndex(int realX, int realY) {
  return ((realY / selectableHeight_) * selectablesPerRow_)
            + (realX / selectableWidth_);
}

int IndexedPickerScene::drawPosToSelectableIndex(int drawX, int drawY) {
  return realPosToSelectableIndex(drawX / zoom_,
                                  drawY / zoom_);
}

int IndexedPickerScene::selectableIndexToDrawX(int index) {
  return selectableIndexToNativeDrawX(index) * zoom_;
}

int IndexedPickerScene::selectableIndexToDrawY(int index) {
  return selectableIndexToNativeDrawY(index) * zoom_;
}

int IndexedPickerScene::selectableIndexToNativeDrawX(int index) {
  return (index % selectablesPerRow_) * selectableWidth_;
}

int IndexedPickerScene::selectableIndexToNativeDrawY(int index) {
  return (index / selectablesPerRow_) * selectableHeight_;
}
  
int IndexedPickerScene::nativeW() const {
  return selectablesPerRow_ * selectableWidth_;
}

int IndexedPickerScene::nativeH() const {
  return selectablesPerColumn() * selectableHeight_;
}
  
int IndexedPickerScene::realW() const {
  return nativeW() * zoom_;
}

int IndexedPickerScene::realH() const {
  return nativeH() * zoom_;
}
  
void IndexedPickerScene::setItemPickedFunctor(IndexedPickerSceneItemPicked&
    itemPickedFunctor__) {
  delete itemPickedFunctor_;
  itemPickedFunctor_ = &itemPickedFunctor__;
}
  
int IndexedPickerScene::totalSelectables() const {
  return totalSelectables_;
}


};
