#ifndef LONELYLEVELVIEW_H
#define LONELYLEVELVIEW_H


#include "structs/Tbyte.h"
#include "structs/Graphic.h"
#include "structs/GridGraphicSceneLayer.h"
#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaStaticMetatiles.h"
#include "gamedata/LaylaMetatile.h"
#include "gamedata/LaylaObjectPatternTable.h"
#include "editor/LonelyMetatileCache.h"
#include "editor/LonelyMetatileCacheSet.h"
#include "editor/LonelyLevelViewManager.h"
#include "editor/LonelyControlViewManager.h"
#include <vector>

namespace Lonely {


class LonelyLevelView {
public:
  friend class LonelyLevelEditor;
  friend class LonelyLevelViewManager;
  friend class LonelyControlViewManager;
  
  enum ViewType {
    visual = 0,
    behavior,
    solidity
  };

  LonelyLevelView(int levelNum__,
                  LaylaLevel& level__,
                  LaylaGraphics& graphics__,
                  LaylaPalettes& palettes__,
                  LaylaStaticMetatiles& staticMetatiles__);
  
  void changeLevel(int levelNum__, LaylaLevel& level__);
  
  void reloadRefresh();
  
  int levelNum() const;
  int levelViewWidth() const;
  int levelViewHeight() const;
  int levelNativeWidth() const;
  int levelNativeHeight() const;
  void seekCamX(int camX__);
  void seekCamY(int camY__);
  void setCamW(int camW__);
  void setCamH(int camH__);
  double zoom() const;
  void setZoom(double zoom__);
  int camX() const;
  int camY() const;
  int camW() const;
  int camH() const;
  int realCamX() const;
  int realCamY() const;
  int realCamW() const;
  int realCamH() const;
  bool gridEnabled() const;
  void setGridEnabled(bool gridEnabled__);
  bool patternMarkersEnabled() const;
  void setPatternMarkersEnabled(bool patternMarkersEnabled__);
  bool loopsEnabled() const;
  void setLoopsEnabled(bool loopsEnabled__);
  bool spawnsEnabled() const;
  void setSpawnsEnabled(bool spawnsEnabled__);
  ViewType viewType() const;
  void setViewType(ViewType viewType__);
  
  void drawViews();
  
  void drawLevelView();
  Graphic& levelView();
  
  void drawControlView();
  Graphic& controlView();
  
  static int levelNativeHeightReal();
  
  void setSourcePattern(Tbyte patternNum);
  
  void setControlCallbacks(void* callbackObj__,
                    void (*nodeSelectedCallback__)
                      (void*,TokenIndexCollection&),
                    void (*nodeDeselectedCallback__)(void*),
                    void (*spawnsChangedCallback__)(void*));
  
  LaylaPatternToken& patternToken(int index);
  
  void removeTokenFromSelectedNode(int index);
  
  void appendTokenToSelectedNode(LaylaPatternToken* token);
  
  bool nodeSelected() const;
  int selectedNodeIndex() const;
  
  bool hasSelectedPattern() const;
  Tbyte selectedPatternNum() const;
  
  bool overwriteEnabled() const;
  void setOverwriteEnabled(bool overwriteEnabled__);
  
  bool copyNodesEnabled() const;
  void setCopyNodesEnabled(bool copyNodesEnabled__);
  
  TokenIndexCollection listOfSpawns() const;
  
  void seekSpawn(int spawnIndex);
  
  void addSpawnAtSelectedNode();
  
  void deleteSelectedNode();
  
  LonelyLevelViewManager::Tool currentTool() const;
  void changeTool(LonelyLevelViewManager::Tool tool__);
  
  bool toolOverwriteAvailable() const;
  bool toolCopyNodesAvailable() const;
  
  bool moveNodeItemUp(int index);
  bool moveNodeItemDown(int index);
  
  void exportLevelMap(const std::string& filename);
  
protected:
  typedef std::vector<int> DrawXCollection;
  typedef std::vector<int> AreaTypeCollection;

  const static Tcolor backgroundColor_;
  const static Tcolor controlNodeBorderColor_;
  const static Tcolor controlNodeFillColor_;
  const static Tcolor leftJumpColor_;
  const static Tcolor rightJumpColor_;
                                      
  const static int topDisplayHeight_ = LaylaMetatile::height * 1;
  const static int bottomDisplayHeight_ = LaylaMetatile::height * 2;
  const static int levelYOffset_ = topDisplayHeight_;
  const static int levelNativeHeightReal_ = 
    (12 * LaylaMetatile::height);  // actual level height
  const static int bottomDisplayOffset_
    = levelYOffset_ + levelNativeHeightReal_;
  const static int levelNativeHeight_ = 
    levelNativeHeightReal_  // actual level height
    + (topDisplayHeight_)   // extra space for displays
    + (bottomDisplayHeight_);
  const static int leftJumpLineYOffset_ = 4;
  const static int rightJumpLineYOffset_ = 2;

  const static int spawnXOffset_ = (LaylaMetatile::width * 1);
  const static int spawnYOffset_ = levelYOffset_
                                      + (LaylaMetatile::height * 9) - 8;
  const static int spawnNumXOffset_ = spawnXOffset_ - 4;
  const static int spawnNumYOffset_ = spawnYOffset_ - 8;
  const static int scrollingDisabledXOffset_
    = -((LaylaMetatile::width) + (LaylaMetatile::width / 2));

  const static int elevatorNumXOffset_ = (LaylaMetatile::width * -2)
                                              - 4;
  const static int elevatorUpNumYOffset_ = levelYOffset_
                                      - 8;
  const static int elevatorDownNumYOffset_ = levelYOffset_
                                      + levelNativeHeightReal_;
  
  const static int controlViewHeight_ = LaylaMetatile::height * 2;
  const static int controlViewNodeXOffset_ = -8;
  const static int controlViewNodeYOffset_ = 8;
  const static int controlViewNodeW_ = 16;
  const static int controlViewNodeH_ = 16;
  const static int controlViewNodeFillXOffset_
      = controlViewNodeXOffset_ + 2;
  const static int controlViewNodeFillYOffset_
      = controlViewNodeYOffset_ + 2;
  const static int controlViewNodeFillW_ = 12;
  const static int controlViewNodeFillH_ = 12;
  const static int controlViewNodeLineWidth_ = 2;
  
  // Some displays (e.g. elevator destinations) are offset to the
  // left by enough that if we stop rendering when the screen is filled,
  // the displays won't show up even though they logically "should" be
  // onscreen. This overdraw adds some tolerance to deal with that.
  const static int rightOverdrawW_ = (LaylaMetatile::width * 6);
  
  LonelyLevelViewManager levelManager;
  LonelyControlViewManager controlManager;

  LaylaLevel* level_;
  LaylaGraphics* graphics_;
  LaylaPalettes* palettes_;
  LaylaStaticMetatiles* staticMetatiles_;
  
  int levelNum_;
  int camX_;
  int camY_;
  int camW_;
  int camH_;
  double zoom_;
  bool gridEnabled_;
  bool patternMarkersEnabled_;
  bool loopsEnabled_;
  bool spawnsEnabled_;
  ViewType viewType_;
  
  int patternPos_;
  int patternSubX_;
  
  int modifyBoxX_;
  int modifyBoxY_;
  int modifyBoxW_;
  int modifyBoxH_;
  
  int pasteBoxX_;
  int pasteBoxY_;
  int pasteBoxW_;
  int pasteBoxH_;
  
  void* spawnsChangedCallbackObj_;
  void (*spawnsChangedCallback_)(void*);
  
  Graphic levelCache_;
  Graphic controlCache_;
  
  LonelyMetatileCacheSet metatileCaches_;
  
  GridGraphicSceneLayer grid_;
  
  void cacheMetatiles();
  
  LaylaPattern& currentPattern();
  
  LaylaPattern& patternDefinition(int patternID);
  
  LaylaPattern& patternDefinitionAtStandardPos(int index);
  
  void seekStartingPattern();
  
  int findStartingDrawAreaType();
  
  int findJumpLength(int startIndex, int target);
  
  int findPreviousStandardPatternIndexNext(int startIndex);
  
  int findPreviousStandardPatternIndex(int startIndex);
  int findNextStandardPatternIndex(int startIndex);
  
  void drawNode(Graphic& dst,
                int drawX,
                bool filled,
                bool selected);
  
  int findNextStandardPatternIndexAndUpdateAreaType(int startIndex,
                                                int& areaType);
  
  int findTargetPatternIndex(int targetX, int targetY);
  
  void drawPatternAndAdvance(LaylaPattern& pattern,
                             int& drawX,
                             int areaType,
                             DrawXCollection& patternMarkers);
                             
  int patternDrawX(int index);
  
  void overwritePatternsAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  void overwritePatternsAndNodesAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  void insertPatternsAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  void insertPatternsAndNodesAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  void deletePatternsAndNodesAt(int index,
                        int numPatterns);
                        
  void resetToPattern(int index);
  
  void deleteNode(int index);
  
  NesPatternTable& backgroundForArea(int areaType);
  NesPatternTable& spritesForArea(int areaType);
  NesPaletteQuad backgroundPalettesForArea(int areaType);
  NesPaletteQuad spritePalettesForArea(int areaType);
};


};


#endif
