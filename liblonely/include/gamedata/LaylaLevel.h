#ifndef LAYLALEVEL_H
#define LAYLALEVEL_H


#include "structs/Tbyte.h"
#include "structs/Tarray.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "gamedata/LaylaArea.h"
#include "gamedata/LaylaPatternDefinitionTable.h"
#include "gamedata/LaylaVulnerabilityArray.h"
#include "gamedata/LaylaObjectScoreArray.h"
#include "gamedata/LaylaHpArray.h"
#include "gamedata/LaylaMetatileSet.h"
#include "gamedata/LaylaObjectPatternTable.h"
#include "gamedata/LaylaObjectSet.h"
#include "gamedata/LaylaPalettePatch.h"
#include "gamedata/LaylaVulnerabilitySettings.h"

namespace Lonely {


class LaylaLevel {
protected:
  typedef Tarray<LaylaPalettePatch> PalettePatchArray;
public:
  const static int numObjectVulnerabilities = 0x10;
  const static int numBossVulnerabilities = 0x5;
  
  LaylaLevel();
  
  void readFromData(const NesRom& rom,
                    int bankNum,
                    int objectSetNum__,
                    Taddress objectCodeBlockStart,
                    Taddress objectCodeBlockEnd,
                    int numPatternDefinitionTableEntries,
                    int numObjectPatternTableEntries,
                    int objectPatternDataSize,
                    Taddress spawnTableStart,
                    int numSpawnTableEntries,
                    Taddress areaDataStart,
                    Taddress areaDataEnd);
  
  void exportToRom(NesRom& rom,
                   int bankNum,
                   Taddress freeSpaceStart,
                   int freeSpaceSize,
                   const LaylaObjectSetCollection& objectSets) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  void changeObjectSet(const LaylaObjectSet& objectSet);
  
  LaylaVulnerabilityArray& bossVulnerabilities();
  
  LaylaVulnerabilityArray& objectVulnerabilities();
  
  LaylaObjectScoreArray& objectScores();
  
  LaylaHpArray& objectHps();
  
  LaylaObjectPatternTable& objectPatterns();
  
  LaylaMetatileSet& baseMetatileSet();
  
  LaylaPatternDefinitionTable& patternDefinitions();
  
  int& objectSetNum();
  
  LaylaArea& areaData();
  
  PalettePatchArray& paletteCyclePatches();
  PalettePatchArray& regularHpCyclePatches();
  PalettePatchArray& lowHpCyclePatches();
  PalettePatchArray& specialFlashCyclePatches();
  
  Tarray<Tbyte>& bossInitTable();
  Tarray<Tbyte>& objectInitTable();
  Taddress& objectCodeBlockBaseOffset();
  Tarray<Tbyte>& objectCodeBlock();
  
  void fixAfterMetatileQuantityChange(
    Tbyte areaType,
    int numMetatiles,
    int index);
protected:
  const static Taddress commonCodeBlockBaseOffset_ = 0x0000;
  const static int commonCodeBlockSize_ = 0x13D9;
  
  const static Taddress paletteCycleBaseAddress_ = 0x076D;
  const static int numPaletteCycleEntries_ = 4;
  const static int numColorsPerPaletteCycleEntry_ = 1;
  const static int paletteCyclePositionIndex_ = 0x0F;
  const static Taddress lowHpCycleBaseAddress_ = 0x03C1;
  const static Taddress specialFlashCycleBaseAddress_ = 0x03C4;
  const static Taddress regularHpCycleBaseAddress_ = 0x03C7;
  
  const static Taddress levelStartPointersBaseOffset_ = 0x13D9;
  const static int numLevelStartPointersEntries = 2;
  
  const static Taddress levelPointerTableBaseStart_ = 0x13D9;
  const static Taddress levelPointerTableBaseEnd_ = 0x13DD;
  const static Taddress levelPointerTableSize_
      = levelPointerTableBaseEnd_ - levelPointerTableBaseStart_;
  
  const static Taddress bossInitTableBaseStart_ = 0x13DD;
  const static Taddress bossInitTableBaseEnd_ = 0x13E7;
  const static Taddress bossInitTableSize_
      = bossInitTableBaseEnd_ - bossInitTableBaseStart_;
  
  const static Taddress bossVulnerabilityTableBase_ = 0x13E7;
  
  const static Taddress objectVulnerabilityTableBase_ = 0x13EC;
  
  const static Taddress objectScoreTableBase_ = 0x13FC;
  
  const static Taddress objectInitTableBaseStart_ = 0x140C;
  const static Taddress objectInitTableBaseEnd_ = 0x142C;
  const static Taddress objectInitTableSize_
      = objectInitTableBaseEnd_ - objectInitTableBaseStart_;
  
  const static Taddress objectHpTableBase_ = 0x142C;
  
  const static Taddress objectPatternTableBaseStart_ = 0x143C;
  const static Taddress objectPatternTableBaseEnd_ = 0x15AF;
  const static Taddress objectPatternTableSize_
    = objectPatternTableBaseEnd_ - objectPatternTableBaseStart_;
  
  const static Taddress baseMetatileAttributeTableBase_ = 0x15AF;
  const static Taddress baseEvenTileIndexTableBase_ = 0x15CC;
  const static Taddress baseOddTileIndexTableBase_ = 0x16B2;
  const static Taddress baseMetatilePropertiesBase_ = 0x1798;
  const static Taddress baseNumMetatilesPerSet_ = 0x73;
  
  const static Taddress patternDefinitionTableBase_ = 0x180B;
  
  Tarray<Tbyte> commonCodeBlock_;
  
  Tarray<Tbyte> levelPointerTable_;
  
  Tarray<Tbyte> bossInitTable_;
  
  LaylaVulnerabilityArray bossVulnerabilities_;
  
  LaylaVulnerabilityArray objectVulnerabilities_;
  
  LaylaObjectScoreArray objectScores_;
  
  Tarray<Tbyte> objectInitTable_;
  
  LaylaHpArray objectHps_;
  
  LaylaObjectPatternTable objectPatterns_;
  
  LaylaMetatileSet baseMetatileSet_;
  
  LaylaPatternDefinitionTable patternDefinitions_;
  
  int objectSetNum_;
  Taddress objectCodeBlockBaseOffset_;
  Tarray<Tbyte> objectCodeBlock_;
  
  LaylaArea areaData_;
  
  PalettePatchArray paletteCyclePatches_;
  PalettePatchArray regularHpCyclePatches_;
  PalettePatchArray lowHpCyclePatches_;
  PalettePatchArray specialFlashCyclePatches_;
  
};


};


#endif
