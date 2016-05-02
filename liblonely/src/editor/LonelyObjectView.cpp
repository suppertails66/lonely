#include "editor/LonelyObjectView.h"
#include <cstdlib>

namespace Lonely {


/*LonelyObjectView::LonelyObjectView()
  : levelNum_(0),
    level_(NULL),
    graphics_(NULL),
    palettes_(NULL) { }; */

LonelyObjectView::LonelyObjectView(int levelNum__,
                   LaylaLevel& level__,
                   LaylaGraphics& graphics__,
                   LaylaPalettes& palettes__,
                   const LaylaObjectSetCollection& objectSets__)
  : levelNum_(levelNum__),
    level_(&level__),
    graphics_(&graphics__),
    palettes_(&palettes__),
    objectSets_(&objectSets__) { };

void LonelyObjectView::changeLevel(int levelNum__, LaylaLevel& level__) {
  levelNum_ = levelNum__;
  level_ = &level__;
//  changePattern(0);
}
  
const LaylaObjectPatternTable& LonelyObjectView::objectPatterns() const {
  return level_->objectPatterns();
}
  
int LonelyObjectView::currentObjectSetNum() const {
  return level_->objectSetNum();
}

int LonelyObjectView::currentLevelNum() const {
  return levelNum_;
}

const LaylaGraphics& LonelyObjectView::graphics() const {
  return *graphics_;
}

const LaylaPalettes& LonelyObjectView::palettes() const {
  return *palettes_;
}
  
LaylaObjectPatternToken& LonelyObjectView::patternToken(int index) {
  return level_->objectPatterns().patternToken(index);
}
  
int LonelyObjectView::numObjectSets() const {
  return LaylaObjectSets::numObjectSets;
}
  
void LonelyObjectView::changeObjectSet(int objectSetNum) {
  level_->changeObjectSet((*objectSets_)[objectSetNum]);
  
//  level_->objectSetNum() = objectSetNum;
}
  
void LonelyObjectView::drawObjectIcon(Graphic& dst,
                    int localObjectNum) {
  level_->objectPatterns().drawObjectIcon(
      dst,
      levelNum_,
      localObjectNum,
      level_->objectSetNum(),
      *graphics_,
      *palettes_);
}
  
int LonelyObjectView::removeObjectPattern(int index) {
  int result = level_->objectPatterns().removeObjectPattern(index);
  
  return result;
}
  
int LonelyObjectView
    ::addObjectPattern(int index, LaylaObjectPatternToken& token) {
  return level_->objectPatterns().addObjectPattern(index, token);
}
  
int LonelyObjectView::moveObjectPatternUp(int index) {
  if ((index <= 0)
      || (index >= (level_->objectPatterns().numPatternTokens()))) {
    return index;
  }
  
  level_->objectPatterns().swapItems(index, index - 1);
  
  return index - 1;
}

int LonelyObjectView::moveObjectPatternDown(int index) {
  if ((index < 0)
      || (index >= (level_->objectPatterns().numPatternTokens() - 1))) {
    return index;
  }
  
  level_->objectPatterns().swapItems(index, index + 1);

  return index + 1;
}


};
