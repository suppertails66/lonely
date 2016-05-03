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
  Tbyte* mariofile = readAndAllocateMarioFile("../mario.nes");
  NesPatternTable bg = prepareMarioBg(mariofile);
  NesPatternTable sprites = prepareMarioSprites(mariofile);
  delete mariofile;
  graphics_.baseBackground() = bg;
  graphics_.baseSprites() = sprites;
  
  
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

NesPatternTable LaylaData::prepareMarioBg(Tbyte* mariofile) {
  NesPatternTable bg;
  bg.fromUncompressedData(mariofile + marioBgOffset_);
  return bg;
}

NesPatternTable LaylaData::prepareMarioSprites(Tbyte* mariofile) {
  NesPatternTable sprites;
  sprites.fromUncompressedData(mariofile + marioSpritesOffset_);
  return sprites;
}


};
