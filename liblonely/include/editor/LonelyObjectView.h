#ifndef LONELYOBJECTVIEW_H
#define LONELYOBJECTVIEW_H


#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaObjectPatternToken.h"
#include "gamedata/LaylaObjectSet.h"
#include "structs/Graphic.h"

namespace Lonely {


class LonelyObjectView {
public:
//  LonelyObjectView();
  
  LonelyObjectView(int levelNum__,
                   LaylaLevel& level__,
                   LaylaGraphics& graphics__,
                   LaylaPalettes& palettes__,
                   const LaylaObjectSetCollection& objectSets__);
  
  void changeLevel(int levelNum__, LaylaLevel& level__);
  
  const LaylaObjectPatternTable& objectPatterns() const;
  int currentObjectSetNum() const;
  int currentLevelNum() const;
  const LaylaGraphics& graphics() const;
  const LaylaPalettes& palettes() const;
  
  LaylaObjectPatternToken& patternToken(int index);
  
  int numObjectSets() const;
  
  void changeObjectSet(int objectSetNum);
  
  void drawObjectIcon(Graphic& dst,
                      int localObjectNum);
  
  int removeObjectPattern(int index);
  
  int addObjectPattern(int index, LaylaObjectPatternToken& token);
  
  int moveObjectPatternUp(int index);
  
  int moveObjectPatternDown(int index);
  
protected:
  int levelNum_;
  LaylaLevel* level_;
  LaylaGraphics* graphics_;
  LaylaPalettes* palettes_;
  const LaylaObjectSetCollection* objectSets_;
  
};


};


#endif
