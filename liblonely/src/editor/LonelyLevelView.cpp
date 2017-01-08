#include "editor/LonelyLevelView.h"
#include "gamedata/LonelyLaylaMappings.h"
#include "structs/PngConversion.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace Lonely {


const Tcolor LonelyLevelView::backgroundColor_(
                0x00, 0x00, 0x00);
const Tcolor LonelyLevelView::controlNodeBorderColor_(
                0x88, 0x88, 0x88);
const Tcolor LonelyLevelView::controlNodeFillColor_(
                0xFF, 0xFF, 0xFF);
const Tcolor LonelyLevelView::leftJumpColor_(
                0xFF, 0x00, 0x00);
const Tcolor LonelyLevelView::rightJumpColor_(
                0x00, 0xFF, 0x00);

LonelyLevelView::LonelyLevelView(int levelNum__,
                  LaylaLevel& level__,
                  LaylaGraphics& graphics__,
                  LaylaPalettes& palettes__,
                  LaylaStaticMetatiles& staticMetatiles__)
    : levelManager(*this),
      controlManager(*this),
      level_(&level__),
      graphics_(&graphics__),
      palettes_(&palettes__),
      staticMetatiles_(&staticMetatiles__),
      levelNum_(levelNum__),
      camX_(0),
      camY_(0),
      camW_(0),
      camH_(0),
      zoom_(1.00),
      gridEnabled_(true),
      patternMarkersEnabled_(true),
      loopsEnabled_(true),
      spawnsEnabled_(true),
      viewType_(visual),
      patternPos_(0),
      patternSubX_(0),
      modifyBoxX_(0),
      modifyBoxY_(0),
      modifyBoxW_(0),
      modifyBoxH_(0),
      pasteBoxX_(0),
      pasteBoxY_(0),
      pasteBoxW_(0),
      pasteBoxH_(0),
      spawnsChangedCallbackObj_(NULL),
      spawnsChangedCallback_(NULL),
      levelCache_(camW_, camH_),
      controlCache_(camW_, camH_),
      grid_(LaylaMetatile::width,
            1,
            Tcolor(0x88, 0x88, 0x88, Tcolor::fullAlphaOpacity),
            0, levelYOffset_,
            0, levelNativeHeightReal_ + levelYOffset_,
            false) {
//  cacheMetatiles();
  changeLevel(levelNum_, level__);
}
  
void LonelyLevelView::changeLevel(int levelNum__, LaylaLevel& level__) {
  
  levelNum_ = levelNum__;
  level_ = &level__;
//  camX_ = 0;
  camY_ = 0;
//  patternPos_ = 0;
//  patternSubX_ = 0;
  levelManager.resetTools();
  seekStartingPattern();
  cacheMetatiles();
  
  levelManager.toolsActive_ = false;
  levelManager.pio_sourcePatternNum_ = 0;
  levelManager.pio_targetPatternIndex_ = patternPos_;
  
  controlManager.deselectNode();
  controlManager.toolMode_ = LonelyControlViewManager::waitingForNodeClick;
  
  if (spawnsChangedCallback_ != NULL) {
    spawnsChangedCallback_(spawnsChangedCallbackObj_);
  }
}
  
void LonelyLevelView::reloadRefresh() {
  cacheMetatiles();
}
  
int LonelyLevelView::levelViewWidth() const {
  return levelNativeWidth() * zoom_;
}

int LonelyLevelView::levelViewHeight() const {
  return levelNativeHeight() * zoom_;
}

int LonelyLevelView::levelNativeWidth() const {
//  std::cout << level_->objectSetNum() << std::endl;

  int total = 0;
  
  for (int i = 0; i < level_->areaData().numPatternTokens(); i++) {
    LaylaPatternToken& token = level_->areaData().patternToken(i);
    
    if (token.type() == LaylaPatternTypes::standardPattern) {
//      std::cout << i << std::endl;
      total += level_->patternDefinitions().pattern(
                  dynamic_cast<StandardPatternToken&>(token).patternNum())
                    .realWidth();
    }
  }
  
  return total;
}

int LonelyLevelView::levelNativeHeight() const {
  return levelNativeHeight_;
}

int LonelyLevelView::levelNum() const {
  return levelNum_;
}

void LonelyLevelView::seekCamX(int camX__) {
  if (camX_ == camX__) {
    return;
  }

  int distance = camX__ - camX_;
  
  int currentPatternWidth = currentPattern().realWidth();
  int currentPatternX = level_->areaData().findRealXOfIndex(
                            patternPos_,
                            level_->patternDefinitions());
//  int patternSubX = camX_ - currentPatternX;
  
  // Determine if we need to seek to a new pattern
  bool needSeek = false;
  if (distance > 0) {
    if (patternSubX_ + distance > currentPatternWidth) {
      needSeek = true;
    }
  }
  else {
    if (patternSubX_ + distance < 0) {
      needSeek = true;
    }
  }
  
  patternSubX_ += distance;
  
  // If we're still in the same pattern, we're done
  if (!needSeek) {
    camX_ = camX__;
    return;
  }
  
//  std::cout << currentPatternWidth << " " << currentPatternX
//      << " " << patternSubX_ << std::endl;
  
  // If we moved to a new pattern, we now need to seek through the
  // token data to find the next/previous pattern (depending on the
  // direction of movement)

  // Determine direction of seek for iterating through patterns
//  int direction = 1;
//  if (distance < 0) {
//    direction = -1;
//  }
  
  if (distance < 0) {
    // Seek left
    
    // The current pattern doesn't contribute to the total when seeking left
    --patternPos_;
    
    while ((patternPos_ >= 0)) {
      LaylaPatternToken& token = level_->areaData().patternToken(patternPos_);
      
      // Iterate backwards through standard patterns until we find the
      // one that the new camX corresponds to
      if (token.type() == LaylaPatternTypes::standardPattern) {
        StandardPatternToken& patternToken
          = dynamic_cast<StandardPatternToken&>(token);
        
        int patW = level_->patternDefinitions().pattern(
                            patternToken.patternNum()).realWidth();
//        currentPatternX -= patW;
        patternSubX_ += patW;
        
        if (patternSubX_ >= 0) {
          break;
        }
      }
      
      --patternPos_;
    }
    
//    std::cout << patternPos_ << std::endl;
  }
  else {
    // Seek right
    while ((patternPos_ < level_->areaData().numPatternTokens())) {
      LaylaPatternToken& token = level_->areaData().patternToken(patternPos_);
      
      // Iterate forwards through standard patterns until we find the
      // one that the new camX corresponds to
      if (token.type() == LaylaPatternTypes::standardPattern) {
        StandardPatternToken& patternToken
          = dynamic_cast<StandardPatternToken&>(token);
        
        int patW = level_->patternDefinitions().pattern(
                            patternToken.patternNum()).realWidth();
//        currentPatternX -= patW;
        
        if (patternSubX_ < patW) {
          break;
        }
        else {
          patternSubX_ -= patW;
        }
      }
      
      ++patternPos_;
    }
  }
    
  // If we hit the start of the data without finding a standard pattern,
  // seek to the start of the data
  if (patternPos_ < 0) {
    seekStartingPattern();
    camX_ = 0;
    return;
  }
  
  // If we hit the end of the data, ???

  // Seek succeeded: set new camX
  camX_ = camX__;
//  std::cout << camX_ << std::endl;
}
                        
void LonelyLevelView::resetToPattern(int index) {
  // Seek pattern from start of level
  camX_ = 0;
  patternSubX_ = 0;
  patternPos_ = 0;
  
  for (int i = 0; i < index; i++) {
    LaylaPatternToken& token = level_->areaData().patternToken(patternPos_);
    
    // Iterate forwards through standard patterns
    if (token.type() == LaylaPatternTypes::standardPattern) {
      StandardPatternToken& patternToken
        = dynamic_cast<StandardPatternToken&>(token);
      
      int patW = level_->patternDefinitions().pattern(
                          patternToken.patternNum()).realWidth();
      camX_ += patW;
    }
    
    ++patternPos_;
  }
}

void LonelyLevelView::seekCamY(int camY__) {
  if (camY_ == camY__) {
    return;
  }
  
  camY_ = camY__;
//  std::cout << camY_ << std::endl;
}

void LonelyLevelView::setCamW(int camW__) {
  camW_ = camW__;
}

void LonelyLevelView::setCamH(int camH__) {
  camH_ = camH__;
}

double LonelyLevelView::zoom() const {
  return zoom_;
}

void LonelyLevelView::setZoom(double zoom__) {
  zoom_ = zoom__;
}

int LonelyLevelView::camX() const {
  return camX_;
}

int LonelyLevelView::camY() const {
  return camY_;
}

int LonelyLevelView::camW() const {
  return camW_;
}

int LonelyLevelView::camH() const {
  return camH_;
}

int LonelyLevelView::realCamX() const {
  return camX_ * zoom_;
}

int LonelyLevelView::realCamY() const {
  return camY_ * zoom_;
}

int LonelyLevelView::realCamW() const {
  return camW_ / zoom_;
}

int LonelyLevelView::realCamH() const {
  return camH_ / zoom_;
}

bool LonelyLevelView::gridEnabled() const {
  return gridEnabled_;
}

void LonelyLevelView::setGridEnabled(bool gridEnabled__) {
  gridEnabled_ = gridEnabled__;
}

bool LonelyLevelView::patternMarkersEnabled() const {
  return patternMarkersEnabled_;
}

void LonelyLevelView::setPatternMarkersEnabled(bool patternMarkersEnabled__) {
  patternMarkersEnabled_ = patternMarkersEnabled__;
}

bool LonelyLevelView::loopsEnabled() const {
  return loopsEnabled_;
}

void LonelyLevelView::setLoopsEnabled(bool loopsEnabled__) {
  loopsEnabled_ = loopsEnabled__;
}

bool LonelyLevelView::spawnsEnabled() const {
  return spawnsEnabled_;
}

void LonelyLevelView::setSpawnsEnabled(bool spawnsEnabled__) {
  spawnsEnabled_ = spawnsEnabled__;
}

LonelyLevelView::ViewType LonelyLevelView::viewType() const {
  return viewType_;
}

void LonelyLevelView::setViewType(ViewType viewType__) {
  viewType_ = viewType__;
}
  
void LonelyLevelView::drawViews() {
  drawLevelView();
  drawControlView();
  
}
  
void LonelyLevelView::drawLevelView() {
  levelCache_.resize(camW_, camH_);
  levelCache_.clear(backgroundColor_);
  
  int areaType = findStartingDrawAreaType();
  
  // Spawn points have to be drawn after the pattern they're associated with,
  // but their tokens come first in the data. Thus we have to keep track of
  // when one is needed when we encounter the token, then draw it at the
  // next pattern.
  bool spawnPending_ = false;
  
  // Some stuff needs to be drawn on top of the grid, so we keep track
  // of where they go while evaluating the patterns and draw them
  // when we're ready.
  DrawXCollection patternMarkers;
  DrawXCollection spawnsX;
  AreaTypeCollection spawnsArea;
  AreaTypeCollection spawnsNum;
  DrawXCollection elevatorsUpNum;
  DrawXCollection elevatorsDownNum;
  DrawXCollection elevatorsX;
  DrawXCollection scrollingDisabledsX;
  DrawXCollection bossesX;
  DrawXCollection bossesID;
  AreaTypeCollection bossesArea;
  DrawXCollection objectsX;
  DrawXCollection objectsPatternNum;
  AreaTypeCollection objectsAreaType;
  
  // **MAIN PATTERN DRAWING LOOP**
  // Draw patterns until we run out or cover the full camera area.
  int drawX = -patternSubX_ * zoom_;
  for (int i = findPreviousStandardPatternIndexNext(patternPos_);
       i < level_->areaData().numPatternTokens();
       i++) {
       
    // Draw tool preview if needed
    if (levelManager.toolsActive_) {
      switch (levelManager.activeTool_) {
      // Pattern modify
      case LonelyLevelViewManager::patternModify:
        {
          if (i == levelManager.pio_targetPatternIndex_) {
            modifyBoxX_ = drawX;
                  
            // Remember settings for drawing object overlay later
            objectsX.push_back(drawX);
            objectsPatternNum.push_back(levelManager.pio_sourcePatternNum_);
            objectsAreaType.push_back(areaType);
            
            drawPatternAndAdvance(patternDefinition(
                                    levelManager.pio_sourcePatternNum_),
                                  drawX,
                                  areaType,
                                  patternMarkers);
            
            modifyBoxW_ = drawX - modifyBoxX_;
            modifyBoxY_ = (levelYOffset_ - camY_) * zoom_;
            modifyBoxH_ = levelNativeHeightReal_ * zoom_;
            
            // If in overwrite mode, skip drawing the pattern that
            // would be overwritten
            if (levelManager.pio_overwrite_) {
              continue;
            }
          }
        }
        break;
      // Pattern copy
      case LonelyLevelViewManager::patternCopy:
        {
        if ((levelManager.cpy_mode_
              == LonelyLevelViewManager::cpy_waitingForPasteClick)
            && (i == levelManager.cpy_patternBaseIndexReal_)) {
          
          // If we're drawing an overwrite preview, we need to preemptively
          // skip over the node data for the base pattern (which won't be
          // shown). Otherwise, the calls to findNextStandardPatternIndex()
          // below will be off by if there's node data at the base pattern.
          if (levelManager.pio_overwrite_) {
            i = levelManager.cpy_patternBaseIndex_;
          }
            
          int startX = drawX;
          for (int j = 0; j < levelManager.cpy_copyPatterns_.size(); j++) {
            if (levelManager.cpy_copyPatterns_[j]->type()
                  == LaylaPatternTypes::standardPattern) {
              StandardPatternToken& token
                = *(dynamic_cast<StandardPatternToken*>(
                    levelManager.cpy_copyPatterns_[j]));
                  
              // Remember settings for drawing object overlay later
              objectsX.push_back(drawX);
              objectsPatternNum.push_back(token.patternNum());
              objectsAreaType.push_back(areaType);
              
              drawPatternAndAdvance(patternDefinition(token.patternNum()),
                                    drawX,
                                    areaType,
                                    patternMarkers);
              
              // If overwriting, skip corresponding pattern in "real" data
              if (levelManager.pio_overwrite_) {
                // If we're not copying nodes, we have to keep track of
                // changes in the area type within the overwritten data
/*                if (levelManager.cpy_copyNodes_) {
                  i = findNextStandardPatternIndex(i);
                }
                else { */
                  i = findNextStandardPatternIndexAndUpdateAreaType(
                      i,  areaType);
//                }
              }
            }
/*            else if ((levelManager.cpy_copyPatterns_[j]->type()
                  == LaylaPatternTypes::setArea)
                    && (levelManager.cpy_copyNodes_)) {
              areaType = dynamic_cast<SetAreaToken*>(
                  levelManager.cpy_copyPatterns_[j])->areaType();
            } */
            else if (levelManager.cpy_copyPatterns_[j]->type()
                  == LaylaPatternTypes::spawnPoint) {
              if ((spawnsEnabled_)
                  && (levelManager.cpy_copyNodes_)
                  && (!(levelManager.pio_overwrite_))) {
                spawnsX.push_back(drawX);
                spawnsArea.push_back(areaType);
                spawnsNum.push_back(
                    dynamic_cast<SpawnPointToken*>(
                      levelManager.cpy_copyPatterns_[j])->spawnNum());
              }
            }
          }
          
          // Box paste area
/*          levelCache_.drawRectBorder(startX,
                                     (levelYOffset_ - camY_) * zoom_,
                                     (drawX - startX),
                                     levelNativeHeightReal_ * zoom_,
                                     Tcolor(0xFF, 0xFF, 0x00),
                                     2 * zoom_); */
          pasteBoxX_ = startX;
          pasteBoxY_ = (levelYOffset_ - camY_) * zoom_;
          pasteBoxW_ = (drawX - startX);
          pasteBoxH_ = levelNativeHeightReal_ * zoom_;
        }
        }
        break;
      // Pattern move
      case LonelyLevelViewManager::patternMove:
        {
          if ((levelManager.mov_mode_
                == LonelyLevelViewManager::mov_waitingForSelectRelease)
              || (levelManager.mov_mode_
                == LonelyLevelViewManager::mov_waitingForMoveClick)
              || (levelManager.mov_mode_
                == LonelyLevelViewManager::mov_waitingForMoveRelease)) {
            if (i == levelManager.mov_patternBaseIndexReal_) {
              pasteBoxX_ = drawX;
              pasteBoxY_ = (levelYOffset_ - camY_) * zoom_;
              pasteBoxH_ = levelNativeHeightReal_ * zoom_;
            }
            else if (i == levelManager.mov_patternEndIndex_) {
              pasteBoxW_ = (drawX - pasteBoxX_);
            }
          }
        }
        break;
      default:
        
        break;
      }
    }
    
    // The process of drawing an overwrite preview may have incremented
    // i past the end of the token data, so check if we need to terminate
    // here
    if (i >= level_->areaData().numPatternTokens()) {
      break;
    }
    
    // Draw each pattern
    switch (level_->areaData().patternToken(i).type()) {
    case LaylaPatternTypes::standardPattern:
    {
      StandardPatternToken& patternToken
        = dynamic_cast<StandardPatternToken&>(
            level_->areaData().patternToken(i));
      int patternNum = patternToken.patternNum();
      
      // Remember settings for drawing object overlay later
      objectsX.push_back(drawX);
      objectsPatternNum.push_back(patternNum);
      objectsAreaType.push_back(areaType);
      
      LaylaPattern& pattern = patternDefinition(patternNum);
      drawPatternAndAdvance(pattern,
                            drawX,
                            areaType,
                            patternMarkers);
    }
      break;
    // Draw spawns if enabled
    case LaylaPatternTypes::spawnPoint:
    {
//      if (spawnsEnabled_) {
//        spawnPending_ = true;
//      }
      if (spawnsEnabled_) {
        spawnsX.push_back(drawX);
        spawnsArea.push_back(areaType);
        spawnsNum.push_back(
          dynamic_cast<SpawnPointToken&>(level_->areaData().patternToken(i))
            .spawnNum());
//        std::cout << spawnsNum[spawnsNum.size() - 1] << std::endl;
      }
    }
      break;
    // Show bosses
    case LaylaPatternTypes::spawnBoss:
    {
      SpawnBossToken& spawnBossToken
        = dynamic_cast<SpawnBossToken&>(
            level_->areaData().patternToken(i));
      
      if (spawnsEnabled_) {
        bossesX.push_back(drawX);
        bossesID.push_back(spawnBossToken.bossNum());
        bossesArea.push_back(areaType);
      }
    }
      break;
    // Draw scrolling-disabled bar
    case LaylaPatternTypes::setScrolling:
    {
      SetScrollingToken& setScrollingToken
        = dynamic_cast<SetScrollingToken&>(
            level_->areaData().patternToken(i));
      if (patternMarkersEnabled_
          && setScrollingToken.scrollingIsDisabled()) {
        scrollingDisabledsX.push_back(drawX);
      }
    }
      break;
    // Switch area type when set area tokens are encountered
    case LaylaPatternTypes::setArea:
    {
      SetAreaToken& setAreaToken
        = dynamic_cast<SetAreaToken&>(
            level_->areaData().patternToken(i));
      areaType = setAreaToken.areaType();
    }
      break;
    // Display elevator destinations
    case LaylaPatternTypes::setElevators:
    {
      SetElevatorsToken& setElevatorsToken
        = dynamic_cast<SetElevatorsToken&>(
            level_->areaData().patternToken(i));
      
      // Draw spawn num
      if (spawnsEnabled_) {
        elevatorsUpNum.push_back(setElevatorsToken.upDestination());
        elevatorsDownNum.push_back(setElevatorsToken.downDestination());
        elevatorsX.push_back(drawX);
      }
    }
      break;
    case LaylaPatternTypes::jumpToPosition:
    {
      if (loopsEnabled_) {
        JumpToPositionToken& jumpToPositionToken
          = dynamic_cast<JumpToPositionToken&>(
              level_->areaData().patternToken(i));
        
        int target = jumpToPositionToken.offset();
        
        int length = findJumpLength(i, target) * zoom_;
        
        int yPos = (length < 0)
                              ? (leftJumpLineYOffset_ * zoom_)
                              : (rightJumpLineYOffset_ * zoom_);
        yPos -= realCamY();
        int dstX = drawX + length;
        Tcolor color = (length < 0)
              ? leftJumpColor_
              : rightJumpColor_;
        
        // Draw body lines
        int lineWidth = 1 * zoom_;
        int baselineY = (topDisplayHeight_ - 2) * zoom_ - realCamY();
        levelCache_.drawLine(drawX, baselineY,
                             drawX, yPos,
                             color,
                             lineWidth,
                             Graphic::noTransUpdate);
        levelCache_.drawLine(drawX, yPos,
                             dstX, yPos,
                             color,
                             lineWidth,
                             Graphic::noTransUpdate);
        levelCache_.drawLine(dstX, baselineY,
                             dstX, yPos,
                             color,
                             lineWidth,
                             Graphic::noTransUpdate);
        
        // Draw arrowhead
        int wdth = 4 * zoom_;
        int len = 5 * zoom_;
        int correctionX = 0 * zoom_;
        int correctionY = 0 * zoom_;
        levelCache_.drawLine(dstX - correctionX, baselineY,
                             dstX - wdth - correctionX, baselineY - len,
                             color, lineWidth,
                             Graphic::noTransUpdate);
        levelCache_.drawLine(dstX + correctionX,
                               baselineY - correctionY,
                             dstX + wdth + correctionX,
                               baselineY - len - correctionY,
                             color, lineWidth,
                             Graphic::noTransUpdate);
      }
    }
      break;
    default:
      
      break;
    }
    
    if (drawX >= camW_ + rightOverdrawW_) {
      break;
    }
  }
  // **END MAIN PATTERN DRAWING LOOP**
  
  // Draw grid
  if (gridEnabled_) {
    grid_.render(levelCache_,
                 Box(camX_, camY_, camW_, camH_),
                 zoom_);
  }
  
  // Draw stuff that goes on top of the grid
  
  // Pattern markers
  for (int i = 0; i < patternMarkers.size(); i++) {
    levelCache_.drawLine(patternMarkers[i], (levelYOffset_ - camY_) * zoom_,
                         patternMarkers[i], camH_,
                         Tcolor(0xFF, 0x00, 0x00),
                         1 * zoom_,
                         Graphic::noTransUpdate);
  }
  
  // Spawn points
  LonelyLaylaMapping laylaMapping
    = LonelyLaylaMappings::lonelyLaylaMapping();
  for (int i = 0; i < spawnsX.size(); i++) {
    Graphic g;
    
    // Trying to pull out the right set of graphics/palettes doesn't
    // really work out with the left-to-right evaluation strategy used
    // in the drawing loop. In the stock game, the SetArea token usually
    // comes after the spawn point rather than before, and so we get
    // end up running "behind" the area changes. Using the cave resources
    // every time should be sufficient for editing purposes.
    NesPatternTable& patternTable = spritesForArea(SetAreaToken::caveID);
    NesPatternTable& bgPatternTable = backgroundForArea(SetAreaToken::caveID);
    NesPaletteQuad palettes = spritePalettesForArea(SetAreaToken::caveID);
    NesPaletteQuad bgPalettes
      = backgroundPalettesForArea(SetAreaToken::caveID);
    
    laylaMapping.draw(g,
                      patternTable,
                      palettes);
    
    levelCache_.scaleAndBlit(
                     g,
                     Box(spawnsX[i] + (spawnXOffset_ * zoom_),
                         (spawnYOffset_ - camY_) * zoom_,
                         g.w() * zoom_,
                         g.h() * zoom_),
                     Graphic::noTransUpdate);
    
    // Draw spawn num
    Graphic n;
    LonelyLaylaMapping numberMapping
      = LonelyLaylaMappings::lonelyNumberMapping(spawnsNum[i], 3);
    
    numberMapping.draw(n,
                       bgPatternTable,
                       bgPalettes,
                       false);
    
    levelCache_.scaleAndBlit(
                     n,
                     Box(spawnsX[i] + (spawnNumXOffset_ * zoom_),
                         (spawnNumYOffset_ - camY_) * zoom_,
                         n.w() * zoom_,
                         n.h() * zoom_),
                     Graphic::noTransUpdate);
  }
  
  // Bosses
  if (spawnsEnabled_) {
    for (int i = 0; i < bossesX.size(); i++) {
      int drawX = bossesX[i];
      Tbyte bossNum = bossesID[i];
      NesPatternTable& patternTable = spritesForArea(bossesArea[i]);
      NesPaletteQuad palettes = spritePalettesForArea(bossesArea[i]);
      
      level_->objectPatterns().drawBossOverlay(
                                    levelCache_,
                                    drawX,
                                    (bottomDisplayOffset_ - camY_) * zoom_,
                                    zoom_,
                                    bossNum,
                                    level_->objectSetNum(),
                                    patternTable,
                                    palettes);
    }
  }
  
  // Objects
  if (spawnsEnabled_) {
    for (int i = 0; i < objectsX.size(); i++) {
      int drawX = objectsX[i];
      int patternNum = objectsPatternNum[i];
      int areaType = objectsAreaType[i];
      
      NesPatternTable& patternTable = spritesForArea(areaType);
  //    NesPatternTable& bgPatternTable = backgroundForArea(areaType);
      NesPaletteQuad palettes = spritePalettesForArea(areaType);
  //    NesPaletteQuad bgPalettes
  //      = backgroundPalettesForArea(areaType);
      
      level_->objectPatterns().drawPatternObjectOverlay(
                                    levelCache_,
                                    drawX,
                                    (bottomDisplayOffset_ - camY_) * zoom_,
                                    zoom_,
                                    patternNum,
                                    patternDefinition(patternNum).numColumns(),
                                    level_->objectSetNum(),
                                    patternTable,
                                    palettes);
    }
  }
  
  // Scrolling disabled markers
  for (int i = 0; i < scrollingDisabledsX.size(); i++) {
    levelCache_.drawLine(scrollingDisabledsX[i]
                          + (scrollingDisabledXOffset_ * zoom_),
                         (levelYOffset_ - camY_) * zoom_,
                         scrollingDisabledsX[i]
                          + (scrollingDisabledXOffset_ * zoom_),
                         (levelYOffset_ + levelNativeHeightReal_
                            - camY_)
                          * zoom_,
                         Tcolor(0x00, 0xFF, 0xFF),
                         2 * zoom_,
                         Graphic::noTransUpdate);
  }
  
  // Elevator markers
  for (int i = 0; i < elevatorsX.size(); i++) {
    Graphic upG;
    Graphic downG;
    LonelyLaylaMapping upMapping
      = LonelyLaylaMappings::lonelyNumberMapping(
          elevatorsUpNum[i], 3);
    LonelyLaylaMapping downMapping
      = LonelyLaylaMappings::lonelyNumberMapping(
          elevatorsDownNum[i], 3);
    
    upMapping.draw(upG, graphics_->caveBackground(),
                     palettes_->generateCavePalette(
                      levelNum_).backgroundPalettes(),
                     false);
    downMapping.draw(downG, graphics_->caveBackground(),
                     palettes_->generateCavePalette(
                      levelNum_).backgroundPalettes(),
                     false);
    
    levelCache_.scaleAndBlit(
                     upG,
                     Box(elevatorsX[i] + (elevatorNumXOffset_ * zoom_),
                         (elevatorUpNumYOffset_ - camY_) * zoom_,
                         upG.w() * zoom_,
                         upG.h() * zoom_),
                     Graphic::noTransUpdate);
    levelCache_.scaleAndBlit(
                     downG,
                     Box(elevatorsX[i] + (elevatorNumXOffset_ * zoom_),
                         (elevatorDownNumYOffset_ - camY_) * zoom_,
                         downG.w() * zoom_,
                         downG.h() * zoom_),
                     Graphic::noTransUpdate);
  }
  
  // Pattern modify selection outline
  if (levelManager.toolsActive_
      && (levelManager.activeTool_
            == LonelyLevelViewManager::patternModify)) {
    Tcolor boxColor = Tcolor(0x00, 0xFF, 0x00);
    if (!(levelManager.pio_overwrite_)) {
      boxColor = Tcolor(0x00, 0x00, 0xFF);
    }
    
    levelCache_.drawRectBorder(modifyBoxX_,
                               modifyBoxY_,
                               modifyBoxW_,
                               modifyBoxH_,
                               boxColor,
                               2 * zoom_);
  }
  
  // Copy tool selection outline
  if (((levelManager.toolsActive_)
            && (levelManager.activeTool_
               == LonelyLevelViewManager::patternCopy)
            && (levelManager.cpy_mode_ == LonelyLevelViewManager
                                    ::cpy_waitingForSelectRelease))
      || ((levelManager.activeTool_
            == LonelyLevelViewManager::patternMove)
            && ((levelManager.mov_mode_ == LonelyLevelViewManager
                                    ::mov_waitingForSelectRelease)
                  || (levelManager.mov_mode_ == LonelyLevelViewManager
                                     ::mov_waitingForMoveClick)
                  || (levelManager.mov_mode_ == LonelyLevelViewManager
                                     ::mov_waitingForMoveRelease)))) {
    int patternBaseIndex = levelManager.cpy_patternBaseIndex_;
    int patternEndIndex = levelManager.cpy_patternEndIndex_;
    Tcolor color = Tcolor(0xFF, 0xFF, 0x00);
    if ((levelManager.activeTool_
            == LonelyLevelViewManager::patternMove)) {
      patternBaseIndex = levelManager.mov_patternBaseIndex_;
      patternEndIndex = levelManager.mov_patternEndIndex_;
      color = Tcolor(0x44, 0xCC, 0xFF);
    }
//    std::cout << patternBaseIndex << " " << patternEndIndex << std::endl;
    
    if (patternBaseIndex > patternEndIndex) {
      std::swap(patternBaseIndex, patternEndIndex);
//      patternBaseIndex = level_->areaData()
//        .findPreviousStandardPatternIndex(patternBaseIndex);
    }
    
    int leftX = patternDrawX(patternBaseIndex);
    int rightX = patternDrawX(patternEndIndex);
    
    int endPatternW =
      patternDefinitionAtStandardPos(patternEndIndex)
                  .realWidth() * zoom_;
//    int endPatternW =
//      patternDefinitionAtStandardPos(
//        level_->areaData().findPreviousStandardPatternIndex(patternEndIndex))
//                  .realWidth() * zoom_;
    // Add the width of the final pattern if selecting rightward
//    if (patternEndIndex >= patternBaseIndex) {
      rightX += endPatternW;
//    }
    // Shift one pattern right if copying left
//    else {
//      leftX += endPatternW;
//    }
    levelCache_.drawRectBorder(leftX,
                               (levelYOffset_ - camY_) * zoom_,
                               rightX - leftX,
                               levelNativeHeightReal_ * zoom_,
                               color,
                               2 * zoom_,
                               Graphic::noTransUpdate);
  }
  else if (((levelManager.toolsActive_)
            && (levelManager.activeTool_
            == LonelyLevelViewManager::patternCopy)
              && (levelManager.cpy_mode_ == LonelyLevelViewManager
                                      ::cpy_waitingForPasteClick))) {
    levelCache_.drawRectBorder(pasteBoxX_,
                               pasteBoxY_,
                               pasteBoxW_,
                               pasteBoxH_,
                               Tcolor(0xFF, 0xFF, 0x00),
                               2 * zoom_);
  }
}
  
NesPatternTable& LonelyLevelView::backgroundForArea(int areaType) {
  if (areaType & SetAreaToken::bossFlag) {
    return graphics_->bossBackground();
  }
  else if (areaType & SetAreaToken::baseFlag) {
    return graphics_->baseBackground();
  }
  else {
    return graphics_->caveBackground();
  }
}

NesPatternTable& LonelyLevelView::spritesForArea(int areaType) {
  if (areaType & SetAreaToken::bossFlag) {
    return graphics_->bossSprites();
  }
  else if (areaType & SetAreaToken::baseFlag) {
    return graphics_->baseSprites();
  }
  else {
    return graphics_->caveSprites();
  }
}

NesPaletteQuad LonelyLevelView::backgroundPalettesForArea(int areaType) {
  if (areaType & SetAreaToken::bossFlag) {
    return palettes_->generateBossPalette(levelNum_).backgroundPalettes();
  }
  else if (areaType & SetAreaToken::baseFlag) {
    return palettes_->generateBasePalette(levelNum_).backgroundPalettes();
  }
  else {
    return palettes_->generateCavePalette(levelNum_).backgroundPalettes();
  }
}

NesPaletteQuad LonelyLevelView::spritePalettesForArea(int areaType) {
  if (areaType & SetAreaToken::bossFlag) {
    return palettes_->generateBossPalette(levelNum_).spritePalettes();
  }
  else if (areaType & SetAreaToken::baseFlag) {
    return palettes_->generateBasePalette(levelNum_).spritePalettes();
  }
  else {
    return palettes_->generateCavePalette(levelNum_).spritePalettes();
  }
}

Graphic& LonelyLevelView::levelView() {
  return levelCache_;
}

void LonelyLevelView::drawControlView() {
  controlCache_.resize(camW_, controlViewHeight_);
  controlCache_.clear(backgroundColor_);
  
  // Draw patterns until we run out or cover the full camera area
  int drawX = -patternSubX_ * zoom_;
  int controlItemCount = 0;
  
  for (int i = findPreviousStandardPatternIndexNext(patternPos_);
       i < level_->areaData().numPatternTokens();
       i++) {
    switch (level_->areaData().patternToken(i).type()) {
    case LaylaPatternTypes::standardPattern:
    {
      StandardPatternToken& patternToken
        = dynamic_cast<StandardPatternToken&>(
            level_->areaData().patternToken(i));
      
      LaylaPattern& pattern =  level_->patternDefinitions().pattern(
                                  patternToken.patternNum());
        
      int patW = pattern.realWidth();
      
      if ((controlManager.toolMode_ == LonelyControlViewManager
                                          ::waitingForNodeRelease)
          && (controlManager.draggedNodePatternIndex_ == i)) {
        // Draw drag-target node as filled if either the source or the
        // destination has at least one node item
        int realNumEntries =
            (level_->areaData().nodeEntriesAtIndex(
                    i))
            + (level_->areaData().nodeEntriesAtIndex(
                    controlManager.selectedNodePatternIndex_));
                    
        drawNode(controlCache_,
                 drawX,
                 (realNumEntries > 0),
                 true);
      }
      else {
        bool filled = (controlItemCount > 0);
        bool selected = ((controlManager.nodeSelected_)
                      && (controlManager.selectedNodePatternIndex_ == i));
        
        // Don't draw the source node of a move drag as filled or selected
        // unless it's at its original position
        if ((controlManager.toolMode_ == LonelyControlViewManager
                                            ::waitingForNodeRelease)
            && (!(controlManager.cloneClick_))
            && (controlManager.selectedNodePatternIndex_ == i)
            && (controlManager.selectedNodePatternIndex_
                  != controlManager.draggedNodePatternIndex_)) {
          filled = false;
          selected = false;
        }
        
        drawNode(controlCache_,
                 drawX,
                 filled,
                 selected);
      }
      
      // Draw pattern marker if enabled
/*      if (patternMarkersEnabled_) {
        controlCache_.drawLine(drawX,
                               0,
                               drawX,
                               controlViewHeight_,
                               Tcolor(0xFF, 0x00, 0x00),
                               3,
                               Graphic::noTransUpdate); 
      } */
      
      controlItemCount = 0;
      drawX += (patW * zoom_);
    }
      break;
    case LaylaPatternTypes::spawnPoint:
    case LaylaPatternTypes::spawnBoss:
    case LaylaPatternTypes::setScrolling:
    case LaylaPatternTypes::setArea:
    case LaylaPatternTypes::setMap:
    case LaylaPatternTypes::setElevators:
    case LaylaPatternTypes::jumpToPosition:
      ++controlItemCount;
      break;
    default:
      
      break;
    }
    
    // Stop drawing if we go off the right side of the camera
    if (drawX >= camW_) {
      break;
    }
  }
}
  
void LonelyLevelView::drawNode(Graphic& dst,
              int drawX,
              bool filled,
              bool selected) {
  // Draw node outline
  dst.fillRect((zoom_ < 1.00)
                          ? (drawX + (controlViewNodeXOffset_ * zoom_))
                          : (drawX + controlViewNodeXOffset_),
                         controlViewNodeYOffset_,
                         (zoom_ < 1.00)
                          ? (controlViewNodeW_ * zoom_)
                          : (controlViewNodeW_),
                         controlViewNodeH_,
                         controlNodeBorderColor_,
//                             controlViewNodeLineWidth_,
                         Graphic::noTransUpdate);
  
  // If no items exist, draw background color ("empty" node)
  Tcolor fillColor = backgroundColor_;
  if (filled) {
    // If control items exists, draw filled node
    fillColor = controlNodeFillColor_;
  }
  
  controlCache_.fillRect((zoom_ < 1.00)
                          ? (drawX + (controlViewNodeFillXOffset_ * zoom_))
                          : (drawX + controlViewNodeFillXOffset_),
                         controlViewNodeFillYOffset_,
                         (zoom_ < 1.00)
                           ? (controlViewNodeFillW_ * zoom_)
                           : controlViewNodeFillW_,
                         controlViewNodeFillH_,
                         fillColor,
                         Graphic::noTransUpdate);
  
  // If node is selected, draw selection box
  if (selected) {
    controlCache_.drawRectBorder((zoom_ < 1.00)
                            ? (drawX + (controlViewNodeXOffset_
                                * zoom_))
                            : (drawX + controlViewNodeXOffset_),
                           controlViewNodeYOffset_,
                           (zoom_ < 1.00)
                             ? (controlViewNodeW_ * zoom_)
                             : controlViewNodeW_,
                           controlViewNodeH_,
                           Tcolor(0xFF, 0xFF, 0x00),
                           3,
                           Graphic::noTransUpdate);
  }
}

Graphic& LonelyLevelView::controlView() {
  return controlCache_;
}
  
int LonelyLevelView::levelNativeHeightReal() {
  return levelNativeHeightReal_;
}
  
void LonelyLevelView::cacheMetatiles() {
  metatileCaches_.cacheMetatiles(
    levelNum_,
    level_->baseMetatileSet(),
    *staticMetatiles_,
    *graphics_,
    *palettes_);
}
  
LaylaPattern& LonelyLevelView::currentPattern() {
  return level_->patternDefinitions().pattern(
            dynamic_cast<StandardPatternToken&>(
                level_->areaData().patternToken(patternPos_))
              .patternNum());
}
  
LaylaPattern& LonelyLevelView::patternDefinition(int patternID) {
  return level_->patternDefinitions().pattern(patternID);
}
  
void LonelyLevelView::seekStartingPattern() {
  camX_ = 0;
  patternSubX_ = 0;
  patternPos_ = 0;
  for (int i = 0; i < level_->areaData().numPatternTokens(); i++) {
    if (level_->areaData().patternToken(i).type()
          == LaylaPatternTypes::standardPattern) {
      patternPos_ = i;
      break;
    }
  }
}
  
int LonelyLevelView::findStartingDrawAreaType() {
  for (int i = patternPos_; i >= 0; i--) {
    if (level_->areaData().patternToken(i).type()
          == LaylaPatternTypes::setArea) {
      SetAreaToken& setAreaToken
        = dynamic_cast<SetAreaToken&>(
            level_->areaData().patternToken(i));
      
      return setAreaToken.areaType();
    }
  }
  
  // If we can't find a token, assume cave
  return SetAreaToken::caveID;
}
  
int LonelyLevelView::findJumpLength(int startIndex, int target) {
  // Search forwards/backwards through tokens to find the
  // target of the jump
  int direction = 1;
  int pos = startIndex;
  if (target > startIndex) {
    direction = 1;
    ++pos;  // doesn't contribute if jumping forward
  }
  else if (target < startIndex) {
    direction = -1;
  }
  else {
    // self-jump
    return 0;
  }
  
//  std::cout << direction << std::endl;
  
  int total = 0;
  while (true) {
    LaylaPatternToken& token = level_->areaData().patternToken(pos);
    switch (token.type()) {
    case LaylaPatternTypes::standardPattern:
    {
      StandardPatternToken& patternToken
        = dynamic_cast<StandardPatternToken&>(token);
      total += level_->patternDefinitions().pattern(
                 patternToken.patternNum()).realWidth();
    }
      break;
    default:
      break;
    }
    
    pos += direction;
    
    // If jumping right, stop when we reach the target; if jumping
    // left, stop after evaluating the target
    if ((direction > 0) && (pos >= target)) {
      break;
    }
    else if ((direction < 0) && (pos < target)) {
      break;
    }
  }
  
  if (direction > 0) {
    return total;
  }
  else {
    return -total;
  }
}
  
int LonelyLevelView::findPreviousStandardPatternIndexNext(int startIndex) {
  return level_->areaData().findPreviousStandardPatternIndexNext(startIndex);
}
  
int LonelyLevelView::findPreviousStandardPatternIndex(int startIndex) {
  return level_->areaData().findPreviousStandardPatternIndex(startIndex);
}
  
int LonelyLevelView::findNextStandardPatternIndex(int startIndex) {
  return level_->areaData().findNextStandardPatternIndex(startIndex);
}
  
int LonelyLevelView
    ::findNextStandardPatternIndexAndUpdateAreaType(int startIndex,
                                              int& areaType) {
  int nextIndex
      = level_->areaData().findNextStandardPatternIndex(startIndex);
  // Look for set area tokens
  for (int i = startIndex; i < nextIndex; i++) {
    if (level_->areaData().patternToken(i).type()
          == LaylaPatternTypes::setArea) {
      SetAreaToken& token = dynamic_cast<SetAreaToken&>(
          level_->areaData().patternToken(i));
      areaType = token.areaType();
    }
  }
  
  return nextIndex;
}
  
int LonelyLevelView::findTargetPatternIndex(int targetX, int targetY) {
  int distance = targetX - camX_;
  
  if ((distance >= 0)
      && (distance < currentPattern().realWidth()
            - patternSubX_)) {
    return patternPos_;
  }
  
  distance -= currentPattern().realWidth() - patternSubX_;
  
  int target = patternPos_;
//  int actualX = camX_ - patternSubX_;
  
  if (distance > 0) {
    bool found = false;
    while (target < level_->areaData().numPatternTokens() - 1) {
      target = findNextStandardPatternIndex(target);
      
      if (target >= level_->areaData().numPatternTokens()) {
        break;
      }
      
      LaylaPatternToken& token = level_->areaData().patternToken(target);
      switch (token.type()) {
      case LaylaPatternTypes::standardPattern:
      {
        StandardPatternToken& patternToken
          = dynamic_cast<StandardPatternToken&>(token);
        distance -= patternDefinition(patternToken.patternNum())
                        .realWidth();
        if (distance < 0) {
          found = true;
          break;
        }
      }
        break;
      default:
        break;
      }
      
      if (found) {
        break;
      }
    }
    
    if (!found) {
      target = findPreviousStandardPatternIndex(
                level_->areaData().numPatternTokens());
    }
  }
  
  return target;
}
  
void LonelyLevelView::drawPatternAndAdvance(LaylaPattern& pattern,
                           int& drawX,
                           int areaType,
                           DrawXCollection& patternMarkers)  {
    
  int patW = pattern.realWidth();
  
  LonelyMetatileCache* targetCache = NULL;
  if (areaType & SetAreaToken::bossFlag) {
    targetCache = &(metatileCaches_.bossCache());
  }
  else if (areaType & SetAreaToken::baseFlag) {
    targetCache = &(metatileCaches_.baseCache());
  }
  else {
    targetCache = &(metatileCaches_.caveCache());
  }
  
  MetatileCacheCollection* targetMetatileCache = NULL;
  targetMetatileCache = &(targetCache->metatilesVisual());
  switch (viewType_) {
  case visual:
    targetMetatileCache = &(targetCache->metatilesVisual());
    break;
  case behavior:
    targetMetatileCache = &(targetCache->metatilesBehavior());
    break;
  case solidity:
    targetMetatileCache = &(targetCache->metatilesSolidity());
    break;
  default:
    
    break;
  }
  
  if (zoom_ == 1.00) {
    pattern.draw(levelCache_,
                 drawX, -(camY_) + levelYOffset_,
                 *targetMetatileCache,
                 false);
  }
  else {
    Graphic nativeG(patW,
                    pattern.realHeight());
    
    pattern.draw(nativeG,
                 0, 0,
                 *targetMetatileCache,
                 false);
    
    levelCache_.scaleAndCopy(
                     nativeG,
                     Box(drawX,
                         (levelYOffset_ - camY_) * zoom_,
                         patW * zoom_,
                         pattern.realHeight() * zoom_),
                     Graphic::noTransUpdate);
    
    // this would be a better option if scale() was implemented
    // halfway decently, but it isn't
//        levelCache_.scale(nativeG,
//                          Box(drawX, -(camY_) * zoom_,
//                              patW * zoom_, camH_ * zoom_),
//                          Graphic::noTransUpdate);
  }
  
  if (patternMarkersEnabled_) {
    patternMarkers.push_back(drawX);
  }
  
  drawX += (patW * zoom_);
}
                             
int LonelyLevelView::patternDrawX(int index) {
  int total = -patternSubX_ * zoom_;
  
  if (index == patternPos_) {
    return total;
  }
  
  int pos = patternPos_;
  if (index > patternPos_) {
    while ((level_->areaData().patternToken(pos).type()
              == LaylaPatternTypes::standardPattern)
           && (pos < index)) {
      total += (patternDefinitionAtStandardPos(pos).realWidth() * zoom_);
      pos = findNextStandardPatternIndex(pos);
    }
  }
  else if (index < patternPos_) {
    while ((level_->areaData().patternToken(pos).type()
              == LaylaPatternTypes::standardPattern)
           && (pos >= index)) {
      pos = findPreviousStandardPatternIndex(pos);
      total -= (patternDefinitionAtStandardPos(pos).realWidth() * zoom_);
    }
  }
  
  return total;
}
  
LaylaPattern& LonelyLevelView::patternDefinitionAtStandardPos(int index) {
  StandardPatternToken& patternToken
    = dynamic_cast<StandardPatternToken&>(
        level_->areaData().patternToken(index));
  return patternDefinition(patternToken.patternNum());
}
  
void LonelyLevelView
		::overwritePatternsAt(int index,
                 LaylaPatternTokenVector& patterns) {
  level_->areaData().overwritePatternsAt(index, patterns);
}
  
void LonelyLevelView
		::overwritePatternsAndNodesAt(int index,
                 LaylaPatternTokenVector& patterns) {
  level_->areaData().overwritePatternsAndNodesAt(index, patterns);
}
  
void LonelyLevelView
		::insertPatternsAt(int index,
                 LaylaPatternTokenVector& patterns) {
  level_->areaData().insertPatternsAt(index, patterns);
}

void LonelyLevelView
		::insertPatternsAndNodesAt(int index,
                 LaylaPatternTokenVector& patterns) {
  level_->areaData().insertPatternsAndNodesAt(index, patterns);
}
  
void LonelyLevelView::deletePatternsAndNodesAt(int index,
                      int numPatterns) {
  level_->areaData().deletePatternsAndNodesAt(index, numPatterns);
}
  
void LonelyLevelView::removeTokenFromSelectedNode(int index) {
  if (!(controlManager.nodeSelected_)) {
    return;
  }
  
  // Ensure node is not empty
  if (level_->areaData().patternToken(index).type()
        == LaylaPatternTypes::standardPattern) {
    return;
  }

  level_->areaData().deletePattern(index);
  
  // If it so happens that the deleted pattern is part of the base display
  // pattern, update the base display index
  if (controlManager.selectedNodePatternIndex_ == patternPos_) {
    --patternPos_;
  }
  
  // Correct the selected node pattern index to account for the deleted
  // pattern
  --(controlManager.selectedNodePatternIndex_);
  
  // Reselect the node to ensure the displayed list updates
  controlManager.selectNode(controlManager.selectedNodePatternIndex_);
  
  // Refresh spawn list
  if (spawnsChangedCallback_ != NULL) {
    spawnsChangedCallback_(spawnsChangedCallbackObj_);
  }
}
  
void LonelyLevelView::appendTokenToSelectedNode(LaylaPatternToken* token) {
  if (!(controlManager.nodeSelected_)) {
    return;
  }
  
  LaylaPatternTokenVector patterns;
  patterns.push_back(token);
  
  level_->areaData().insertPatternsAndNodesAt(
    controlManager.selectedNodePatternIndex_,
    patterns);
  
  // If it so happens that the inserted pattern is part of the base display
  // pattern, update the base display index
  if (controlManager.selectedNodePatternIndex_ == patternPos_) {
    ++patternPos_;
  }
  
  // Correct the selected node pattern index to account for the inserted
  // pattern
  ++(controlManager.selectedNodePatternIndex_);
  
  // Reselect the node to ensure the displayed list updates
  controlManager.selectNode(controlManager.selectedNodePatternIndex_);
}
  
void LonelyLevelView::setControlCallbacks(void* callbackObj__,
                  void (*nodeSelectedCallback__)
                    (void*,TokenIndexCollection&),
                  void (*nodeDeselectedCallback__)(void*),
                  void (*spawnsChangedCallback__)(void*)) {
  controlManager.setCallbacks(callbackObj__,
                              nodeSelectedCallback__,
                              nodeDeselectedCallback__);
  spawnsChangedCallbackObj_ = callbackObj__;
  spawnsChangedCallback_ = spawnsChangedCallback__;
}
  
TokenIndexCollection LonelyLevelView::listOfSpawns() const {
  return level_->areaData().listOfSpawns();
}
  
void LonelyLevelView::seekSpawn(int spawnIndex) {
  int nodeIndex = level_->areaData().spawnPos(spawnIndex);
  nodeIndex = level_->areaData().findNextStandardPatternIndex(nodeIndex);
  
  resetToPattern(nodeIndex);
  
  controlManager.selectNode(nodeIndex);
}
  
void LonelyLevelView::addSpawnAtSelectedNode() {
  if (!(controlManager.nodeSelected_)) {
    return;
  }

  int finalIndex
     = level_->areaData().addSpawn(controlManager.selectedNodePatternIndex_);
  
  controlManager.deselectNode();
  controlManager.selectNode(finalIndex);
  
  if (spawnsChangedCallback_ != NULL) {
    spawnsChangedCallback_(spawnsChangedCallbackObj_);
  }
}
  
void LonelyLevelView::deleteSelectedNode() {
  if (!(controlManager.nodeSelected_)) {
    return;
  }
  
  deleteNode(controlManager.selectedNodePatternIndex_);
  
  controlManager.deselectNode();
  
  if (spawnsChangedCallback_ != NULL) {
    spawnsChangedCallback_(spawnsChangedCallbackObj_);
  }
}
  
void LonelyLevelView::deleteNode(int index) {
  int result = level_->areaData().deleteNode(index);
  
  // If the deleted node was for the base pattern, update
  // the base pattern position to the correct index
  if (index == patternPos_) {
    patternPos_ = result;
  }
}
  
bool LonelyLevelView::nodeSelected() const {
  return controlManager.nodeSelected_;
}
  
int LonelyLevelView::selectedNodeIndex() const {
  return controlManager.selectedNodePatternIndex_;
}
  
LaylaPatternToken& LonelyLevelView::patternToken(int index) {
  return level_->areaData().patternToken(index);
}
  
void LonelyLevelView::setSourcePattern(Tbyte patternNum) {
  levelManager.pio_sourcePatternNum_ = patternNum;
}

bool LonelyLevelView::hasSelectedPattern() const {
  return levelManager.activeTool_ == LonelyLevelViewManager::patternModify;
}

Tbyte LonelyLevelView::selectedPatternNum() const {
  return levelManager.pio_sourcePatternNum_;
}
  
bool LonelyLevelView::overwriteEnabled() const {
  return levelManager.pio_overwrite_;
}

void LonelyLevelView::setOverwriteEnabled(bool overwriteEnabled__) {
  levelManager.pio_overwrite_ = overwriteEnabled__;
}
  
bool LonelyLevelView::copyNodesEnabled() const {
  return levelManager.cpy_copyNodes_;
}

void LonelyLevelView::setCopyNodesEnabled(bool copyNodesEnabled__) {
  levelManager.cpy_copyNodes_ = copyNodesEnabled__;
}
  
LonelyLevelViewManager::Tool LonelyLevelView::currentTool() const {
  return levelManager.activeTool_;
}
  
void LonelyLevelView::changeTool(LonelyLevelViewManager::Tool tool__) {
  levelManager.changeTool(tool__);
}
  
bool LonelyLevelView::toolOverwriteAvailable() const {
  if ((levelManager.activeTool_ == LonelyLevelViewManager::patternModify)
      || (levelManager.activeTool_ == LonelyLevelViewManager::patternCopy)) {
    return true;
  }
  
  return false;
}

bool LonelyLevelView::toolCopyNodesAvailable() const {
  if ((levelManager.activeTool_ == LonelyLevelViewManager::patternCopy)
      && (!(levelManager.pio_overwrite_))) {
    return true;
  }
  
  return false;
}

bool LonelyLevelView::moveNodeItemUp(int index) {
  bool result = level_->areaData().moveNodeItemLeft(index);
  
  if (spawnsChangedCallback_ != NULL) {
    spawnsChangedCallback_(spawnsChangedCallbackObj_);
  }
  
  return result;
}

bool LonelyLevelView::moveNodeItemDown(int index) {
  bool result = level_->areaData().moveNodeItemRight(index);
  
  if (spawnsChangedCallback_ != NULL) {
    spawnsChangedCallback_(spawnsChangedCallbackObj_);
  }
  
  return result;
}
  
void LonelyLevelView::exportLevelMap(const std::string& filename) {
  int oldCamX = camX_;
  int oldCamY = camY_;
  int oldCamW = camW_;
  int oldCamH = camH_;
  int oldZoom = zoom_;
  
/*  camX_ = 0;
  camY_ = 0;
  camW_ = levelNativeWidth();
  camH_ = levelNativeHeight();
  zoom_ = 1.00; */
  seekCamX(0);
  seekCamY(0);
  camW_ = levelNativeWidth();
  camH_ = levelNativeHeight();
  zoom_ = 1.00;
  
  drawLevelView();
  PngConversion::graphicToRGBAPng(filename,
                      levelView());
  
//  camX_ = oldCamX;
//  camY_ = oldCamY;
  seekCamX(oldCamX);
  seekCamY(oldCamY);
  camW_ = oldCamW;
  camH_ = oldCamH;
  zoom_ = oldZoom;
  
  drawLevelView();
}


};
