#ifndef LAYLADATA_H
#define LAYLADATA_H


#include "structs/Tstring.h"
#include "structs/Tcolor.h"
#include "gamedata/LaylaOffsetFile.h"
#include "gamedata/LaylaLevel.h"
#include "gamedata/LaylaLevelSet.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaStaticMetatiles.h"
#include "gamedata/LaylaHacks.h"
#include "gamedata/LaylaMetatileSet.h"
#include "gamedata/LaylaObjectSets.h"
#include "gamedata/LaylaObjectSet.h"
#include <vector>
#include <unordered_map>

namespace Lonely {


class LaylaData {
public:
  LaylaData(const Tstring& romFileName,
            const Tstring& offsetFileName);
  
  void exportToFile(const Tstring& romFileName);
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  LaylaLevelSet& levels();
  
  LaylaPalettes& palettes();
  
  LaylaGraphics& graphics();
  
  LaylaStaticMetatiles& staticMetatiles();
  
  int numObjectSets() const;
  LaylaObjectSet& objectSet(int index);
  const LaylaObjectSet& objectSet(int index) const;
  const LaylaObjectSetCollection& objectSets() const;
  
  static int numExportPrgBanks();
  static int numExportChrBanks();
  static NesRom::NametableArrangementFlag
      exportNametableFlag();
  static NesRom::MapperType
      exportMapperNum();
  
//  LaylaMetatileSet& caveMetatiles();
  
//  LaylaMetatileSet& bossMetatiles();
protected:
  const static int numExportPrgBanks_ = 16;
  const static int numExportChrBanks_ = 0;
  const static NesRom::NametableArrangementFlag
      exportNametableFlag_ = NesRom::nametablesHorizontal;
  const static NesRom::MapperType
      exportMapperNum_ = NesRom::mapperUxRom;
      
  // ******LTIM-SPECIFIC ADDITIONS*********
  const static int marioBgOffset_ = 0x9010;
  const static int marioSpritesOffset_ = 0x8010;
  // Bank D = new graphics
  const static int marioTablesExportDest_ = 0x34010;
  const static Tbyte* marioOverworldPalette;
  const static Tbyte* marioUnderworldPalette;
//  const static Tbyte* marioWaterPalette;
  const static Tbyte* marioCastlePalette;
  void ltimPostImportStep();
  void ltimPreExportStep(NesRom& rom);
  void ltimPostExportStep(NesRom& rom);
  bool tileIsBlank(const NesTile& tile);
  void addTiles(std::vector<NesTile> tiles,
                NesPatternTable& patterns,
                int srcPos,
                int srcLen,
                int position);
  NesTile graphicToTile(Graphic& src,
                        const NesPaletteQuad& palettes,
                        std::unordered_map<Tcolor, NesColor,
                                             TcolorHash> colorMap);
  Tbyte* readAndAllocateMarioFile(const Tstring& filename);
  NesPatternTable prepareMarioBg(Tbyte* mariofile);
  NesPatternTable prepareMarioSprites(Tbyte* mariofile);

  NesRom rom_;

  LaylaLevelSet levels_;
  
  LaylaPalettes palettes_;
  
  LaylaGraphics graphics_;
  
  LaylaStaticMetatiles staticMetatiles_;
  
  LaylaObjectSetCollection objectSets_;
  
  Tbyte titleBankOrig[UxRomBanking::sizeOfPrgBank];
  
};


};


#endif
