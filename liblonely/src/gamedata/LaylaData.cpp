#include "gamedata/LaylaData.h"
#include "gamedata/LaylaOffsetFile.h"
#include "gamedata/LaylaArea.h"
#include "gamedata/LaylaPatternDefinitionTable.h"
#include "nes/NesRom.h"
#include "nes/UxRomBanking.h"
#include "structs/Tstring.h"
#include "structs/AddressTableReader.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "util/StringConversion.h"
#include "util/ByteConversion.h"
#include "util/FileManip.h"
#include <fstream>
#include <iostream>

#include "structs/Graphic.h"
#include "structs/PngConversion.h"

namespace Lonely {


LaylaData::LaylaData(const Tstring& romFileName,
            const Tstring& offsetFileName)
  : rom_(romFileName) {
//  NesRom rom(romFileName);
            
  LaylaOffsetFile offsets(offsetFileName);
  
  levels_ = LaylaLevelSet(rom_,
                          offsets);
  
  palettes_ = LaylaPalettes(rom_,
                            offsets);
  
  graphics_ = LaylaGraphics(rom_,
                            offsets);
  
  staticMetatiles_ = LaylaStaticMetatiles(rom_,
                                          offsets);
  
  for (int i = 0; i < LaylaObjectSets::numObjectSets; i++) {
    int targetLevelNum = LaylaObjectSets::objectSetToLevelNum(i);
    LaylaLevel& targetLevel = levels_.level(targetLevelNum);
    
    objectSets_.push_back(
      LaylaObjectSet(
          targetLevel.objectSetNum(),
          targetLevel.bossInitTable(),
          targetLevel.objectInitTable(),
          targetLevel.objectCodeBlockBaseOffset(),
          targetLevel.objectCodeBlock()));
  }
  
  ltimPostImportStep();
/*  for (int i = 0; i < 8; i++) {
    Graphic g(NesColorData::size * 16,
               16);
    NesColorData col = palettes_.generateBossPalette(i);
    for (int j = 0; j < NesColorData::size; j++) {
      g.fillRect(j * 16, 0,
                 16, 16,
                 Tcolor(col.color(j).realColor()));
    }
    
    PngConversion::graphicToRGBAPng(
        Tstring("boss-")
        + StringConversion::toString(i)
        + ".png",
        g);
  } */
}
  
void LaylaData::exportToFile(const Tstring& romFileName) {
  // Create output ROM
  NesRom outrom(rom_);
  
  // Do LTIM-specific pre-export modifications
  ltimPreExportStep(outrom);
  
  // Export metatiles before expanding ROM (since they go in the fixed bank,
  // which will be copied to the end of the ROM during expansion)
  staticMetatiles_.exportToRom(outrom);
  
  // Ditto the palettes
  palettes_.exportToRom(outrom);
  
  // Expand to 256kb
  LaylaHacks::expandAndAdjustRom(outrom);
  
  // Write level data
  levels_.exportToRom(outrom, objectSets_);
  
  // Write graphics data
  graphics_.exportToRom(outrom);
  
  // Do LTIM-specific post-export modifications
  ltimPostExportStep(outrom);
  
  // Write to file
  outrom.exportToFile(romFileName,
                      numExportPrgBanks_,
                      numExportChrBanks_,
                      exportNametableFlag_,
                      exportMapperNum_);
}
  
int LaylaData::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LaylaData,
                   0);
  
  levels_.save(data);
  
  palettes_.save(data);
  
  graphics_.save(data);
  
  staticMetatiles_.save(data);
  
  SaveHelper::saveInt(data, objectSets_.size(),
                      ByteSizes::uint16Size);
  
  for (int i = 0; i < objectSets_.size(); i++) {
    objectSets_[i].save(data);
  }
  
  return saver.finalize();
}

int LaylaData::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);
                    
  objectSets_.clear();
                    
  byteCount += levels_.load(data + byteCount);
                    
  byteCount += palettes_.load(data + byteCount);
                    
  byteCount += graphics_.load(data + byteCount);
                    
  byteCount += staticMetatiles_.load(data + byteCount);
  
  int numObjectSets = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint16Size);
  
  for (int i = 0; i < numObjectSets; i++) {
    objectSets_.push_back(LaylaObjectSet());
    byteCount += objectSets_[i].load(data + byteCount);
  }
  
  return byteCount;
}
  
LaylaLevelSet& LaylaData::levels() {
  return levels_;
}

LaylaPalettes& LaylaData::palettes() {
  return palettes_;
}

LaylaGraphics& LaylaData::graphics() {
  return graphics_;
}
  
LaylaStaticMetatiles& LaylaData::staticMetatiles() {
  return staticMetatiles_;
}
  
int LaylaData::numObjectSets() const {
  return objectSets_.size();
}

LaylaObjectSet& LaylaData::objectSet(int index) {
  return objectSets_[index];
}

const LaylaObjectSet& LaylaData::objectSet(int index) const {
  return objectSets_[index];
}

const LaylaObjectSetCollection& LaylaData::objectSets() const {
  return objectSets_;
}
  
int LaylaData::numExportPrgBanks() {
  return numExportPrgBanks_;
}

int LaylaData::numExportChrBanks() {
  return numExportChrBanks_;
}

NesRom::NametableArrangementFlag
    LaylaData::exportNametableFlag() {
  return exportNametableFlag_;
}

NesRom::MapperType
    LaylaData::exportMapperNum() {
  return exportMapperNum_;
}
      
void LaylaData::ltimPostImportStep() {
  // TEMP: Substitute Mario sprite/background tables for base
/*  Tbyte* mariofile = readAndAllocateMarioFile("../mario.nes");
  NesPatternTable bg = prepareMarioBg(mariofile);
  NesPatternTable sprites = prepareMarioSprites(mariofile);
  delete mariofile;
  graphics_.baseBackground() = bg;
  graphics_.baseSprites() = sprites;
  
  // TEMP: Substitute Mario palettes
  palettes_.standardPalette().readFullPaletteSet(
    marioOverworldPalette); */
  
  // Replace keycard sprite graphics with "?" item BG graphics
  graphics_.bossSprites().tile(0xEE)
    = graphics_.bossBackground().tile(0x38);
  graphics_.bossSprites().tile(0xEF)
    = graphics_.bossBackground().tile(0x39);
  graphics_.bossSprites().tile(0xFE)
    = graphics_.bossBackground().tile(0x48);
  graphics_.bossSprites().tile(0xFF)
    = graphics_.bossBackground().tile(0x49);
}
  
void LaylaData::ltimPreExportStep(NesRom& rom) {
  
}

void LaylaData::ltimPostExportStep(NesRom& rom) {
  // Write Mario sprite/background tables to new location
  Tbyte* mariofile = readAndAllocateMarioFile("../mario.nes");
  NesPatternTable bg = prepareMarioBg(mariofile);
  NesPatternTable sprites = prepareMarioSprites(mariofile);
  delete mariofile;
  sprites.toUncompressedData(rom.directWrite(marioTablesExportDest_ + 0x0000));
  bg.toUncompressedData(rom.directWrite(marioTablesExportDest_
                            + 0x1000));
  
  
}
  
Tbyte* LaylaData::readAndAllocateMarioFile(const Tstring& filename) {
  std::ifstream ifs("../mario.nes",
                    std::ios_base::binary);
  int filesize = FileManip::getFileSize(ifs);
  Tbyte* mariofile = new Tbyte[filesize];
  ifs.read((char*)mariofile, filesize);
  return mariofile;
}

const Tbyte* LaylaData::marioOverworldPalette(
//    (Tbyte*)("\x22\x29\x1a\x0f\x22\x36\x17\x0f"
//             "\x22\x30\x21\x0f\x22\x27\x17\x0f"
//             "\x00\x16\x27\x18\x00\x1a\x30\x27"
//             "\x00\x16\x30\x27\x00\x0f\x36\x17"));
    (Tbyte*)("\x22\x29\x1a\x0f\x22\x36\x17\x0f"
             "\x22\x30\x21\x0f\x22\x27\x17\x0f"
             "\x00\x16\x27\x18\x00\x1a\x30\x27"
             "\x00\x16\x30\x27\x00\x0f\x36\x17"));

const Tbyte* LaylaData::marioUnderworldPalette(
    (Tbyte*)("\x0f\x29\x1a\x09\x0f\x3c\x1c\x0f"
             "\x0f\x30\x21\x1c\x0f\x27\x17\x1c"
             "\x00\x16\x27\x18\x00\x1c\x36\x17"
             "\x00\x16\x30\x27\x00\x0c\x3c\x1c"));

//const Tbyte* LaylaData::marioWaterPalette(
//    (Tbyte*)("\x22\x29\x1a\x0f\x22\x36\x17\x0f"
//             "\x22\x30\x21\x0f\x22\x27\x17\x0f"));

const Tbyte* LaylaData::marioCastlePalette(
    (Tbyte*)("\x0f\x30\x10\x00\x0f\x30\x10\x00"
             "\x0f\x30\x16\x00\x0f\x27\x17\x00"
             "\x00\x16\x27\x18\x00\x1c\x36\x17"
             "\x00\x16\x30\x27\x00\x00\x30\x10"));

NesPatternTable LaylaData::prepareMarioBg(Tbyte* mariofile) {
  NesPatternTable bg;
  bg.fromUncompressedData(mariofile + marioBgOffset_);
  
  // Mario rotates BG palette color 0x13, Layla rotates 0x15. Swap
  // colors 1 and 3 of tiles using palette 3 (that we care about)
  // to compensate.
  // ? block (regular and hit)
  for (int i = 0x53; i < 0x5B; i++) {
    bg.tile(i).swapColors(1, 3);
  }
  // Axe
  for (int i = 0x7B; i < 0x7F; i++) {
    bg.tile(i).swapColors(1, 3);
  }
  
  // palette 1 = 22 0F 17 36 for HUD
/*  for (int i = 0; i < 0x100; i++) {
    switch (i) {
//    case 0x75:
//    case 0x76:
    case 0x9B:
    case 0x9C:
    case 0x9D:
    case 0x9E:
    case 0xA9:
    case 0xAA:
    case 0xAB:
    case 0xAC:
    case 0xAD:
    case 0xAE:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB7:
      bg.tile(i).swapColors(1, 3);
      break;
    default:
      break;
    }
  } */
  
  NesPatternTable composite = graphics_.baseBackground();
  
  // Swap colors in the HUD graphics to better work with the Mario palette
  for (int i = 0x36; i < 0x3A; i++) {
    composite.tile(i).swapColors(1, 3);
  }
  for (int i = 0x46; i < 0x4A; i++) {
    composite.tile(i).swapColors(1, 3);
  }
  for (int i = 0xB6; i < 0xB7; i++) {
    composite.tile(i).swapColors(1, 3);
  }
  for (int i = 0xC6; i < 0xC8; i++) {
    composite.tile(i).swapColors(1, 3);
  }
  for (int i = 0xDC; i < 0x100; i++) {
    composite.tile(i).swapColors(1, 3);
  }
  
  // Copy needed Mario tiles to Layla table
  int putpos = 0x29;
  for (int i = 0x0; i < 0x100; i++) {
    switch (i) {
    // bg colors
    case 0x24: case 0x25: case 0x26: case 0x27:
    // other
    case 0xB4: case 0xB5: case 0xB6: case 0xB7:
      composite.tile(putpos++) = bg.tile(i);
      break;
    default:
      break;
    }
  }
  putpos = 0x3C;
  for (int i = 0; i < 0x100; i++) {
    switch (i) {
    case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE:
      composite.tile(putpos++) = bg.tile(i);
      break;
    default:
      break;
    }
  }
  putpos = 0x4C;
  for (int i = 0x2F; i < 0xA0; i++) {
    if ((i >= 0x6B) && (i < 0x75)) {
      continue;
    }
    
    composite.tile(putpos++) = bg.tile(i);
  }
  
  return composite;
}

NesPatternTable LaylaData::prepareMarioSprites(Tbyte* mariofile) {
  NesPatternTable sprites;
  sprites.fromUncompressedData(mariofile + marioSpritesOffset_);
  return sprites;
}


};
