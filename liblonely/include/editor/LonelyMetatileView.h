#ifndef LONELYMETATILEVIEW_H
#define LONELYMETATILEVIEW_H


#include "structs/Graphic.h"
#include "structs/Tbyte.h"
#include "structs/Graphic.h"
#include "structs/GridGraphicSceneLayer.h"
#include "structs/CachedGraphic.h"
#include "gamedata/LaylaLevelSet.h"
#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaStaticMetatiles.h"
#include "gamedata/LaylaMetatile.h"
#include "gamedata/LaylaPatternToken.h"
#include "editor/LonelyMetatileCache.h"
#include "editor/LonelyMetatileCacheSet.h"
#include "editor/MetatileViewType.h"
#include "editor/LonelyMetatilePickerScene.h"
#include "editor/LonelyMetatilePickerSceneFunctor.h"
#include "editor/LonelyTilePickerScene.h"
#include "editor/LonelySubtilePickerScene.h"

namespace Lonely {


class LonelyMetatileView {
public:
  LonelyMetatileView(int levelNum__,
                     LaylaLevelSet& levelSet__,
                     LaylaLevel& level__,
                     LaylaGraphics& graphics__,
                     LaylaPalettes& palettes__,
                     LaylaStaticMetatiles& staticMetatiles__);
  
  void changeLevel(int levelNum__, LaylaLevel& level__);
  
  void changeArea(Tbyte areaType__);
  
  void changeMetatile(int index);
  
  void drawViews();
  
  Graphic& metatilePickerView();
  
  Graphic& tilePickerView();
  
  Graphic& subtilePickerView();
  
  void* metatilePickerCallbackObj();
  
  void* tilePickerCallbackObj();
  
  void* subtilePickerCallbackObj();
  
  void metatilePickerItemPicked(int index);
  
  void tilePickerItemPicked(int index);
  
  void subtilePickerItemPicked(int index);
  
//  double zoom() const;
//  void setZoom(double zoom__);
  
  bool metatilePickerZoom() const;
  void setMetatilePickerZoom(bool metatilePickerZoom__);
  
  bool gridEnabled() const;
  void setGridEnabled(bool gridEnabled__);
  
  MetatileViewTypes::MetatileViewType viewType() const;
  void setViewType(MetatileViewTypes::MetatileViewType viewType__);
  
  LaylaMetatile& activeMetatile();
  
  void changeActiveMetatilePalette(int index);
  void changeActiveMetatileBehavior(
      LaylaMetatileBehaviors::LaylaMetatileBehavior behavior);
  void changeActiveMetatileTopSolid(bool solid);
  void changeActiveMetatileBottomSolid(bool solid);
  
  int removeActiveMetatile();
  int addMetatile();
  
  int minNumMetatiles() const;
  
  int maxNumMetatiles() const;
  
  int numMetatiles() const;
protected:
  void cacheMetatiles();
  
  void refreshMetatiles();
  
  void refreshScenes();
  
  void refreshTilePickerTiles();
  
  void refreshActiveMetatile();
  
  void refreshAfterActiveMetatileUpdate();
  
  LaylaMetatileSet& activeMetatileSet();
  const LaylaMetatileSet& activeMetatileSet() const;

  int levelNum_;
  LaylaLevelSet* levelSet_;
  LaylaLevel* level_;
  LaylaGraphics* graphics_;
  LaylaPalettes* palettes_;
  LaylaStaticMetatiles* staticMetatiles_;
  
  LaylaMetatile* activeMetatile_;
  
//  double zoom_;
  bool metatilePickerZoom_;
  bool gridEnabled_;
  MetatileViewTypes::MetatileViewType viewType_;
  
  Tbyte areaType_;
  int pickedSubtileIndex_;
  
  LonelyMetatileCacheSet metatileCaches_;
  
  LonelyMetatilePickerScene metatilePickerScene_;
  CachedGraphic metatileCache_;
  
  LonelyTilePickerScene tilePickerScene_;
  CachedGraphic tileCache_;
  
  LonelySubtilePickerScene subtilePickerScene_;
  CachedGraphic subtileCache_;
};


};


#endif
