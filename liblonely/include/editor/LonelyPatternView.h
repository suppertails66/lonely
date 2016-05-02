#ifndef LONELYPATTERNVIEW_H
#define LONELYPATTERNVIEW_H


#include "structs/Tbyte.h"
#include "structs/Graphic.h"
#include "structs/GridGraphicSceneLayer.h"
#include "structs/CachedGraphic.h"
#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaStaticMetatiles.h"
#include "gamedata/LaylaMetatile.h"
#include "gamedata/LaylaObjectPatternTable.h"
#include "editor/LonelyMetatileCache.h"
#include "editor/LonelyMetatileCacheSet.h"
#include "editor/MetatileViewType.h"
#include "editor/LonelyPatternEditingScene.h"
#include "editor/LonelyMetatilePickerScene.h"

namespace Lonely {


class LonelyPatternView {
public:
  
  LonelyPatternView(int levelNum__,
                     LaylaLevel& level__,
                     LaylaGraphics& graphics__,
                     LaylaPalettes& palettes__,
                     LaylaStaticMetatiles& staticMetatiles__);
  
  void changeLevel(int levelNum__, LaylaLevel& level__);
  
  void cacheMetatiles();
  
  void reloadRefresh();
  
  void drawViews();
  
  Graphic& patternView();
  
  Graphic& metatilePickerView();
  
  Graphic& objectsView();
  
  const LaylaObjectPatternTable& objectPatterns() const;
  
  void changePattern(int index);
  
  void changeMetatiles(int areaType);
  
  void setPatternViewSize(int w__, int h__);
  
  void setZoom(double zoom__);
  
  bool gridEnabled() const;
  void setGridEnabled(bool gridEnabled__);
  
  int activePatternWidth() const;
  void changeActivePatternWidth(int width);
  
  void* patternViewCallbackObj();
  
  void* metatilePickerCallbackObj();
  
  MetatileViewTypes::MetatileViewType viewType() const;
  void changeViewType(MetatileViewTypes::MetatileViewType viewType__);
  
  void metatilePickerItemPicked(int index);
  
  bool metatilePickerZoomEnabled() const;
  void setMetatilePickerZoomEnabled(bool enabled);
  
  Tbyte activeAreaType() const;
  
  int currentObjectPatternIndex() const;
  void setCurrentObjectPatternIndex(int newIndex);
  
  int insertNewPattern();
  
  int removePattern();
  
  const NesPatternTable& currentSpritePatternTable() const;
  NesPaletteQuad currentSpritePalettes() const;
  int currentObjectSetNum() const;
//  int currentLevelNum() const;
//  const LaylaGraphics& graphics() const;
//  const LaylaPalettes& palettes() const;

  StandardEditingScene::Tool activeTool() const;
  void changeActiveTool(StandardEditingScene::Tool tool);
protected:
  const static int objectsGraphicHeight_ = 32;

  LaylaPattern& activePattern();
  const LaylaPattern& activePattern() const;
  
  void reloadActivePattern();
  
  void refreshMetatilePickerZoom();
  
  void regenerateDefaultAreaTypes();

  LonelyPatternEditingScene patternEditingScene_;
  CachedGraphic patternCache_;
  
  LonelyMetatilePickerScene metatilePickerScene_;
  CachedGraphic metatileCache_;
  
  CachedGraphic objectsCache_;

  int levelNum_;
  int patternNum_;
  LaylaLevel* level_;
  LaylaGraphics* graphics_;
  LaylaPalettes* palettes_;
  LaylaStaticMetatiles* staticMetatiles_;
  
  PatternPreviewAreaTypeCollection defaultAreaTypes_;
  
  double zoom_;
  bool metatilePickerZoom_;
  bool gridEnabled_;
  MetatileViewTypes::MetatileViewType viewType_;
  LonelyMetatileCacheSet metatileCaches_;
  GridGraphicSceneLayer grid_;
  
  Tbyte activeAreaType_;
  
};


};


#endif
