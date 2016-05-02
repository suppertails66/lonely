#ifndef LONELYRESISTANCEVIEW_H
#define LONELYRESISTANCEVIEW_H


#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaVulnerabilitySettings.h"
#include "structs/Tstring.h"
#include "structs/Graphic.h"

namespace Lonely {


class LonelyResistanceView {
public:
  LonelyResistanceView(int levelNum__,
                    LaylaLevel& level__,
                    LaylaGraphics& graphics__,
                    LaylaPalettes& palettes__);
  
  void changeLevel(int levelNum__, LaylaLevel& level__);
  
  void drawObjectIcon(Graphic& dst,
                    int localObjectNum);
  
  void drawBossIcon(Graphic& dst,
                    int bossNum);
  
  LaylaVulnerabilitySettings& objectResistances(int objNum);
  
  LaylaVulnerabilitySettings& bossResistances(int bossNum);
  
  Tbyte& objectHp(int objNum);
  
  int numObjectResistances() const;
  int numBossResistances() const;
  
  Tstring nameOfObject(int objNum);
  Tstring nameOfBoss(int bossNum);
protected:
  const static int numObjectResistances_ = 0x10;
  const static int numBossResistances_ = 0x05;
  
  int levelNum_;
  LaylaLevel* level_;
  LaylaGraphics* graphics_;
  LaylaPalettes* palettes_;
};


};


#endif
