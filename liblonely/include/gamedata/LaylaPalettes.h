#ifndef LAYLAPALETTES_H
#define LAYLAPALETTES_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tarray.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "nes/NesRom.h"
#include "nes/NesColorData.h"
#include "gamedata/LaylaOffsetFile.h"
#include "gamedata/LaylaPalettePatch.h"
#include <vector>

namespace Lonely {


class LaylaPalettes {
protected:
  typedef Tarray<LaylaPalettePatch> PalettePatchArray;
  
public:
  LaylaPalettes();
  
  LaylaPalettes(const NesRom& rom,
                const LaylaOffsetFile& offsets);
  
  void exportToRom(NesRom& rom) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  NesColorData generateCavePalette(int levelNum) const;
  NesColorData generateBasePalette(int levelNum) const;
  NesColorData generateBossPalette(int levelNum) const;
  
  NesColorData generateAreaPalette(
    Tbyte areaNum,
    int levelNum) const;
  
  NesColorData& standardPalette();
  NesColorData& bossPalette();
  LaylaPalettePatch& caveReplacementBackgroundPalette(int index);
  LaylaPalettePatch& baseReplacementBackgroundPalette(int index);
  LaylaPalettePatch& bossReplacementSpritePalette(int index);
  LaylaPalettePatch& laylaFadeSpritePalette(int index);
  
  int cavePatchIndex(int levelNum) const;
  int basePatchIndex(int levelNum) const;
  int bossPatchIndex(int levelNum) const;
  
  static void readPatchTable(PalettePatchArray& patches,
                      const Tbyte* src,
                      int startIndex,
                      int numEntries,
                      int colorsPerEntry);
  
  static void writePatchTable(
                      Tbyte* dst,
                      const PalettePatchArray& patches);
  
  static int savePatchTable(Tstring& data,
                      const PalettePatchArray& patches);
  
  static int loadPatchTable(PalettePatchArray& patches,
                      const Tbyte* data);
                      
protected:
  const static char* offsetFilePalettesName_;
  
  const static int caveReplacementBackgroundStartIndex_ = 0x4;
  const static int caveReplacementBackgroundColorsPerEntry_ = 0x0C;
  const static int baseReplacementBackgroundStartIndex_ = 0x4;
  const static int baseReplacementBackgroundColorsPerEntry_ = 0x0C;
  const static int bossReplacementSpriteStartIndex_ = 0x5;
  const static int bossReplacementSpriteColorsPerEntry_ = 0x03;
  const static int paletteCycleTableStartIndex_ = 0x0F;
  const static int paletteCycleTableColorsPerEntry_ = 0x01;
  const static int laylaFadeStartIndex_ = 0x00;
  const static int laylaFadeColorsPerEntry_ = 0x04;
  
  Taddress standardPaletteOffset_;
  Taddress bossPaletteOffset_;
  Taddress caveReplacementTableOffset_;
  Taddress numCaveReplacementTableEntries_;
  Taddress baseReplacementTableOffset_;
  Taddress numBaseReplacementTableEntries_;
  Taddress bossReplacementTableOffset_;
  Taddress numBossReplacementTableEntries_;
  Taddress laylaFadeTableOffset_;
  Taddress numLaylaFadeTableEntries_;
  
  NesColorData standardPalette_;
  NesColorData bossPalette_;
  PalettePatchArray caveReplacementBackgroundPalettes_;
  PalettePatchArray baseReplacementBackgroundPalettes_;
  PalettePatchArray bossReplacementSpritePalettes_;
  PalettePatchArray laylaFadeSpritePalettes_;
  
};


};


#endif
