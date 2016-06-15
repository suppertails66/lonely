#ifndef LONELYPATTERNEDITINGSCENE_H
#define LONELYPATTERNEDITINGSCENE_H


#include "structs/StandardEditingScene.h"
#include "gamedata/LaylaPattern.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "editor/LonelyMetatileCache.h"
#include "editor/MetatileViewType.h"

namespace Lonely {


class LonelyPatternEditingScene : public StandardEditingScene {
public:
  LonelyPatternEditingScene();
  
  LonelyPatternEditingScene(LaylaPattern& pattern__,
                            LonelyMetatileCache& metatiles__);
  
  virtual void drawEditable(Graphic& dst,
                              int x, int y,
                              int editableIndex);
  
  virtual int indexOfEditableAt(int editableX, int editableY);
  
  virtual void overwriteEditableAt(int editableX, int editableY,
                                   int newIndex);
  
  void changePattern(LaylaPattern& pattern__);
  
  void changeMetatiles(LonelyMetatileCache& metatiles__);
  
  void changeViewType(MetatileViewTypes::MetatileViewType viewType__);
  
  bool ignoreMetatileZero() const;
  void setIgnoreMetatileZero(bool ignoreMetatileZero__);
protected:
  LaylaPattern* pattern_;
  LonelyMetatileCache* metatiles_;
  
  MetatileViewTypes::MetatileViewType viewType_;
  
  bool ignoreMetatileZero_;
  
  Graphic& metatileGraphic(int index);
};


};


#endif
