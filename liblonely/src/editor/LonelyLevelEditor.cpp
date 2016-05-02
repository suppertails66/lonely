#include "editor/LonelyLevelEditor.h"
#include "util/StringConversion.h"
#include <iostream>

namespace Lonely {


LonelyLevelEditor::LonelyLevelEditor(LaylaLevelSet& levels__,
                  LaylaGraphics& graphics__,
                  LaylaPalettes& palettes__,
                  LaylaStaticMetatiles& staticMetatiles__,
                  const LaylaObjectSetCollection& objectSets__)
  : currentLevelNum_(0),
    levels_(levels__),
    graphics_(graphics__),
    palettes_(palettes__),
    staticMetatiles_(staticMetatiles__),
    objectSets_(objectSets__),
    levelView_(currentLevelNum_,
               levels_.level(currentLevelNum_),
               graphics_,
               palettes_,
               staticMetatiles_),
    patternView_(currentLevelNum_,
               levels_.level(currentLevelNum_),
               graphics_,
               palettes_,
               staticMetatiles_),
    objectView_(currentLevelNum_,
               levels_.level(currentLevelNum_),
               graphics_,
               palettes_,
               objectSets_),
    metatileView_(currentLevelNum_,
               levels_,
               levels_.level(currentLevelNum_),
               graphics_,
               palettes_,
               staticMetatiles_),
    paletteView_(currentLevelNum_,
               levels_.level(currentLevelNum_),
               palettes_),
    resistanceView_(currentLevelNum_,
               levels_.level(currentLevelNum_),
               graphics_,
               palettes_) { };
  
int LonelyLevelEditor::numLevels() {
  return levels_.numLevels();
}
  
Tstring LonelyLevelEditor::levelName(int levelNum) {
  return Tstring("Asteroid ")
    + StringConversion::toString(levelNum + 1);
}
  
LonelyLevelView& LonelyLevelEditor::levelView() {
  return levelView_;
}
  
LonelyPatternView& LonelyLevelEditor::patternView() {
  return patternView_;
}
  
LonelyObjectView& LonelyLevelEditor::objectView() {
  return objectView_;
}
  
LonelyMetatileView& LonelyLevelEditor::metatileView() {
  return metatileView_;
}
  
LonelyPaletteView& LonelyLevelEditor::paletteView() {
  return paletteView_;
}
  
LonelyResistanceView& LonelyLevelEditor::resistanceView() {
  return resistanceView_;
}
  
void LonelyLevelEditor::loadLevel(int levelNum) {
  currentLevelNum_ = levelNum;
  
/*  double oldZoom = levelView_.zoom();
  levelView_ = LonelyLevelView(currentLevelNum_,
                               levels_.level(currentLevelNum_),
                               graphics_,
                               palettes_,
                               staticMetatiles_);
  levelView_.setZoom(oldZoom); */
  levelView_.changeLevel(levelNum, levels_.level(levelNum));
  patternView_.changeLevel(levelNum, levels_.level(levelNum));
  objectView_.changeLevel(levelNum, levels_.level(levelNum));
  metatileView_.changeLevel(levelNum, levels_.level(levelNum));
  paletteView_.changeLevel(levelNum, levels_.level(levelNum));
  resistanceView_.changeLevel(levelNum, levels_.level(levelNum));
}
  
int LonelyLevelEditor::currentLevelNum() const {
  return currentLevelNum_;
}
  
int LonelyLevelEditor::currentObjectSetNum() const {
  return levels_.level(currentLevelNum_).objectSetNum();
}
  
void LonelyLevelEditor::generatePatternPreviews(
                             PatternPreviewCollection& dst,
                             PatternPreviewAreaTypeCollection& areaTypes) {
  // Draw previews
  for (int i = 0; i < areaTypes.size(); i++) {
    Tbyte areaType = areaTypes[i];
    LonelyMetatileCache* targetCache = NULL;
    MetatileCacheCollection* targetMetatileCache = NULL;

    if (areaType & SetAreaToken::bossFlag) {
      targetCache = &(levelView_.metatileCaches_.bossCache());
    }
    else if (areaType & SetAreaToken::baseFlag) {
      targetCache = &(levelView_.metatileCaches_.baseCache());
    }
    else {
      targetCache = &(levelView_.metatileCaches_.caveCache());
    }
    
    // Draw the pattern
    LaylaPattern& pattern = currentLevel().patternDefinitions().pattern(i);
    targetMetatileCache = &(targetCache->metatilesVisual());
    Graphic g(pattern.realWidth(), pattern.realHeight());
    pattern.draw(g,
                 0, 0,
                 *targetMetatileCache,
                 false);
    
    dst.push_back(g);
  }
}
  
void LonelyLevelEditor
    ::generatePatternPreviews(PatternPreviewCollection& dst) {
  dst.clear();
  
  // Generate area type guesses
  PatternPreviewAreaTypeCollection areaTypes;
  generatePatternAreaTypeGuesses(areaTypes);
  
  // Draw previews
  generatePatternPreviews(dst,
                          areaTypes);
}
  
void LonelyLevelEditor
    ::generatePatternAreaTypeGuesses(PatternPreviewAreaTypeCollection& dst) {
  currentLevel().areaData().generatePatternAreaTypeGuesses(
    dst,
    currentLevel().patternDefinitions());
}
  
LaylaLevel& LonelyLevelEditor
    ::currentLevel() {
  return levels_.level(currentLevelNum_);
}

const LaylaLevel& LonelyLevelEditor
    ::currentLevel() const {
  return levels_.level(currentLevelNum_);
}


};
