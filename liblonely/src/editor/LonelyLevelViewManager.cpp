#include "editor/LonelyLevelViewManager.h"
#include "editor/LonelyLevelView.h"
#include <iostream>
#include <algorithm>
#include <utility>

namespace Lonely {


LonelyLevelViewManager::LonelyLevelViewManager(LonelyLevelView& levelView__)
  : levelView_(&levelView__),
    toolsActive_(false),
    mouseIsIn_(false),
    activeTool_(patternModify),
    pio_overwrite_(true),
    pio_sourcePatternNum_(0),
    pio_targetPatternIndex_(0),
    cpy_mode_(cpy_waitingForSelectClick),
    cpy_copyNodes_(false),
    cpy_patternBaseIndex_(0),
    cpy_patternBaseIndexReal_(0),
    cpy_patternEndIndex_(0),
    cpy_patternOrigIndex_(0),
    mov_mode_(mov_waitingForSelectClick),
    mov_patternBaseIndex_(0),
    mov_patternBaseIndexReal_(0),
    mov_patternEndIndex_(0),
    mov_patternDragIndex_(0) { };
  
void LonelyLevelViewManager::resetTools() {
  toolsActive_ = mouseIsIn_;
  cpy_mode_ = cpy_waitingForSelectClick;
  mov_mode_ = mov_waitingForSelectClick;
}
  
void LonelyLevelViewManager::changeTool(Tool tool__) {
  resetTools();
  
  activeTool_ = tool__;
}

void LonelyLevelViewManager
    ::enterEvent(void* levelViewV) {
//  std::cout << "enter" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  levelView.levelManager.toolsActive_ = true;
  levelView.levelManager.mouseIsIn_ = true;
}

void LonelyLevelViewManager
		::leaveEvent(void* levelViewV) {
//  std::cout << "leave" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  levelView.levelManager.toolsActive_ = false;
  levelView.levelManager.mouseIsIn_ = false;
}

void LonelyLevelViewManager
		::mouseMoveEvent(void* levelViewV, InputEventData event) {
//  std::cout << "move" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  
  event.morphCoordinates(levelView.camX_,
                         levelView.camY_,
                         levelView.zoom_);
  
  if (levelView.levelManager.toolsActive_) {
    switch (levelView.levelManager.activeTool_) {
    case LonelyLevelViewManager::patternModify:
      levelView.levelManager.pio_targetPatternIndex_
          = levelView.findTargetPatternIndex(event.x(), event.y());
      
      // Allow click-and-drag to overwrite patterns, but not to insert
      // (since doing it with insert is really confusing)
      if (levelView.levelManager.pio_overwrite_) {
        levelView.levelManager.handlePatternModifyClick(event);
      }
      break;
    case LonelyLevelViewManager::patternCopy:
      levelView.levelManager.handlePatternCopyMove(event);
      break;
    case LonelyLevelViewManager::patternMove:
      levelView.levelManager.handlePatternMoveMove(event);
      break;
    default:
      break;
    }
  }
      
//  std::cout << levelView.levelManager.pio_targetPatternIndex_ << std::endl;
}

void LonelyLevelViewManager
		::mousePressEvent(void* levelViewV, InputEventData event) {
//  std::cout << "press" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  // Clear node selection whenever mouse click occurs (we might change
  // the pattern data, invalidating the referenced nodes)
  levelView.controlManager.deselectNode();
  
  event.morphCoordinates(levelView.camX_,
                         levelView.camY_,
                         levelView.zoom_);
  
  if (levelView.levelManager.toolsActive_) {
    switch (levelView.levelManager.activeTool_) {
    case LonelyLevelViewManager::patternModify:
      levelView.levelManager.handlePatternModifyClick(event);
      break;
    case LonelyLevelViewManager::patternCopy:
      levelView.levelManager.handlePatternCopyClick(event);
      break;
    case LonelyLevelViewManager::patternMove:
      levelView.levelManager.handlePatternMoveClick(event);
      break;
    default:
      break;
    }
  }
}

void LonelyLevelViewManager
		::mouseReleaseEvent(void* levelViewV, InputEventData event) {
//  std::cout << "release" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  event.morphCoordinates(levelView.camX_,
                         levelView.camY_,
                         levelView.zoom_);
  
  if (levelView.levelManager.toolsActive_) {
    switch (levelView.levelManager.activeTool_) {
    case LonelyLevelViewManager::patternCopy:
      levelView.levelManager.handlePatternCopyRelease(event);
      break;
    case LonelyLevelViewManager::patternMove:
      levelView.levelManager.handlePatternMoveRelease(event);
      break;
    default:
      break;
    }
  }
}

void LonelyLevelViewManager
		::mouseDoubleClickEvent(void* levelViewV, InputEventData event) {
//  std::cout << "doubleclick" << std::endl;
  LonelyLevelView& levelView = *(static_cast<LonelyLevelView*>(levelViewV));
  
  // Clear node selection whenever mouse click occurs (we might change
  // the pattern data, invalidating the referenced nodes)
  levelView.controlManager.deselectNode();
  
  event.morphCoordinates(levelView.camX_,
                         levelView.camY_,
                         levelView.zoom_);
  
  if (levelView.levelManager.toolsActive_) {
    switch (levelView.levelManager.activeTool_) {
/*    case LonelyLevelViewManager::patternModify:
      levelView.levelManager.handlePatternModifyClick(event);
      break;
    case LonelyLevelViewManager::patternCopy:
      levelView.levelManager.handlePatternCopyClick(event);
      break; */
    case LonelyLevelViewManager::patternMove:
      levelView.levelManager.handlePatternMoveDoubleClick(event);
      break;
    default:
      break;
    }
  }
  
}
  
void LonelyLevelViewManager
		::handlePatternModifyClick(InputEventData event) {
  if (event.mouseLeftButton()) {
    if (event.control()) {
      // Switch to pattern copy tool and start copy
      changeTool(patternCopy);
      handlePatternCopyClick(event);
    }
    // Go to pattern move tool on shift+click
    else if (event.shift()) {
      changeTool(patternMove);
      doPatternMoveSelectClick(event);
      return;
    }
    else if (pio_overwrite_) {
      // Overwrite target
      LaylaPatternToken& token
          = levelView_->level_->areaData().patternToken(
              pio_targetPatternIndex_);
      StandardPatternToken& patternToken
        = dynamic_cast<StandardPatternToken&>(token);
      patternToken.setPatternNum(
        pio_sourcePatternNum_);
    }
    else {
      // Insert at target
      levelView_->level_->areaData().insertStandardPatternToken(
        pio_targetPatternIndex_,
        new StandardPatternToken(
          pio_sourcePatternNum_));
      
      if (levelView_->spawnsChangedCallback_ != NULL) {
        levelView_->spawnsChangedCallback_(
          levelView_->spawnsChangedCallbackObj_);
      }
    }
  }
  else if (event.mouseRightButton()) {
    // Pick pattern under mouse as source
    LaylaPatternToken& token
        = levelView_->level_->areaData().patternToken(
            pio_targetPatternIndex_);
    StandardPatternToken& patternToken
      = dynamic_cast<StandardPatternToken&>(token);
    pio_sourcePatternNum_ = patternToken.patternNum();
  }
  else if (event.mouseMiddleButton()) {
    // Delete pattern under mouse
    
    // Never delete the first pattern in the data
    if (levelView_->levelManager.pio_targetPatternIndex_
          == levelView_->level_->areaData().findStartingPattern()) {
      return;
    }
    
    // Deal with changes to base pattern
    if (levelView_->levelManager.pio_targetPatternIndex_
          == levelView_->patternPos_) {
      int nodeSize = levelView_->level_->areaData().nodeEntriesAtIndex(
                          levelView_->levelManager.pio_targetPatternIndex_);
      levelView_->patternPos_ = (levelView_->patternPos_ - nodeSize - 1);
      // rrrrrrg
      levelView_->resetToPattern(levelView_->patternPos_);
    }
    
    levelView_->level_->areaData().deletePatternsAndNodesAt(
      levelView_->levelManager.pio_targetPatternIndex_, 1);
    
    if (levelView_->spawnsChangedCallback_ != NULL) {
      levelView_->spawnsChangedCallback_(
        levelView_->spawnsChangedCallbackObj_);
    }
  }
}
  
void LonelyLevelViewManager
		::handlePatternCopyClick(InputEventData event) {
  switch (cpy_mode_) {
  case cpy_waitingForSelectClick:
    if (event.mouseLeftButton()) {
      doPatternCopyCopyClick(event);
    }
    break;
  case cpy_waitingForPasteClick:
    if (event.mouseLeftButton()) {
      // Restart selection process on ctrl+click
      if (event.control()) {
        doPatternCopyCopyClick(event);
        return;
      }
      // Go to pattern move tool on shift+click
      else if (event.shift()) {
        changeTool(patternMove);
        doPatternMoveSelectClick(event);
        return;
      }
      // Copy-overwrite
      else if (pio_overwrite_) {
        // Node overwrite disabled due to being buggy as hell
//        if (cpy_copyNodes_) {
//          levelView_->overwritePatternsAndNodesAt(
//            cpy_patternBaseIndex_, cpy_copyPatterns_);
//        }
//        else {
          levelView_->overwritePatternsAt(
            cpy_patternBaseIndex_, cpy_copyPatterns_);
//        }
    
        if (levelView_->spawnsChangedCallback_ != NULL) {
          levelView_->spawnsChangedCallback_(
            levelView_->spawnsChangedCallbackObj_);
        }
      }
      // Copy-insert
      else {
        if (cpy_copyNodes_) {
          levelView_->level_->areaData().insertPatternsAndNodesAtPure(
            cpy_patternBaseIndex_,
            cpy_patternOrigIndex_,
            cpy_copyPatterns_);
        }
        else {
          levelView_->insertPatternsAt(
            cpy_patternBaseIndex_, cpy_copyPatterns_);
        }
    
        if (levelView_->spawnsChangedCallback_ != NULL) {
          levelView_->spawnsChangedCallback_(
            levelView_->spawnsChangedCallbackObj_);
        }
      }
      
/*    cpy_patternBaseIndex_
        = levelView_->findTargetPatternIndex(event.x(), event.y());
      cpy_patternEndIndex_ = cpy_patternBaseIndex_;
      cpy_mode_ = cpy_waitingForSelectRelease; */
    }
    else if (event.mouseRightButton()) {
      // Switch to pattern modify tool
      activeTool_ = patternModify;
      // Set source pattern (pio_targetPatternIndex_ is only updated
      // when the modify tool is active)
      pio_targetPatternIndex_
          = levelView_->findTargetPatternIndex(event.x(), event.y());
      handlePatternModifyClick(event);
    }
    break;
  default:
    break;
  }
}
  
void LonelyLevelViewManager
		::handlePatternCopyMove(InputEventData event) {
  
  switch (cpy_mode_) {
  case cpy_waitingForSelectRelease:
    cpy_patternEndIndex_
      = levelView_->findTargetPatternIndex(event.x(), event.y());
    break;
  case cpy_waitingForPasteClick:
    cpy_patternBaseIndex_
      = levelView_->findTargetPatternIndex(event.x(), event.y());
    cpy_patternBaseIndexReal_
      = levelView_
          ->findPreviousStandardPatternIndexNext(cpy_patternBaseIndex_);
    
    if (pio_overwrite_) {
      handlePatternCopyClick(event);
    }
    break;
  default:
    break;
  }
}

void LonelyLevelViewManager
		::handlePatternCopyRelease(InputEventData event) {
  switch (cpy_mode_) {
  case cpy_waitingForSelectRelease:
    if (!(event.mouseLeftButton())) {
      // Copy selection to buffer
      
      // Clear existing buffer contents
      cpy_copyPatterns_.deleteData();
      cpy_copyPatterns_.clear();
      
      // Always copy left to right
      if (cpy_patternEndIndex_ < cpy_patternBaseIndex_) {
        std::swap(cpy_patternBaseIndex_, cpy_patternEndIndex_);
      }
      
      int pos = cpy_patternBaseIndex_;
      while ((pos < levelView_->level_->areaData().numPatternTokens())
             && (pos <= cpy_patternEndIndex_)) {
        // Get the position of the node data associated with the current
        // pattern
        int startPos
          = levelView_->findPreviousStandardPatternIndexNext(pos);
        
        // Clone the node and standard pattern data
        for (int j = startPos; j <= pos; j++) {
          LaylaPatternToken& token
            = levelView_->level_->areaData().patternToken(j);
          
          // Ignore spawn point tokens
//          if (token.type() != LaylaPatternTypes::spawnPoint) {
            cpy_copyPatterns_.push_back(
              levelView_->level_->areaData().patternToken(j).cloneNew());
//          }
        }
        
        // Go to next standard pattern
        pos = levelView_->findNextStandardPatternIndex(pos);
      }
      
      // Go to paste mode
      cpy_patternOrigIndex_ = cpy_patternBaseIndex_;
      cpy_patternBaseIndex_
        = levelView_->findTargetPatternIndex(event.x(), event.y());
      cpy_patternBaseIndexReal_
        = levelView_
          ->findPreviousStandardPatternIndexNext(cpy_patternBaseIndex_);
      cpy_mode_ = cpy_waitingForPasteClick;
    }
    break;
  default:
    break;
  }
}

void LonelyLevelViewManager
		::doPatternCopyCopyClick(InputEventData event) {
	cpy_patternBaseIndex_
    = levelView_->findTargetPatternIndex(event.x(), event.y());
  cpy_patternEndIndex_ = cpy_patternBaseIndex_;
  cpy_mode_ = cpy_waitingForSelectRelease;
}
  
void LonelyLevelViewManager
		::handlePatternMoveClick(InputEventData event) {
  switch (mov_mode_) {
  case mov_waitingForSelectClick:
    if (event.mouseLeftButton()) {
      doPatternMoveSelectClick(event);
    }
    break;
  case mov_waitingForMoveClick:
    if (event.mouseLeftButton()) {
      // Restart selection process on shift+click
      if (event.shift()) {
        doPatternMoveSelectClick(event);
        return;
      }
      // Switch to pattern copy on ctrl+click
      else if (event.control()) {
        changeTool(patternCopy);
        doPatternCopyCopyClick(event);
        return;
      }
      // DO FUCKING NOTHING, THIS CODE IS GODDAMN HOPELESS, JUST FUCK OFF
      return;
      
      mov_patternDragIndex_
        = levelView_->findTargetPatternIndex(event.x(), event.y());
      
      mov_mode_ = mov_waitingForMoveRelease;

/*      int newBase = levelView_->level_->areaData().movePatterns(
        levelView_->findTargetPatternIndex(event.x(), event.y()),
        mov_patternBaseIndex_,
        mov_patternEndIndex_ - mov_patternBaseIndex_);
      int sz = mov_patternEndIndex_ - mov_patternBaseIndex_;
      mov_patternBaseIndex_ = newBase;
      mov_patternEndIndex_ = mov_patternBaseIndex_ + sz;
      
      
      if (levelView_->spawnsChangedCallback_ != NULL) {
        levelView_->spawnsChangedCallback_(
          levelView_->spawnsChangedCallbackObj_);
      } */
    }
    else if (event.mouseMiddleButton()) {
      // Delete selection
      
      // Never delete the first pattern in the data
      int startingPatternIndex
        = levelView_->level_->areaData().findStartingPattern();
      if (mov_patternBaseIndex_ == startingPatternIndex) {
        mov_patternBaseIndex_
          = levelView_->level_->areaData().findNextStandardPatternIndex(
                startingPatternIndex);
        if (mov_patternBaseIndex_ >= mov_patternEndIndex_) {
          return;
        }
      }
      
      levelView_->level_->areaData().deletePatternRange(
        mov_patternBaseIndex_, mov_patternEndIndex_);
      mov_mode_ = mov_waitingForSelectClick;
      
      levelView_->resetToPattern(
        levelView_->level_->areaData().
          findNearbyStandardPatternIndex(levelView_->patternPos_));
      
      if (levelView_->spawnsChangedCallback_ != NULL) {
        levelView_->spawnsChangedCallback_(
          levelView_->spawnsChangedCallbackObj_);
      }
    }
    else if (event.mouseRightButton()) {
      // Switch to pattern modify tool
      activeTool_ = patternModify;
      // Set source pattern (pio_targetPatternIndex_ is only updated
      // when the modify tool is active)
      pio_targetPatternIndex_
          = levelView_->findTargetPatternIndex(event.x(), event.y());
      handlePatternModifyClick(event);
    }
    break;
  default:
    break;
  }
}
  
void LonelyLevelViewManager
		::handlePatternMoveDoubleClick(InputEventData event) {
  switch (mov_mode_) {
  case mov_waitingForSelectClick:
//    if (event.mouseLeftButton()) {
//      doPatternMoveSelectClick(event);
//    }
    break;
  case mov_waitingForMoveClick:
    if (event.mouseLeftButton()) {
      // Restart selection process on ctrl+click
      if (event.control()) {
        doPatternMoveSelectClick(event);
        return;
      }
      
//      mov_mode_ = mov_waitingForMoveRelease;

      moveSelection(
          levelView_->findTargetPatternIndex(event.x(), event.y()));
    }
    else if (event.mouseRightButton()) {
      // Switch to pattern modify tool
      activeTool_ = patternModify;
      // Set source pattern (pio_targetPatternIndex_ is only updated
      // when the modify tool is active)
      pio_targetPatternIndex_
          = levelView_->findTargetPatternIndex(event.x(), event.y());
      handlePatternModifyClick(event);
    }
    break;
  default:
    break;
  }
}
  
void LonelyLevelViewManager
		::handlePatternMoveMove(InputEventData event) {
  
  switch (mov_mode_) {
  case mov_waitingForSelectRelease:
    mov_patternEndIndex_
      = levelView_->findTargetPatternIndex(event.x(), event.y());
    break;
  case mov_waitingForMoveRelease:
    // Move
    {
    int target = levelView_->findTargetPatternIndex(event.x(), event.y());
    
    if (target == mov_patternDragIndex_) {
      return;
    }
    
//    std::cout << target << " " << mov_patternDragIndex_ << std::endl;
    
    // Count number of patterns between clicked location and current location
    int count = 0;
    int origTarget = target;
    int findTarget = target;
    int realTarget = mov_patternBaseIndex_;
    if (target > mov_patternDragIndex_) {
      while ((realTarget < levelView_->level_->areaData().numPatternTokens())
             && (findTarget > 0)
             && (findTarget > mov_patternDragIndex_)) {
        findTarget = levelView_->level_->areaData()
          .findPreviousStandardPatternIndex(findTarget);
        realTarget = levelView_->level_->areaData()
          .findNextStandardPatternIndex(realTarget);
        --count;
      }
    }
    else {
      while ((realTarget > 0)
             && (findTarget < levelView_->level_
                                ->areaData().numPatternTokens())
             && (findTarget < mov_patternDragIndex_)) {
        findTarget = levelView_->level_->areaData()
          .findNextStandardPatternIndex(findTarget);
        realTarget = levelView_->level_->areaData()
          .findPreviousStandardPatternIndex(realTarget);
        ++count;
      }
    }
    
    mov_patternDragIndex_ = origTarget;
    
//    std::cout << realTarget << std::endl;
    
    moveSelection(realTarget);
    
//    std::cout << mov_patternDragIndex_ << std::endl;
    
//    std::cout << mov_patternDragIndex_ << " " << target
//      << " " << count << std::endl;
    
    
//    mov_patternDragIndex_
//      = levelView_->findTargetPatternIndex(event.x(), event.y());
//    std::cout << target << std::endl;
    }
//    mov_patternEndIndex_
//      = levelView_->findTargetPatternIndex(event.x(), event.y());
    break;
  default:
    break;
  }
}

void LonelyLevelViewManager
		::handlePatternMoveRelease(InputEventData event) {
  switch (mov_mode_) {
  case mov_waitingForSelectRelease:
    if (!(event.mouseLeftButton())) {
      if (mov_patternEndIndex_ < mov_patternBaseIndex_) {
        std::swap(mov_patternBaseIndex_, mov_patternEndIndex_);
      }
      mov_patternBaseIndexReal_
        = levelView_
          ->findPreviousStandardPatternIndexNext(mov_patternBaseIndex_);
      
      // Go to paste mode
/*      mov_patternBaseIndex_
        = levelView_->findTargetPatternIndex(event.x(), event.y());
      mov_patternBaseIndexReal_
        = levelView_
          ->findPreviousStandardPatternIndexNext(mov_patternBaseIndex_); */
      mov_mode_ = mov_waitingForMoveClick;
    }
    break;
  case mov_waitingForMoveRelease:
    if (!(event.mouseLeftButton())) {
      mov_mode_ = mov_waitingForMoveClick;
    }
    break;
  default:
    break;
  }
}

void LonelyLevelViewManager
		::doPatternMoveSelectClick(InputEventData event) {
	mov_patternBaseIndex_
    = levelView_->findTargetPatternIndex(event.x(), event.y());
	mov_patternBaseIndexReal_
        = levelView_
          ->findPreviousStandardPatternIndexNext(mov_patternBaseIndex_);
  mov_patternEndIndex_ = mov_patternBaseIndex_;
  mov_mode_ = mov_waitingForSelectRelease;
}

void LonelyLevelViewManager
		::moveSelection(int target) {
//  std::cout << target << " " << mov_patternBaseIndex_
//    << " " << mov_patternEndIndex_ << std::endl;
//  int realTarget = levelView_->level_->areaData()
//                      .findPreviousStandardPatternIndexNext(target);
  int datStart = levelView_->level_->areaData()
    .findPreviousStandardPatternIndexNext(mov_patternBaseIndex_);
  int datSz = ((mov_patternEndIndex_ + 1) - datStart);
  int newBase = levelView_->level_->areaData().movePatterns(
    target,
    mov_patternBaseIndex_,
    mov_patternEndIndex_);
  int sz = mov_patternEndIndex_ - mov_patternBaseIndex_;
  mov_patternBaseIndex_ = newBase;
  mov_patternEndIndex_ = mov_patternBaseIndex_ + sz;
  
  // Moving data around the origin pattern causes all kinds of nasty issues.
  // The "nuclear option" used here is to simply reload the patterns
  // starting from 0.
  levelView_->resetToPattern(
    levelView_->level_->areaData().
      findNearbyStandardPatternIndex(levelView_->patternPos_));
  
  if (levelView_->spawnsChangedCallback_ != NULL) {
    levelView_->spawnsChangedCallback_(
      levelView_->spawnsChangedCallbackObj_);
  }
}

void LonelyLevelViewManager
		::doPatternMoveMoveClick(InputEventData event) {
	
}


};
