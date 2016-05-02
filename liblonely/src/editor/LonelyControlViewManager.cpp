#include "editor/LonelyControlViewManager.h"
#include "editor/LonelyLevelView.h"
#include <cstdlib>
#include <iostream>

namespace Lonely {



LonelyControlViewManager
    ::LonelyControlViewManager(LonelyLevelView& levelView__)
  : levelView_(&levelView__),
    toolMode_(waitingForNodeClick),
    nodeSelected_(false),
    cloneClick_(false),
    selectedNodePatternIndex_(0),
    draggedNodePatternIndex_(0),
    callbackObj_(NULL),
    nodeSelectedCallback_(NULL),
    nodeDeselectedCallback_(NULL) { };
  
void LonelyControlViewManager
    ::setCallbacks(void* callbackObj__,
                  void (*nodeSelectedCallback__)
                    (void*,TokenIndexCollection&),
                  void (*nodeDeselectedCallback__)(void*)) {
  callbackObj_ = callbackObj__;
  nodeSelectedCallback_ = nodeSelectedCallback__;
  nodeDeselectedCallback_ = nodeDeselectedCallback__;
}
  
void LonelyControlViewManager
    ::enterEvent(void* levelViewV) {
//  std::cout << "enter" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  
}

void LonelyControlViewManager
		::leaveEvent(void* levelViewV) {
//  std::cout << "leave" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  
}

void LonelyControlViewManager
		::mouseMoveEvent(void* levelViewV, InputEventData event) {
//  std::cout << "move" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  levelView.controlManager.handleStandardMove(event);
/*  switch (levelView.controlManager.toolMode_) {
  case waitingForNodeRelease:
    break;
  default:
    
    break;
  } */
}

void LonelyControlViewManager
		::mousePressEvent(void* levelViewV, InputEventData event) {
//  std::cout << "press" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  levelView.controlManager.handleStandardClick(event);
  
/*  switch (levelView.controlManager.toolMode_) {
  case waitingForNodeClick:
    break;
  default:
    
    break;
  } */
}

void LonelyControlViewManager
		::mouseReleaseEvent(void* levelViewV, InputEventData event) {
//  std::cout << "release" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  levelView.controlManager.handleStandardRelease(event);
  
/*  switch (levelView.controlManager.toolMode_) {
  case waitingForNodeRelease:
    break;
  default:
    
    break;
  } */
}

void LonelyControlViewManager
		::mouseDoubleClickEvent(void* levelViewV, InputEventData event) {
//  std::cout << "doubleclick" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  
}
  
bool LonelyControlViewManager
		::isTouchingNode(InputEventData event,
                    int targetDrawX) {
  if ((isTouchingNodeInX(event, targetDrawX))
      && (isTouchingNodeInY(event, targetDrawX))) {
    return true;
  }
  
  return false;
}
  
bool LonelyControlViewManager
		::isTouchingNodeInX(InputEventData event,
                      int targetDrawX) {
  // If zoom is 100% or more, nodes display at a fixed (zoom-independent)
  // width and height. If zoom is less than 100%, they scale down horizontally
  // but not vertically to fit.
  if (levelView_->zoom_ >= 1.00) {
    if ((event.x() >= targetDrawX
                          + (LonelyLevelView::controlViewNodeXOffset_))
        && (event.x() < targetDrawX
                            + ((LonelyLevelView::controlViewNodeXOffset_
                                + LonelyLevelView::controlViewNodeW_)))
        ) {
      return true;
    }
  }
  else {
    if ((event.x() >= targetDrawX
                          + (LonelyLevelView::controlViewNodeXOffset_
                                  * levelView_->zoom()))
        && (event.x() < targetDrawX
                            + ((LonelyLevelView::controlViewNodeXOffset_
                              + LonelyLevelView::controlViewNodeW_)
                                  * levelView_->zoom()))
        ) {
      return true;
    }
  }
  
  return false;
}
  
bool LonelyControlViewManager
		::isTouchingNodeInY(InputEventData event,
                    int targetDrawX) {
  // If zoom is 100% or more, nodes display at a fixed (zoom-independent)
  // width and height. If zoom is less than 100%, they scale down horizontally
  // but not vertically to fit.
  if (levelView_->zoom_ >= 1.00) {
    if ((event.y() >= LonelyLevelView::controlViewNodeYOffset_)
        && (event.y() < (LonelyLevelView::controlViewNodeYOffset_
                          + LonelyLevelView::controlViewNodeH_))) {
      return true;
    }
  }
  else {
    if ((event.y() >= LonelyLevelView::controlViewNodeYOffset_)
        && (event.y() < (LonelyLevelView::controlViewNodeYOffset_
                          + LonelyLevelView::controlViewNodeH_))) {
      return true;
    }
  }
  
  return false;
}
  
int LonelyControlViewManager::getTargetIndex(InputEventData event) {
  // Convert coordinates to native level position so we can pass them
  // to findTargetPatternIndex()
  InputEventData nativeEvent(event);
  nativeEvent.morphCoordinates(levelView_->camX_,
                         0,
                         levelView_->zoom_);
  
  // Find the corresponding pattern index in the level data.
  // We have to offset in x by the offset of the node boxes, or else
  // we might look up the wrong pattern if nodes are set up to straddle
  // patterns (which they currently are).
  return (levelView_->findTargetPatternIndex(
    nativeEvent.x() - LonelyLevelView::controlViewNodeXOffset_,
    nativeEvent.y() - LonelyLevelView::controlViewNodeYOffset_));
}
  
int LonelyControlViewManager::getTargetDrawX(InputEventData event) {
  int targetIndex = getTargetIndex(event);
  return levelView_->patternDrawX(targetIndex);
}
  
void LonelyControlViewManager
		::handleStandardClick(InputEventData event) {
  int targetIndex = getTargetIndex(event);
  int targetDrawX = levelView_->patternDrawX(targetIndex);
  
  // Check if the cursor is touching the node square for the pattern (using
  // the non-native click coordinates)
  bool touchingNode = isTouchingNode(event,
                                     targetDrawX);
  
  switch (toolMode_) {
  case waitingForNodeClick:
    if (event.mouseLeftButton()) {
      if (!touchingNode) {
        deselectNode();
      }
      else {
        selectNode(targetIndex);
        toolMode_ = waitingForNodeRelease;
        draggedNodePatternIndex_ = selectedNodePatternIndex_;
        
        // If Control is pressed, this is a clone instead of a move
        if (event.control()) {
          cloneClick_ = true;
        }
        else {
          cloneClick_ = false;
        }
      }
    }
    else if (event.mouseMiddleButton()) {
      if (touchingNode) {
        // Never delete the first node
        if ((targetIndex
              == levelView_->level_->areaData().findStartingPattern())) {
          return;
        }
        
        deselectNode();
      
        // If node was for base pattern, update base pattern index
        // to account for removed elements
        if ((targetIndex == levelView_->patternPos_)) {
          int nodeSize = levelView_->level_->areaData()
                              .nodeEntriesAtIndex(targetIndex);
          levelView_->patternPos_
            = (levelView_->patternPos_ - nodeSize);
        }
        
        levelView_->level_->areaData().deleteNode(targetIndex);
        
        if (levelView_->spawnsChangedCallback_ != NULL) {
          levelView_->spawnsChangedCallback_(
            levelView_->spawnsChangedCallbackObj_);
        }
      }
    }
    break;
  default:
    break;
  }
  
  
}
  
void LonelyControlViewManager::handleStandardMove(InputEventData event) {
  int targetIndex = getTargetIndex(event);
  int targetDrawX = levelView_->patternDrawX(targetIndex);
  
  switch (toolMode_) {
  case waitingForNodeRelease:
    if (event.mouseLeftButton()) {
      if (!(isTouchingNodeInX(event, targetDrawX))) {
        return;
      }
      
      draggedNodePatternIndex_ = targetIndex;
    }
    break;
  default:
    break;
  }
}
  
void LonelyControlViewManager
		::handleStandardRelease(InputEventData event) {
  int targetIndex = getTargetIndex(event);
  int targetDrawX = levelView_->patternDrawX(targetIndex);
  
  switch (toolMode_) {
  case waitingForNodeRelease:
    if (!(event.mouseLeftButton())) {
      toolMode_ = waitingForNodeClick;
      
      if (draggedNodePatternIndex_ == selectedNodePatternIndex_) {
        return;
      }
      
      // If node was for base pattern, update base pattern index
      // to account for removed elements
      if (!(cloneClick_)
          && (selectedNodePatternIndex_ == levelView_->patternPos_)) {
        int nodeSize = levelView_->level_->areaData()
                            .nodeEntriesAtIndex(selectedNodePatternIndex_);
        levelView_->patternPos_
          = (levelView_->patternPos_ - nodeSize);
//        levelView_->resetToPattern(
//          levelView_->level_->areaData().findNearbyStandardPatternIndex(
//            levelView_->patternPos_));
      }
      else if ((draggedNodePatternIndex_ == levelView_->patternPos_)) {
        int nodeSize = levelView_->level_->areaData()
                            .nodeEntriesAtIndex(selectedNodePatternIndex_);
        levelView_->patternPos_
          = (levelView_->patternPos_ + nodeSize);
      }
      
      int movedPos;
      if (cloneClick_) {
        // Clone the dragged node
        movedPos =
            levelView_->level_->areaData().cloneNode(draggedNodePatternIndex_,
                                    selectedNodePatternIndex_);
      }
      else {
        // Move the dragged node to the target position
        movedPos =
            levelView_->level_->areaData().moveNode(draggedNodePatternIndex_,
                                    selectedNodePatternIndex_);
      }
      
      selectNode(movedPos);
      if (levelView_->spawnsChangedCallback_ != NULL) {
        levelView_->spawnsChangedCallback_(
          levelView_->spawnsChangedCallbackObj_);
      }
      
//      std::cout << "drop at " << draggedNodePatternIndex_ << std::endl;
    }
    break;
  default:
    break;
  }
}
  
void LonelyControlViewManager
		::selectNode(int index) {
  nodeSelected_ = true;
  toolMode_ = waitingForNodeClick;
  selectedNodePatternIndex_ = index;
  
  if (nodeSelectedCallback_ != NULL) {
    // Gather all the tokens in the node into a collection
    int nodeStart
      = levelView_->level_->areaData()
          .findPreviousStandardPatternIndexNext(index);
    
    TokenIndexCollection tokens;
    for (int i = nodeStart; i < index; i++) {
//      tokens.push_back(&(levelView_->level_->areaData().patternToken(i)));
//      if (levelView_->level_->areaData().patternToken(i).type()
//            != LaylaPatternTypes::spawnPoint) {
        tokens.push_back(i);
//      }
    }
    
    nodeSelectedCallback_(callbackObj_, tokens);
  }
}

void LonelyControlViewManager
		::deselectNode() {
  nodeSelected_ = false;
  toolMode_ = waitingForNodeClick;
  
  if (nodeDeselectedCallback_ != NULL) {
    nodeDeselectedCallback_(callbackObj_);
  }
}


};
