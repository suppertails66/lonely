#include "editor/LonelyResistanceView.h"
#include "editor/LonelyPalettePickerSceneFunctor.h"
#include "gamedata/LaylaObjectSets.h"
#include "gamedata/LonelyLaylaObject.h"
#include "gamedata/LaylaBoss.h"
#include "gamedata/LonelyLaylaMappings.h"
#include <iostream>

namespace Lonely {


LonelyResistanceView::LonelyResistanceView(int levelNum__,
                    LaylaLevel& level__,
                    LaylaGraphics& graphics__,
                    LaylaPalettes& palettes__)
  : levelNum_(levelNum__),
    level_(&level__),
    graphics_(&graphics__),
    palettes_(&palettes__) {
  changeLevel(levelNum__, level__);
}
  
void LonelyResistanceView::changeLevel(int levelNum__, LaylaLevel& level__) {
  levelNum_ = levelNum__;
  level_ = &level__;
}
  
void LonelyResistanceView::drawObjectIcon(Graphic& dst,
                    int localObjectNum) {
  level_->objectPatterns().drawObjectIcon(
      dst,
      levelNum_,
      localObjectNum,
      level_->objectSetNum(),
      *graphics_,
      *palettes_);
}
  
void LonelyResistanceView::drawBossIcon(Graphic& dst,
                    int bossNum) {
  dst.resize(32, 32);
  dst.clear(Tcolor(0x00, 0x00, 0x00));
  
  LonelyLaylaMapping bossMapping
    = LonelyLaylaMappings::lonelyBossMapping(
        static_cast<LaylaBosses::LaylaBoss>(bossNum));
  
  Graphic bossG;
  bossMapping.draw(bossG,
                   graphics_->bossSprites(),
                   palettes_->generateBossPalette(levelNum_).spritePalettes(),
                   true);
  
  int offsetX = (dst.w() - bossG.w()) / 2;
  int offsetY = (dst.h() - bossG.h()) / 2;
  
  dst.blit(bossG,
           Box(offsetX, offsetY, 0, 0),
           Graphic::noTransUpdate);
}
  
LaylaVulnerabilitySettings&
    LonelyResistanceView::objectResistances(int objNum) {
  return level_->objectVulnerabilities().flags(objNum);
}

LaylaVulnerabilitySettings&
    LonelyResistanceView::bossResistances(int bossNum) {
  return level_->bossVulnerabilities().flags(bossNum);
}
  
Tbyte& LonelyResistanceView::objectHp(int objNum) {
  return level_->objectHps().hp(objNum);
}
  
int LonelyResistanceView::numObjectResistances() const {
  return numObjectResistances_;
}

int LonelyResistanceView::numBossResistances() const {
  return numBossResistances_;
}
  
Tstring LonelyResistanceView::nameOfObject(int objNum) {
  LonelyLaylaObjects::LonelyLaylaObject lonelyID
    = LaylaObjectSets::lonelyID(level_->objectSetNum(),
                                objNum);
  
  return LonelyLaylaObjects::nameOfObject(lonelyID);
}
  
Tstring LonelyResistanceView::nameOfBoss(int bossNum) {
  return LaylaBosses::nameOfBoss(
    static_cast<LaylaBosses::LaylaBoss>(bossNum));
}


};
