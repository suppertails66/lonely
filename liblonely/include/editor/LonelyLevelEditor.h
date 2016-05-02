#ifndef LONELYLEVELEDITOR_H
#define LONELYLEVELEDITOR_H


#include "structs/Tstring.h"
#include "gamedata/LaylaLevelSet.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaStaticMetatiles.h"
#include "gamedata/LaylaObjectSet.h"
#include "editor/LonelyLevelView.h"
#include "editor/LonelyPatternView.h"
#include "editor/LonelyObjectView.h"
#include "editor/LonelyMetatileView.h"
#include "editor/LonelyPaletteView.h"
#include "editor/LonelyResistanceView.h"

namespace Lonely {


typedef std::vector<Graphic> PatternPreviewCollection;

class LonelyLevelEditor {
public:
  LonelyLevelEditor(LaylaLevelSet& levels__,
                    LaylaGraphics& graphics__,
                    LaylaPalettes& palettes__,
                    LaylaStaticMetatiles& staticMetatiles__,
                    const LaylaObjectSetCollection& objectSets__);
  
  int numLevels();
  
  static Tstring levelName(int levelNum);
  
  LonelyLevelView& levelView();
  
  LonelyPatternView& patternView();
  
  LonelyObjectView& objectView();
  
  LonelyMetatileView& metatileView();
  
  LonelyPaletteView& paletteView();
  
  LonelyResistanceView& resistanceView();
  
  void loadLevel(int levelNum);
  
  int currentLevelNum() const;
  
  int currentObjectSetNum() const;
  
  void generatePatternPreviews(PatternPreviewCollection& dst,
                               PatternPreviewAreaTypeCollection& areaTypes);
  
  void generatePatternPreviews(PatternPreviewCollection& dst);
  
  void generatePatternAreaTypeGuesses(PatternPreviewAreaTypeCollection& dst);
protected:
  int currentLevelNum_;

  LaylaLevelSet& levels_;
  LaylaGraphics& graphics_;
  LaylaPalettes& palettes_;
  LaylaStaticMetatiles& staticMetatiles_;
  const LaylaObjectSetCollection& objectSets_;
  
  LonelyLevelView levelView_;
  
  LonelyPatternView patternView_;
  
  LonelyObjectView objectView_;
  
  LonelyMetatileView metatileView_;
  
  LonelyPaletteView paletteView_;
  
  LonelyResistanceView resistanceView_;
  
  LaylaLevel& currentLevel();
  
  const LaylaLevel& currentLevel() const;
  
};


};


#endif
