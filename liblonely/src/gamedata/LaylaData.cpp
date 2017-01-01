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
#include "structs/PngConversion.h"
#include "util/StringConversion.h"
#include "util/ByteConversion.h"
#include "util/FileManip.h"
#include "exception/TGenericException.h"
#include <fstream>
#include <iostream>

#include "structs/Graphic.h"
#include "structs/PngConversion.h"

namespace Lonely {


std::string stringEncode(const std::string& str) {
  
  std::string output;

  for (int i = 0; i < str.size(); i++) {
    if ((str[i] >= 0x30) && (str[i] <= 0x39)) {
      output += (str[i] - 0x2F);
    }
    else if ((str[i] >= 0x41) && (str[i] <= 0x5A)) {
      output += (str[i] - 0x36);
    }
    else if (str[i] == ' ') {
      output += (char)0x00;
    }
    else if (str[i] == '-') {
      output += (char)0x25;
    }
    else if (str[i] == 'd') {
      output += (char)0x26;
    }
    else if (str[i] == '.') {
      output += (char)0x27;
    }
    else if (str[i] == '@') {
      output += (char)0x28;
    }
    else if (str[i] == ',') {
      output += (char)0x29;
    }
    else if (str[i] == '\'') {
      output += (char)0x2A;
    }
    else if (str[i] == '!') {
      output += (char)0x2B;
    }
    else if (str[i] == '?') {
      output += (char)0x2C;
    }
    else if (str[i] == '(') {
      output += (char)0x2D;
    }
    else if (str[i] == ')') {
      output += (char)0x2E;
    }
    else {
      output += str[i];
    }
  }
  
  return output;
}

void buildStringTable(std::string& table,
                      std::vector<std::string> strings,
                      int baseAddress) {
  char buffer[2];
  
  // for each string, fill in corresponding slot in duplicates with -1
  // if not a duplicate or the index of the duplicated string if it is
  std::vector<int> duplicates;
  for (int i = 0; i < strings.size(); i++) {
    bool found = false;
    for (int j = 0; j < i; j++) {
      if (strings[i].compare(strings[j]) == 0) {
        duplicates.push_back(j);
        found = true;
        break;
      }
    }
    
    if (!found) {
      duplicates.push_back(-1);
    }
  }
  
  // Write address table
  std::vector<int> addresses;
  int address = baseAddress + (strings.size() * 2);
  for (int i = 0; i < strings.size(); i++) {
    // if a duplicate, fill in address of duplicated line
    if (duplicates[i] != -1) {
      int dupaddr = addresses[duplicates[i]];
      ByteConversion::toBytes(dupaddr,
                              buffer,
                              2,
                              EndiannessTypes::little,
                              SignednessTypes::nosign);
      table += std::string(buffer, 2);
      addresses.push_back(dupaddr);
    }
    else {
      ByteConversion::toBytes(address,
                              buffer,
                              2,
                              EndiannessTypes::little,
                              SignednessTypes::nosign);
                              
      table += std::string(buffer, 2);
      
      addresses.push_back(address);
      
      address += (strings[i].size());
    }
  }
  
  // Write strings
  for (int i = 0; i < strings.size(); i++) {
    // skip duplicate entries
    if (duplicates[i] == -1) {
      table += strings[i];
    }
  }
}

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
//  graphics_.bossSprites().tile(0xEE)
//    = graphics_.bossBackground().tile(0x38);
//  graphics_.bossSprites().tile(0xEF)
//    = graphics_.bossBackground().tile(0x39);
//  graphics_.bossSprites().tile(0xFE)
//    = graphics_.bossBackground().tile(0x48);
//  graphics_.bossSprites().tile(0xFF)
//    = graphics_.bossBackground().tile(0x49);

  // Copy title bank (needed for ending)
  std::memcpy(titleBankOrig,
              rom_.directRead(0x8000),
              UxRomBanking::sizeOfPrgBank);
              
  // Thanks for not putting a question mark in the font, dB-soft!
  NesTile qMark;
  // Row 0
  qMark.setData(0, 0, 0);
  qMark.setData(1, 0, 0);
  qMark.setData(2, 0, 1);
  qMark.setData(3, 0, 1);
  qMark.setData(4, 0, 1);
  qMark.setData(5, 0, 0);
  qMark.setData(6, 0, 0);
  qMark.setData(7, 0, 0);
  // Row 1
  qMark.setData(0, 1, 0);
  qMark.setData(1, 1, 1);
  qMark.setData(2, 1, 1);
  qMark.setData(3, 1, 0);
  qMark.setData(4, 1, 1);
  qMark.setData(5, 1, 1);
  qMark.setData(6, 1, 0);
  qMark.setData(7, 1, 0);
  // Row 2
  qMark.setData(0, 2, 0);
  qMark.setData(1, 2, 0);
  qMark.setData(2, 2, 0);
  qMark.setData(3, 2, 0);
  qMark.setData(4, 2, 1);
  qMark.setData(5, 2, 1);
  qMark.setData(6, 2, 0);
  qMark.setData(7, 2, 0);
  // Row 3
  qMark.setData(0, 3, 0);
  qMark.setData(1, 3, 0);
  qMark.setData(2, 3, 0);
  qMark.setData(3, 3, 1);
  qMark.setData(4, 3, 1);
  qMark.setData(5, 3, 0);
  qMark.setData(6, 3, 0);
  qMark.setData(7, 3, 0);
  // Row 4
  qMark.setData(0, 4, 0);
  qMark.setData(1, 4, 0);
  qMark.setData(2, 4, 0);
  qMark.setData(3, 4, 0);
  qMark.setData(4, 4, 0);
  qMark.setData(5, 4, 0);
  qMark.setData(6, 4, 0);
  qMark.setData(7, 4, 0);
  // Row 5
  qMark.setData(0, 5, 0);
  qMark.setData(1, 5, 1);
  qMark.setData(2, 5, 1);
  qMark.setData(3, 5, 0);
  qMark.setData(4, 5, 0);
  qMark.setData(5, 5, 0);
  qMark.setData(6, 5, 0);
  qMark.setData(7, 5, 0);
  // Row 6
  qMark.setData(0, 6, 0);
  qMark.setData(1, 6, 1);
  qMark.setData(2, 6, 1);
  qMark.setData(3, 6, 0);
  qMark.setData(4, 6, 0);
  qMark.setData(5, 6, 0);
  qMark.setData(6, 6, 0);
  qMark.setData(7, 6, 0);
  // Row 7
  qMark.setData(0, 7, 0);
  qMark.setData(1, 7, 0);
  qMark.setData(2, 7, 0);
  qMark.setData(3, 7, 0);
  qMark.setData(4, 7, 0);
  qMark.setData(5, 7, 0);
  qMark.setData(6, 7, 0);
  qMark.setData(7, 7, 0);
  graphics_.baseBackground().tile(0xB4) = qMark;
  graphics_.titleBackground().tile(0x2C) = qMark;
  
  // parentheses
  // open
  for (int j = 0; j < qMark.height; j++) {
    for (int i = 0; i < qMark.height; i++) {
      qMark.setData(i, j, 0);
    }
  }
  // Row 0
  qMark.setData(4, 0, 1);
  qMark.setData(5, 0, 1);
  // Row 1
  qMark.setData(3, 1, 1);
  qMark.setData(4, 1, 1);
  // Row 2
  qMark.setData(2, 2, 1);
  qMark.setData(3, 2, 1);
  // Row 3
  qMark.setData(2, 3, 1);
  qMark.setData(3, 3, 1);
  // Row 4
  qMark.setData(2, 4, 1);
  qMark.setData(3, 4, 1);
  // Row 5
  qMark.setData(2, 5, 1);
  qMark.setData(3, 5, 1);
  // Row 6
  qMark.setData(3, 6, 1);
  qMark.setData(4, 6, 1);
  // Row 7
  qMark.setData(4, 7, 1);
  qMark.setData(5, 7, 1);
  graphics_.titleBackground().tile(0x2D) = qMark;
  // closed
  for (int j = 0; j < qMark.height; j++) {
    for (int i = 0; i < qMark.height; i++) {
      qMark.setData(i, j, 0);
    }
  }
  // Row 0
  qMark.setData(1, 0, 1);
  qMark.setData(2, 0, 1);
  // Row 1
  qMark.setData(2, 1, 1);
  qMark.setData(3, 1, 1);
  // Row 2
  qMark.setData(3, 2, 1);
  qMark.setData(4, 2, 1);
  // Row 3
  qMark.setData(3, 3, 1);
  qMark.setData(4, 3, 1);
  // Row 4
  qMark.setData(3, 4, 1);
  qMark.setData(4, 4, 1);
  // Row 5
  qMark.setData(3, 5, 1);
  qMark.setData(4, 5, 1);
  // Row 6
  qMark.setData(2, 6, 1);
  qMark.setData(3, 6, 1);
  // Row 7
  qMark.setData(1, 7, 1);
  qMark.setData(2, 7, 1);
  graphics_.titleBackground().tile(0x2E) = qMark;
  
  // make copy of old question mark power-up (for mission 9)
/*  graphics_.baseBackground().tile(0x29)
    = graphics_.baseBackground().tile(0x38);
  graphics_.baseBackground().tile(0x96)
    = graphics_.baseBackground().tile(0x48);
  graphics_.baseBackground().tile(0x9B)
    = graphics_.baseBackground().tile(0x39);
  graphics_.baseBackground().tile(0xB1)
    = graphics_.baseBackground().tile(0x49); */
    
  // the ROM is pre-hacked to include period, comma, etc. at D0-D3 --
  // move them to different spaces
  graphics_.baseBackground().tile(0x29)
    = graphics_.baseBackground().tile(0xD0);
  graphics_.baseBackground().tile(0x96)
    = graphics_.baseBackground().tile(0xD1);
  graphics_.baseBackground().tile(0x9B)
    = graphics_.baseBackground().tile(0xD2);
  graphics_.baseBackground().tile(0xB1)
    = graphics_.baseBackground().tile(0xD3);
  
    
  // add heart graphics (replacing D0-D3)
  for (int j = 0; j < qMark.height; j++) {
    for (int i = 0; i < qMark.height; i++) {
      qMark.setData(i, j, 0);
    }
  }
  // upper-left
  // Row 0
  qMark.setData(1, 0, 1);
  qMark.setData(2, 0, 1);
  qMark.setData(3, 0, 1);
  qMark.setData(4, 0, 1);
  qMark.setData(5, 0, 1);
  qMark.setData(6, 0, 1);
  qMark.setData(7, 0, 1);
  // Row 1
  qMark.setData(0, 1, 2);
  qMark.setData(1, 1, 2);
  qMark.setData(2, 1, 2);
  qMark.setData(3, 1, 2);
  qMark.setData(4, 1, 2);
  qMark.setData(5, 1, 2);
  qMark.setData(6, 1, 2);
  qMark.setData(7, 1, 2);
  // Row 2
  qMark.setData(0, 2, 2);
  // Row 3
  qMark.setData(0, 3, 2);
  qMark.setData(4, 3, 1);
  qMark.setData(5, 3, 1);
  // Row 4
  qMark.setData(0, 4, 2);
  qMark.setData(3, 4, 1);
  qMark.setData(4, 4, 3);
  qMark.setData(5, 4, 3);
  qMark.setData(6, 4, 1);
  // Row 5
  qMark.setData(0, 5, 2);
  qMark.setData(2, 5, 1);
  qMark.setData(3, 5, 3);
  qMark.setData(4, 5, 1);
  qMark.setData(5, 5, 1);
  qMark.setData(6, 5, 3);
  qMark.setData(7, 5, 1);
  // Row 6
  qMark.setData(0, 6, 2);
  qMark.setData(2, 6, 1);
  qMark.setData(3, 6, 3);
  qMark.setData(4, 6, 1);
  qMark.setData(5, 6, 3);
  qMark.setData(6, 6, 3);
  qMark.setData(7, 6, 3);
  // Row 7
  qMark.setData(0, 7, 2);
  qMark.setData(2, 7, 1);
  qMark.setData(3, 7, 3);
  qMark.setData(4, 7, 3);
  qMark.setData(5, 7, 3);
  qMark.setData(6, 7, 3);
  qMark.setData(7, 7, 3);
  graphics_.caveBackground().tile(0xD0) = qMark;
  graphics_.baseBackground().tile(0xD0) = qMark;
  graphics_.bossBackground().tile(0xD0) = qMark;
  
  // upper-right
  qMark.setData(0, 0, 1);
  qMark.setData(1, 0, 1);
  qMark.setData(2, 0, 1);
  qMark.setData(3, 0, 1);
  qMark.setData(4, 0, 1);
  qMark.setData(5, 0, 1);
  qMark.setData(6, 0, 1);
  qMark.setData(7, 0, 0);
  // 
  qMark.setData(0, 1, 2);
  qMark.setData(1, 1, 2);
  qMark.setData(2, 1, 2);
  qMark.setData(3, 1, 2);
  qMark.setData(4, 1, 2);
  qMark.setData(5, 1, 2);
  qMark.setData(6, 1, 2);
  qMark.setData(7, 1, 1);
  // 
  qMark.setData(0, 2, 0);
  qMark.setData(1, 2, 0);
  qMark.setData(2, 2, 0);
  qMark.setData(3, 2, 0);
  qMark.setData(4, 2, 0);
  qMark.setData(5, 2, 0);
  qMark.setData(6, 2, 2);
  qMark.setData(7, 2, 1);
  // 
  qMark.setData(0, 3, 0);
  qMark.setData(1, 3, 1);
  qMark.setData(2, 3, 1);
  qMark.setData(3, 3, 0);
  qMark.setData(4, 3, 0);
  qMark.setData(5, 3, 0);
  qMark.setData(6, 3, 2);
  qMark.setData(7, 3, 1);
  // 
  qMark.setData(0, 4, 1);
  qMark.setData(1, 4, 3);
  qMark.setData(2, 4, 3);
  qMark.setData(3, 4, 1);
  qMark.setData(4, 4, 0);
  qMark.setData(5, 4, 0);
  qMark.setData(6, 4, 2);
  qMark.setData(7, 4, 1);
  // 
  qMark.setData(0, 5, 3);
  qMark.setData(1, 5, 3);
  qMark.setData(2, 5, 3);
  qMark.setData(3, 5, 3);
  qMark.setData(4, 5, 1);
  qMark.setData(5, 5, 0);
  qMark.setData(6, 5, 2);
  qMark.setData(7, 5, 1);
  // 
  qMark.setData(0, 6, 3);
  qMark.setData(1, 6, 3);
  qMark.setData(2, 6, 3);
  qMark.setData(3, 6, 3);
  qMark.setData(4, 6, 1);
  qMark.setData(5, 6, 0);
  qMark.setData(6, 6, 2);
  qMark.setData(7, 6, 1);
  // 
  qMark.setData(0, 7, 3);
  qMark.setData(1, 7, 3);
  qMark.setData(2, 7, 3);
  qMark.setData(3, 7, 3);
  qMark.setData(4, 7, 1);
  qMark.setData(5, 7, 0);
  qMark.setData(6, 7, 2);
  qMark.setData(7, 7, 1);
  graphics_.caveBackground().tile(0xD1) = qMark;
  graphics_.baseBackground().tile(0xD1) = qMark;
  graphics_.bossBackground().tile(0xD1) = qMark;
  
  // lower-left
  qMark.setData(0, 0, 2);
  qMark.setData(1, 0, 0);
  qMark.setData(2, 0, 1);
  qMark.setData(3, 0, 3);
  qMark.setData(4, 0, 3);
  qMark.setData(5, 0, 3);
  qMark.setData(6, 0, 3);
  qMark.setData(7, 0, 3);
  // 
  qMark.setData(0, 1, 2);
  qMark.setData(1, 1, 0);
  qMark.setData(2, 1, 0);
  qMark.setData(3, 1, 1);
  qMark.setData(4, 1, 3);
  qMark.setData(5, 1, 3);
  qMark.setData(6, 1, 3);
  qMark.setData(7, 1, 3);
  // 
  qMark.setData(0, 2, 2);
  qMark.setData(1, 2, 0);
  qMark.setData(2, 2, 0);
  qMark.setData(3, 2, 0);
  qMark.setData(4, 2, 1);
  qMark.setData(5, 2, 3);
  qMark.setData(6, 2, 3);
  qMark.setData(7, 2, 3);
  // 
  qMark.setData(0, 3, 2);
  qMark.setData(1, 3, 0);
  qMark.setData(2, 3, 0);
  qMark.setData(3, 3, 0);
  qMark.setData(4, 3, 0);
  qMark.setData(5, 3, 1);
  qMark.setData(6, 3, 3);
  qMark.setData(7, 3, 3);
  // 
  qMark.setData(0, 4, 2);
  qMark.setData(1, 4, 0);
  qMark.setData(2, 4, 0);
  qMark.setData(3, 4, 0);
  qMark.setData(4, 4, 0);
  qMark.setData(5, 4, 0);
  qMark.setData(6, 4, 1);
  qMark.setData(7, 4, 3);
  // 
  qMark.setData(0, 5, 2);
  qMark.setData(1, 5, 0);
  qMark.setData(2, 5, 0);
  qMark.setData(3, 5, 0);
  qMark.setData(4, 5, 0);
  qMark.setData(5, 5, 0);
  qMark.setData(6, 5, 0);
  qMark.setData(7, 5, 1);
  // 
  qMark.setData(0, 6, 2);
  qMark.setData(1, 6, 0);
  qMark.setData(2, 6, 0);
  qMark.setData(3, 6, 0);
  qMark.setData(4, 6, 0);
  qMark.setData(5, 6, 0);
  qMark.setData(6, 6, 0);
  qMark.setData(7, 6, 0);
  // 
  qMark.setData(0, 7, 0);
  qMark.setData(1, 7, 2);
  qMark.setData(2, 7, 2);
  qMark.setData(3, 7, 2);
  qMark.setData(4, 7, 2);
  qMark.setData(5, 7, 2);
  qMark.setData(6, 7, 2);
  qMark.setData(7, 7, 2);
  graphics_.caveBackground().tile(0xD2) = qMark;
  graphics_.baseBackground().tile(0xD2) = qMark;
  graphics_.bossBackground().tile(0xD2) = qMark;
  
  // lower-right
  qMark.setData(0, 0, 3);
  qMark.setData(1, 0, 3);
  qMark.setData(2, 0, 3);
  qMark.setData(3, 0, 3);
  qMark.setData(4, 0, 1);
  qMark.setData(5, 0, 0);
  qMark.setData(6, 0, 2);
  qMark.setData(7, 0, 1);
  // 
  qMark.setData(0, 1, 3);
  qMark.setData(1, 1, 3);
  qMark.setData(2, 1, 3);
  qMark.setData(3, 1, 1);
  qMark.setData(4, 1, 0);
  qMark.setData(5, 1, 0);
  qMark.setData(6, 1, 2);
  qMark.setData(7, 1, 1);
  // 
  qMark.setData(0, 2, 3);
  qMark.setData(1, 2, 3);
  qMark.setData(2, 2, 1);
  qMark.setData(3, 2, 0);
  qMark.setData(4, 2, 0);
  qMark.setData(5, 2, 0);
  qMark.setData(6, 2, 2);
  qMark.setData(7, 2, 1);
  // 
  qMark.setData(0, 3, 3);
  qMark.setData(1, 3, 1);
  qMark.setData(2, 3, 0);
  qMark.setData(3, 3, 0);
  qMark.setData(4, 3, 0);
  qMark.setData(5, 3, 0);
  qMark.setData(6, 3, 2);
  qMark.setData(7, 3, 1);
  // 
  qMark.setData(0, 4, 1);
  qMark.setData(1, 4, 0);
  qMark.setData(2, 4, 0);
  qMark.setData(3, 4, 0);
  qMark.setData(4, 4, 0);
  qMark.setData(5, 4, 0);
  qMark.setData(6, 4, 2);
  qMark.setData(7, 4, 1);
  // 
  qMark.setData(0, 5, 0);
  qMark.setData(1, 5, 0);
  qMark.setData(2, 5, 0);
  qMark.setData(3, 5, 0);
  qMark.setData(4, 5, 0);
  qMark.setData(5, 5, 0);
  qMark.setData(6, 5, 2);
  qMark.setData(7, 5, 1);
  // 
  qMark.setData(0, 6, 0);
  qMark.setData(1, 6, 0);
  qMark.setData(2, 6, 0);
  qMark.setData(3, 6, 0);
  qMark.setData(4, 6, 0);
  qMark.setData(5, 6, 0);
  qMark.setData(6, 6, 2);
  qMark.setData(7, 6, 1);
  // 
  qMark.setData(0, 7, 2);
  qMark.setData(1, 7, 2);
  qMark.setData(2, 7, 2);
  qMark.setData(3, 7, 2);
  qMark.setData(4, 7, 2);
  qMark.setData(5, 7, 2);
  qMark.setData(6, 7, 2);
  qMark.setData(7, 7, 0);
  graphics_.caveBackground().tile(0xD3) = qMark;
  graphics_.baseBackground().tile(0xD3) = qMark;
  graphics_.bossBackground().tile(0xD3) = qMark;
    
  // create grid tiles for mission 9
  for (int j = 0; j < qMark.height; j++) {
    for (int i = 0; i < qMark.height; i++) {
      qMark.setData(i, j, 0);
    }
  }
  qMark.setData(0, 0, 1);
  qMark.setData(0, 1, 1);
  qMark.setData(0, 2, 1);
  qMark.setData(0, 3, 1);
  qMark.setData(0, 4, 1);
  qMark.setData(0, 5, 1);
  qMark.setData(0, 6, 1);
  qMark.setData(0, 7, 1);
  graphics_.baseBackground().tile(0x90) = qMark;
  for (int j = 0; j < qMark.height; j++) {
    for (int i = 0; i < qMark.height; i++) {
      qMark.setData(i, j, 0);
    }
  }
  qMark.setData(0, 7, 1);
  qMark.setData(1, 7, 1);
  qMark.setData(2, 7, 1);
  qMark.setData(3, 7, 1);
  qMark.setData(4, 7, 1);
  qMark.setData(5, 7, 1);
  qMark.setData(6, 7, 1);
  qMark.setData(7, 7, 1);
  graphics_.baseBackground().tile(0x93) = qMark;
  for (int j = 0; j < qMark.height; j++) {
    for (int i = 0; i < qMark.height; i++) {
      qMark.setData(i, j, 0);
    }
  }
  qMark.setData(0, 0, 1);
  qMark.setData(0, 1, 1);
  qMark.setData(0, 2, 1);
  qMark.setData(0, 3, 1);
  qMark.setData(0, 4, 1);
  qMark.setData(0, 5, 1);
  qMark.setData(0, 6, 1);
  qMark.setData(0, 7, 1);
  qMark.setData(0, 7, 1);
  qMark.setData(1, 7, 1);
  qMark.setData(2, 7, 1);
  qMark.setData(3, 7, 1);
  qMark.setData(4, 7, 1);
  qMark.setData(5, 7, 1);
  qMark.setData(6, 7, 1);
  qMark.setData(7, 7, 1);
  graphics_.baseBackground().tile(0xC1) = qMark;
  
//  0x7B
  
}

void LaylaData::addTiles(std::vector<NesTile> tiles,
              NesPatternTable& patterns,
              int srcPos,
              int srcLen,
              int position) {
  int total = 0;
  for (int i = srcPos; i < srcPos + srcLen; i++) {
    patterns.tile(position + (total++)) = tiles[i];
  }
}
  
void LaylaData::ltimPreExportStep(NesRom& rom) {
  
  Graphic titleIris;
  Graphic titleBorder;
  Graphic titleLogo;
  bool pngLoaded = PngConversion::RGBAPngToGraphic("title_iris.png",
                                  titleIris);
  bool pngLoaded2 = PngConversion::RGBAPngToGraphic("title_border.png",
                                  titleBorder);
  bool pngLoaded3 = PngConversion::RGBAPngToGraphic("title_logo.png",
                                  titleLogo);

  std::unordered_map<Tcolor, NesColor, TcolorHash> colorMapTitleIris;
  colorMapTitleIris[Tcolor(0x00, 0x00, 0x00)] = NesColor(0x0F);
  colorMapTitleIris[Tcolor(0xFC, 0xFC, 0xFC)] = NesColor(0x30);
  colorMapTitleIris[Tcolor(0x20, 0x38, 0xEC)] = NesColor(0x12);
  colorMapTitleIris[Tcolor(0xFC, 0xBC, 0xB0)] = NesColor(0x36);
  colorMapTitleIris[Tcolor(0x00, 0x3C, 0x14)] = NesColor(0x0B);
  colorMapTitleIris[Tcolor(0x00, 0x94, 0x00)] = NesColor(0x19);
  colorMapTitleIris[Tcolor(0xE4, 0x00, 0x58)] = NesColor(0x15);

  std::unordered_map<Tcolor, NesColor, TcolorHash> colorMapLogo;
  colorMapLogo[Tcolor(0x00, 0x00, 0x00)] = NesColor(0x0F);
  colorMapLogo[Tcolor(0x3C, 0xBC, 0xFC)] = NesColor(0x21);
  colorMapLogo[Tcolor(0xE4, 0x00, 0x58)] = NesColor(0x15);
  colorMapLogo[Tcolor(0xFC, 0xFC, 0xFC)] = NesColor(0x30);
  
  NesPaletteQuad palettes;
  palettes.readFromData((unsigned char*)
    "\x0F\x30\x12\x36\x0F\x0B\x19\x36"
//    "\x0F\x21\x30\x15\x0F\x0B\x15\x36");
    // blank out palette 2 -- it's reserved for the logo and
    // can cause attribute map conflicts if present
    "\x1F\x1F\x1F\x1F\x0F\x0B\x15\x36");
  
  NesPaletteQuad palettes2;
  // blank unneeded palettes
  palettes2.readFromData((unsigned char*)
    "\x1F\x1F\x1F\x1F\x1F\x1F\x1F\x1F"
    "\x0F\x21\x30\x15\x1F\x1F\x1F\x1F");
                                  
  if (pngLoaded) {
    
    std::vector<NesTile> irisTitleTiles;
  
    int tilesW = titleIris.w() / NesTile::width;
    int tilesH = titleIris.h() / NesTile::height;
    
    for (int j = 0; j < tilesH; j++) {
      for (int i = 0; i < tilesW; i++) {
        try {
          Graphic tileGraphic(NesTile::width, NesTile::height);
          tileGraphic.copy(titleIris,
                           Box(0,
                               0,
                               0,
                               0),
                           Box(i * NesTile::width,
                               j * NesTile::height,
                               NesTile::width,
                               NesTile::height));
          std::unordered_map<Tcolor, NesColor, TcolorHash>& colorMap
            = colorMapTitleIris;
          NesTile tile = graphicToTile(tileGraphic,
                                       palettes,
                                       colorMap);
          
          if (!(tileIsBlank(tile))) {
            irisTitleTiles.push_back(tile);
            
/*            Graphic g;
            tile.drawGrayscale(g);
            PngConversion::graphicToRGBAPng(
              StringConversion::toString(j)
                + "-"
                + StringConversion::toString(i)
                + ".png",
              g); */
          }
        }
        catch (TGenericException& e) {
          std::cerr << "Error converting image to tile at tile pos ("
               << i << ", " << j << "): "
               << e.what() << std::endl;
        }
      }
    }
    
    // ROW 0
    
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             0,
             5,
             0x5B);
             
    // ROW 1
    
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             5,
             5,
             0x6B);
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             10,
             1,
             0x7B);
             
    // ROW 2
    
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             11,
             4,
             0x7C);
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             15,
             2,
             0x8B);
             
    // ROW 3
    
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             17,
             3,
             0x8D);
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             20,
             4,
             0x9B);
             
    // ROW 4
    
    // NO, this is used for the blinking cursor on password/ending
/*    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             24,
             1,
             0x9F); */
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             24,
             1,
             0xAA);
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             25,
             5,
             0xAB);
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             30,
             1,
             0xBB);
             
    // ROW 5
    
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             31,
             4,
             0xBC);
    addTiles(irisTitleTiles,
             graphics_.titleBackground(),
             35,
             3,
             0xCB);
    
    
  }
                                  
  if (pngLoaded2) {
    
    std::vector<NesTile> borderTitleTiles;
  
    int tilesW = titleBorder.w() / NesTile::width;
    int tilesH = titleBorder.h() / NesTile::height;
    
    for (int j = 0; j < tilesH; j++) {
      for (int i = 0; i < tilesW; i++) {
        try {
          Graphic tileGraphic(NesTile::width, NesTile::height);
          tileGraphic.copy(titleBorder,
                           Box(0,
                               0,
                               0,
                               0),
                           Box(i * NesTile::width,
                               j * NesTile::height,
                               NesTile::width,
                               NesTile::height));
          std::unordered_map<Tcolor, NesColor, TcolorHash>& colorMap
            = colorMapTitleIris;
          NesTile tile = graphicToTile(tileGraphic,
                                       palettes,
                                       colorMap);
          
          if (!(tileIsBlank(tile))) {
            borderTitleTiles.push_back(tile);
          }
        }
        catch (TGenericException& e) {
          std::cerr << "Error converting image to tile at tile pos ("
               << i << ", " << j << "): "
               << e.what() << std::endl;
        }
      }
    }
    
    addTiles(borderTitleTiles,
             graphics_.titleBackground(),
             0,
             3,
             0x54);
    addTiles(borderTitleTiles,
             graphics_.titleBackground(),
             3,
             1,
             0x64);
    addTiles(borderTitleTiles,
             graphics_.titleBackground(),
             4,
             2,
             0x65);
    addTiles(borderTitleTiles,
             graphics_.titleBackground(),
             6,
             2,
             0x74);
    
  }
                                  
  if (pngLoaded3) {
    std::vector<NesTile> tiles;
  
    int tilesW = titleLogo.w() / NesTile::width;
    int tilesH = titleLogo.h() / NesTile::height;
    
    for (int j = 0; j < tilesH; j++) {
      for (int i = 0; i < tilesW; i++) {
        try {
          Graphic tileGraphic(NesTile::width, NesTile::height);
          tileGraphic.copy(titleLogo,
                           Box(0,
                               0,
                               0,
                               0),
                           Box(i * NesTile::width,
                               j * NesTile::height,
                               NesTile::width,
                               NesTile::height));
          std::unordered_map<Tcolor, NesColor, TcolorHash>& colorMap
            = colorMapLogo;
          
          NesTile tile = graphicToTile(tileGraphic,
                                       palettes2,
                                       colorMap);
          
          if (!(tileIsBlank(tile))) {
            tiles.push_back(tile);
            
/*            Graphic g;
            tile.drawGrayscale(g);
            PngConversion::graphicToRGBAPng(
              StringConversion::toString(j)
                + "-"
                + StringConversion::toString(i)
                + ".png",
              g); */
          }
        }
        catch (TGenericException& e) {
          std::cerr << "Error converting image to tile at tile pos ("
               << i << ", " << j << "): "
               << e.what() << std::endl;
        }
      }
    }
    
    int total = 0;
    for (int i = 0; i < 0x0D; i++) {
      addTiles(tiles,
               graphics_.titleBackground(),
               total,
               4,
               0x30 + (i * 0x10));
      total += 4;
    }
    
    // total should be 52
    
    for (int i = 0x00; i < 0x03; i++) {
      addTiles(tiles,
               graphics_.titleBackground(),
               total,
               2,
               0x59 + (i * 0x10));
      total += 2;
    }
    
    // total should be 58
    
    // tile DF is used in level intermission
    addTiles(tiles,
             graphics_.titleBackground(),
             total,
             5,
             0xDA);
    total += 5;
    
    for (int i = 0x00; i < 0x02; i++) {
      addTiles(tiles,
               graphics_.titleBackground(),
               total,
               6,
               0xEA + (i * 0x10));
      total += 6;
    }
    
    // total should be 75
    
    addTiles(tiles,
             graphics_.titleBackground(),
             total,
             1,
             0xC4);
    total += 1;
    
    addTiles(tiles,
             graphics_.titleBackground(),
             total,
             1,
             0xC5);
    total += 1;
             
    // total should be 77
    
  }
  
  // border left will go at 0x54, 55, 56, 64
  // border right will go at 0x65, 66, 74, 75
  
  // update title tilemap
  std::memcpy(rom.directWrite(0xA75B),
              "\x54\x55\x56\x64"
                "\x00\x5B\x5C\x5D\x5E\x5F\x00\x00"
              "\x65\x66\x74\x75"
              "\x54\x55\x56\x64"
                "\x00\x6B\x6C\x6D\x6E\x6F\x7B\x00"
              "\x65\x66\x74\x75"
              "\x54\x55\x56\x64"
                "\x00\x7C\x7D\x7E\x7F\x8B\x8C\x00"
              "\x65\x66\x74\x75"
              "\x54\x55\x56\x64"
                "\x8D\x8E\x8F\x9B\x9C\x9D\x9E\x00"
              "\x65\x66\x74\x75"
              "\x54\x55\x56\x64"
                "\xAA\xAB\xAC\xAD\xAE\xAF\xBB\x00"
              "\x65\x66\x74\x75"
              "\x54\x55\x56\x64"
                "\xBC\xBD\xBE\xBF\xCB\xCC\xCD\x00"
              "\x65\x66\x74\x75",
              96);
  
  // update title attribute map
  std::memcpy(rom.directWrite(0xA7BD),
              "\x00\x05\x55\x00\x00\x0C\x07\x00",
              8);
  
  // update title logo tilemap
  std::memcpy(rom.directWrite(0xA6A5),
              // row 0
              "\x00\x00\x00\x00\x00\x00\x00\x30\x31\x32\x33\x40"
              "\x00\x00\x00\x00\x00\x00\x00\x00"
              // row 1
              "\x00\x00\x00\x00\x00\x00\x00\x41\x42\x43\x50\x51"
              "\x52\x53\x00\x00\x00\x00\x00\x00"
              // row 2
              "\x00\x60\x61\x62\x63\x70\x71\x00\x00\x72\x73\x00"
              "\x00\x00\x00\x00\x00\x00\x00\x00"
              // row 3
              "\x00\x80\x81\x82\x83\x90\x91\x92\x00\x00\x00\x93"
              "\xA0\x00\x00\x00\x00\x00\x00\x00"
              // row 4
              "\x00\x00\x00\xA1\xA2\xA3\xB0\xB1\xB2\xB3\xC0\xC1"
              "\xC2\xC3\xD0\x00\xD1\x00\x00\x00"
              // row 5
              "\x00\x00\x00\xD2\xD3\xE0\xE1\xE2\xE3\xF0\xF1\xF2"
              "\xF3\x59\x5A\x69\x6A\x79\x00\x00"
              // row 6
              "\x00\x00\x00\x7A\xDA\xDB\xDC\xDD\xDE\xEA\xEB\xEC"
              "\xED\xEE\xEF\xFA\xFB\x00\x00\x00"
              // row 7
              "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
              "\x00\xFC\xFD\xFE\xFF\xC4\xC5\x00",
              160);
  
}

void LaylaData::ltimPostExportStep(NesRom& rom) {
  // Write Mario sprite/background tables to new location
//  Tbyte* mariofile = readAndAllocateMarioFile("../mario.nes");
//  NesPatternTable bg = prepareMarioBg(mariofile);
//  NesPatternTable sprites = prepareMarioSprites(mariofile);
//  delete mariofile;
//  sprites.toUncompressedData(rom.directWrite(marioTablesExportDest_ + 0x0000));
//  bg.toUncompressedData(rom.directWrite(marioTablesExportDest_
//                            + 0x1000));

  // Disable cyclone
  *(rom.directWrite(0x3EFFC)) = 0x60;
  
  // Disable ammo penalty for falling in pit
  *(rom.directWrite(0x3D3ED)) = 0x60;
  
  // Disable health penalty for falling in pit
  *(rom.directWrite(0x3C1B9)) = 0xEA;
  *(rom.directWrite(0x3C1BA)) = 0xEA;
  *(rom.directWrite(0x3C1BB)) = 0xEA;
  
  // disable some dead code
  *(rom.directWrite(0x3C1BC)) = 0xEA;
  *(rom.directWrite(0x3C1BD)) = 0xEA;
  *(rom.directWrite(0x3C1BE)) = 0xEA;
  
  // Disable speed penalty for falling in pit
  *(rom.directWrite(0x3C1BF)) = 0xEA;
  *(rom.directWrite(0x3C1C0)) = 0xEA;
  *(rom.directWrite(0x3C1C1)) = 0xEA;
  
  // Reinstate health penalty for falling in pit (need to move
  // due to adding additional code)
  *(rom.directWrite(0x3C1BD)) = 0xA9;
  *(rom.directWrite(0x3C1BE)) = 0x04; // amount of damage
  *(rom.directWrite(0x3C1BF)) = 0x20;
  *(rom.directWrite(0x3C1C0)) = 0x02;
  *(rom.directWrite(0x3C1C1)) = 0xE2;
  
  // Add "restart from checkpoint" mechanism.
  // ***This depends on the removal of the pit pentalty above!***
  // 1. NOP 81C2-81C7 (don't disable interrupts after death -- will freeze
  // otherwise)
  // 2. JSR C441 @ 81C8
  // 3. JMP 813D @ 81CB
  //
  // 1. turn off dead flag ($0044 to 00)
  // 2. restore health
  // 3. redraw life bar?
  // 4. prep level load (C441, A = spawn point)
  // 5. resume logic loop (80D2? 813D?)
  //
  // 81C2:
  //   85 44          STA $0044       ; turn off death flag
  //   A9 17          LDA #17
  //   85 71          STA $0071       ; restore health to default
  //   AD 12 04       LDA $0412       ; get checkpoint num
  //   4C B2 C1       JMP $C1B2       ; ...
  //
  // 3C1B0:
  //   B0 0B          BCS ....        ; always true: skip below code
  //                                  ; when entered via pit restart
  // 3C1B2:
  //   29 7F          AND #7F         ; ignore high bit (for crusher)
  //   20 41 C4       JSR $C441       ; prep level load
  //   20 XX XX       JSR $XXXX       ; call routine to reset shields/stuff
  //   4C 3D 81       JMP $813D       ; resume logic loop
  //
  // 3DFA6:
  //   A9 00          LDA #00
  //   85 54          STA $54         ; reset shield count
  //   85 4C          STA $4C         ; equip pistol
  //                                  ; (sometimes this doesn't work???)
  //   60             RTS
  //
  // alternately, in place of previous 2 lines:
  // JMP XXXX
  //
  // ; reset boss count on death
  // XXXX:
  // A2 ??      LDX CurrentLevel  ; actually, this isn't stored anywhere??
  // LDA NumBossesArray,X
  // STA $0461
  // RTS
  std::memcpy(rom.directWrite(0x3C1B0),
              "\xB0\x0B"
              "\x29\x7F"
              "\x20\x41\xC4"
              "\x20\xA6\xDF"
              "\x4C\x3D\x81",
              13);
  std::memcpy(rom.directWrite(0x3DFA6),
              "\xA9\x00"
              "\x85\x54"
              "\x85\x4C"
              "\x60",
              7);
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x01C2;
    std::memcpy(rom.directWrite(addr),
                "\x85\x44"
                "\xA9\x17"
                "\x85\x71"
                "\xAD\x12\x04"
                "\x4C\xB2\xC1",
                12);
  }
  
  // Start + Select + A + B to exit level
  // version 1: works, but only when unpausing (pause check preempts
  // button combo check)
  // 818D:
  //   4C AE 81 JMP 81AE
  // 81AE? (post-end level -- only used in bank 2?):
  //   A9 0F    LDA #0F
  //   C5 12    CMP $0012
  //   D0 05    BNE done
  //   C6 7C    DEC $007C
  //   20 90 81 JMP $8190
  // done:
  //   20 D2 80 JMP 80D2    ; resume game loop
  //
  // version 2
  // 81E0:
  //   4C AE 81 JMP 81AE
  // 81AE? (post-end level -- only used in bank 2?):
  //   A9 0F    LDA #0F
  //   C5 12    CMP $0012
  //   D0 05    BNE done
  //   C6 7C    DEC $007C
  //   20 90 81 JMP $8190
  // done:
  //   20 58 EF JSR $EF58   ; do previous work
  //   20 E3 81 JMP 81E3    ; resume pause loop
  //
  //
  // ????:
  //   A9 0F    LDA #0F
  //   C5 12    CMP $0012
  //   D0 03    BNE done
  //   C6 7C    DEC $007C
  //            LDA $80
  //            STA $0044
  //   20 99 81 JMP $81A6
  // done:
  //   20 .. .. JMP ....
/*  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x018D;
    std::memcpy(rom.directWrite(addr),
                "\x4C\xAE\x81",
                3);
                
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x01AE;
    std::memcpy(rom.directWrite(addr),
                "\xA9\x0F"
                "\xC5\x12"
                "\xD0\x05"
                "\xC6\x7C"
                "\x4C\x90\x81"
                "\x4C\xD2\x80",
                14);
  } */
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x01E0;
    std::memcpy(rom.directWrite(addr),
                "\x4C\xAE\x81",
                3);
                
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x01AE;
    std::memcpy(rom.directWrite(addr),
                "\xA9\x0F"
                "\xC5\x12"
                "\xD0\x05"
                "\xC6\x7C"
                "\x4C\x90\x81"
                "\x20\x58\xEF"
                "\x4C\xE3\x81",
                17);
  }
  
  // Fix deceleration (mutually exclusive with no-doubled-ammo)
  // 3C4B4:
  //   20 9B DF JSR DF9B
  //   EA       NOP
  // 3DF9B:
  //   A5 31    LDA $0031   ; get x-velocity
  //   C9 88    CMP #88
  //   90 04    BCC done    ; don't zero x-velocity if <= 88
  //   A9 00    LDA #00
  //   85 31    STA $0031
  // done:
  //   60       RTS
  std::memcpy(rom.directWrite(0x3C4B4),
              "\x20\x9B\xDF"
              "\xEA",
              4);
  std::memcpy(rom.directWrite(0x3DF9B),
              "\xA5\x31"
              "\xC9\x88"
              "\x90\x04"
              "\xA9\x00"
              "\x85\x31"
              "\x60",
              11);
  
  // Disable pistol ammo penalty for replaying a level
  *(rom.directWrite(0x3D3E3)) = 0x60;
  
  // Disable non-pistol ammo penalty for replaying a level
  *(rom.directWrite(0x3D3CE)) = 0xEA;
  *(rom.directWrite(0x3D3CF)) = 0xEA;
  
  // Disable speed penalty for replaying a level
  *(rom.directWrite(0x3C0AA)) = 0xEA;
  *(rom.directWrite(0x3C0AB)) = 0xEA;
//  *(rom.directWrite(0x3C07F)) = 0xEA;
//  *(rom.directWrite(0x3C080)) = 0xEA;
//  *(rom.directWrite(0x3C081)) = 0xEA;
//  *(rom.directWrite(0x3D3E9)) = 0xEA;
//  *(rom.directWrite(0x3D3EA)) = 0xEA;
//  *(rom.directWrite(0x3D3EB)) = 0xEA;
  
  // Set starting speed
  *(rom.directWrite(0x3C0A5)) = 0x58;
  // set to 0x88 on pickup
  *(rom.directWrite(0x3EA49)) = 0xEA;
  *(rom.directWrite(0x3EA4A)) = 0xEA;
  *(rom.directWrite(0x3EA4C)) = 0x88;
  // turns out passwords only encode the low 7 bits, so 0x88
  // will be "restored" to 0x08, which is no good
//  *(rom.directWrite(0x3EA4C)) = 0x7F;
  // FUCK YOU
  // NO COMPROMISE
  
  // Pistol patch
  // Starting "ammo": 4
  *(rom.directWrite(0x3D3DF)) = 0x04;
  // Refire rate when button held (default 0x10)
  // (by comparison, machine gun is 0x04)
  *(rom.directWrite(0x3D4A0)) = 0x10;
  // set to 8 on pistol pickup
  // ...
  
  // Give axes higher x-velocity
//  *(rom.directWrite(0x3D594)) = 0x03;
//  *(rom.directWrite(0x3D594)) = 0x01;
  // and higher initial y-velocity
  *(rom.directWrite(0x3D56F)) = 0xF8;
//  *(rom.directWrite(0x3D56F)) = 0xE0;

  // Saber can penetrate walls
  *(rom.directWrite(0x3DB26)) = 0x04;
  
  // Always show password when level finished
  *(rom.directWrite(0x3E3C5)) = 0xEA;
  *(rom.directWrite(0x3E3C6)) = 0xEA;
  
  // Replace bonus game with intro cutscene
  //
  // this eliminates it entirely and goes straight to the level select
//  std::memcpy(rom.directWrite(0x81AB),
//              "\xEA\xEA\xEA",
//              3);
  // show intro cutscene instead of bonus game
  *(rom.directWrite(0x8F08)) = 0x00;
  // remove intertitle
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x01A1;
    std::memcpy(rom.directWrite(addr),
                "\xEA\xEA\xEA\xEA\xEA",
                5);
  }
  
  /* Dying returns to asteroid menu instead of resetting game
    01C0 in each level bank:
      //A9 80        LDA #$80
      //48           PHA
      //A9 18        LDA #$18     ; 24
      //48           PHA          ; set up RTS into bank 2 @ 8019
      A9 A0        LDA #$A0
      48           PHA
      A9 6A        LDA #$6A
      48           PHA          ; set up RTS into bank 2 @ A06B
      A9 02        LDA #$02
      4C 85 F8     JMP $F885    ; change to bank 2
    A06B:
      // 803A (bypass 81B1)
      A9 01        LDA #01
      85 11        STA $0011
      A9 00        LDA #00
      85 7D        STA $007D
      4C 3A 80     JMP $803A
  */
/*  std::memcpy(rom.directWrite(0xA06B),
              "\xA9\x01"
              "\x85\x11"
              "\xA9\x00"
              "\x85\x7D"
              "\x4C\x3A\x80",
              11);
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x01C0;
    std::memcpy(rom.directWrite(addr),
                "\xA9\xA0"
                "\x48"
                "\xA9\x6A"
                "\x48"
                "\xA9\x02"
                "\x4C\x85\xF8",
                11);
  } */
  
  // Prevent weapons from being manually refired faster than their
  // automatic refire rate
  //
  // 3C2A2-3: rel jump if B not pressed
  // 3C2B5-6: rel jump if B not triggered
  // 
  // 3C29E:
  //  4C 82 C8 JMP ????
  // ????:
  //  A5 4B    LDA $004B
  ///////////////  C9 01    CMP #$01
  ///////////////  F0 02    BEQ nxt
  //  C9 02    CMP #$02
  //  A9 02    LDA #$02
  //  24 12    BIT $0012
  //  90 04    BCC nxt      ; don't dec if timer >= 2
  //  D0 02    BNE nxt      ; don't dec if B pressed
  //  C6 4B    DEC $004B
  // nxt:
  //  24 12    BIT $0012    ; for return
  //  4C A2 C2 JMP C2A2
  //
  // 3C2B5 NOP NOP (don't always fire when B triggered)
  *(rom.directWrite(0x3C2B5)) = 0xEA;
  *(rom.directWrite(0x3C2B6)) = 0xEA;
  *(rom.directWrite(0x3C29E)) = 0x4C;
  *(rom.directWrite(0x3C29F)) = 0x82;
  *(rom.directWrite(0x3C2A0)) = 0xC8;
/*  std::memcpy(rom.directWrite(0x3C882),
              "\xA5\x4B"
              "\xC9\x02"
              "\x90\x02"
              "\xC6\x4B"
              "\xA9\x02"
              "\x24\x12"
              "\x4C\xA2\xC2",
              15); */
  std::memcpy(rom.directWrite(0x3C882),
              "\xA5\x4B"
              "\xC9\x02"
              "\xA9\x02"
              "\x24\x12"
              "\x90\x04"
              "\xD0\x02"
              "\xC6\x4B"
              "\x24\x12"
              "\x4C\xA2\xC2",
              19);
  // 3D63A LDA #$10 (adjust special weapon autofire -- it's 0xFF
  // in the original game, which makes for a very long delay)
  *(rom.directWrite(0x3D63B)) = 0x10;
  // The weapon cooldown counter is not initialized, which is fine in the
  // original game because it's reset every time a weapon is fired. Since
  // we disabled that, we have to initialize it ourselves.
  // 
  // A1A9:
  //  20 63 A0      JSR $A063
  // Free space: object layout for the first bonus game, A061-A088
  //  8D 00 20      STA $2000
  //  A9 01         LDA #$01
  //  85 4B         STA $4B
  //  60            RTS
  std::memcpy(rom.directWrite(0xA1A9),
              "\x20\x63\xA0",
              3);
  std::memcpy(rom.directWrite(0xA063),
              "\x8D\x00\x20"
              "\xA9\x01"
              "\x85\x4B"
              "\x60",
              8);
  // counter is getting decremented too fast; fix?
//  std::memcpy(rom.directWrite(0x3C888),
//              "\xEA\xEA",
//              2);
//  std::memcpy(rom.directWrite(0x3C2B7),
//              "\xEA\xEA",
//              2);
  
  // Don't double ammo usage after collecting partner
  // 3C87A:         ; check for "double partner" mode
  //  20 9B DF      JSR $DF9B
  //  EA EA EA EA   NOP NOP NOP NOP
  // 
  // 3DF9B:         ; unused, broken "expanded spawn" mode
  //  8A          TXA           ; if parter did not successfully
  //  30 0F       BMI done      ; fire weapon, X is FF and we're done
  //  A5 4C       LDA $4C       ; get equipped weapon ID
  //  F0 0B       BEQ done      ; if pistol, shield, or special, done
  //  C9 07       CMP #$07
  //  10 07       BPL done
  //  AA          TAX
  //  B5 4D       LDA $4D,X     ; if equipped weapon has no ammo, done
  //  F0 02       BEQ done
  //  F6 4D       INC $4D,X     ; otherwise, increment equipped weapon ammo
  //  20 14 E2    JSR $E214     ; update ammo display
  // done:
  //  60          RTS
  //  
/*  std::memcpy(rom.directWrite(0x3C87A),
              "\x20\x9B\xDF"
              "\xEA\xEA\xEA\xEA",
              7);
  std::memcpy(rom.directWrite(0x3DF9B),
              "\x8A"
              "\x30\x12"
              "\xA5\x4C"
              "\xF0\x0E"
              "\xC9\x07"
              "\x10\x0A"
              "\xAA"
              "\xB5\x4D"
              "\xF0\x05"
              "\xF6\x4D"
              "\x20\x14\xE2"
              "\x60",
              22); */

  // Disable music change during levels (cave music will always play)
  // 
  // 1. don't start cave music when level begins (if we made the level
  // start somewhere other than cave, the game will synchronously load
  // a new graphics bank, causing the music to hang)
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x00BA;
    std::memcpy(rom.directWrite(addr),
                "\xEA\xEA\xEA",
                3);
  }
  // 2. change "currently playing" byte (0x466) to a garbage value
  // at level start so the game will start playing the cave music after
  // it's loaded the first part of the level
  //
  // 8058:
  //  20 6B A0    JSR A06B      ; don't write 00 to currently playing
  // A06B:
  //  A9 44       LDA #$44      ; any invalid value (not 0, 1, or 81)
  //                            ; (doubles as "damage enemies when iris
  //                            ; touches them" flag)
  //  8D 66 04    STA $0466
  //  8D 64 04    STA $0464
  //  A9 00       LDA #$00      ; for remainder of init routine
  //  85 54       STA $0054     ; reset shield count to 0
  //                            ; completely and totally irrelevant to
  //                            ; this routine, but it's convenient
  //  60          RTS
  //  
  // first bonus game, after previous hack
  std::memcpy(rom.directWrite(0x8058),
              "\x20\x6B\xA0",
              3);
  std::memcpy(rom.directWrite(0xA06B),
              "\xA9\x44"
              "\x8D\x66\x04"
//              "\x8D\x64\x04"
              "\xA9\x00"
              "\x85\x54"
              "\x60",
              10);
  // 3. disable music change itself
  //      (currently, this just NOPs the call that would trigger the music
  //      change -- in the future, this should be replaced by a JSR to a
  //      location in the movable bank that is an RTS in regular levels,
  //      but has been modified to possibly play certain music in others,
  //      e.g. the boss theme)
//  *(rom.directWrite(0x3F0FE)) = 0xEA;
//  *(rom.directWrite(0x3F0FF)) = 0xEA;
//  *(rom.directWrite(0x3F100)) = 0xEA;
  
  // ok, here's the real deal
  // 126C in each level bank is the update code for the cyclone.
  // We've disabled that and don't need it anymore, so we replace the
  // original instruction that sets the new target music value at 465 with
  // a JSR to 926C.
  *(rom.directWrite(0x3F0FE)) = 0x20;
  *(rom.directWrite(0x3F0FF)) = 0x6C;
  *(rom.directWrite(0x3F100)) = 0x92;
  
  // Obviously we don't want to run the cyclone update code, so we initially
  // overwrite the first instruction with an RTS in all banks. All levels
  // that don't override this behavior will play one track continuously
  // throughout.
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x126C;
//    *(rom.directWrite(addr)) = 0x60;
    
    // adding boss theme: accept ONLY boss
    // C9 81      CMP #81
    // D0 03      BNE done   ; branch if not requesting boss
    // 8D 65 04   STA $0465  ; trigger boss music
    // done:
    // 60         RTS
    std::memcpy(rom.directWrite(addr),
                "\xC9\x81"
                "\xD0\x03"
                "\x8D\x65\x04"
                "\x60",
                8);
  }
  
  // Level 8 has multiple tracks, so we need to override it.
  // Base location: 0x2926C
  // 00 = cave
  // 01 = base
  // 81 = boss
  //
  // ; if target music is cave:
  //   ; if current music is base, don't change
  //   ; otherwise, play
  // ; otherwise, start new music
  //
  // ; trying to switch to cave music?
  // C9 00      CMP #$00
  // D0 0B      BNE notCave
  //
  // ; ignore cave request if current music is base
  // 48         PHA
  // AD 66 04   LDA $0466
  // C9 01      CMP #$01
  // D0 02      BNE dontIgnore
  // 68         PLA
  // 60         RTS
  // 
  // dontIgnore:
  // 68         PLA
  //
  // notCave:
  // ; trigger new music
  // 8D 65 04   STA $0465
  //
  // done:
  // 60         RTS
  // 
  std::memcpy(rom.directWrite(0x2926C),
              "\xC9\x00"
              "\xD0\x0B"
              "\x48"
              "\xAD\x66\x04"
              "\xC9\x01"
              "\xD0\x02"
              "\x68"
              "\x60"
              "\x68"
              "\x8D\x65\x04"
              "\x60",
              19);
  // STA $0465
//  std::memcpy(rom.directWrite(addr),
//              "\x8D\x65\x04",
//              3);
  
  // this still leaves the problem of the music hanging momentarily during
  // the bank change between different areas, but this happens infrequently
  // enough that we can hopefully get away with it
  
  // fix freezes caused by overflowing sprite table
  // the game uses a table in RAM at 130-17F to keep track of OAM slots,
  // flipping the order of most entries on alternate draws to achieve
  // sprite flicker ("virtual" sprite slots are mapped to "physical"
  // by a table lookup). However, there's no check for overflow, so
  // if there are too many sprites, sprite 0 will get overwritten.
  // Sprite 0 is reset to its proper position at vblank(?),
  // but if the game lags and this doesn't happen in time/at the correct
  // time, sprite 0 won't be at the right position and will usually
  // cause a crash.
  // Thus we need to add a check to ensure this doesn't happen.
  // FAF7:
  //  4C B1 DF    JMP ????
  // ????:
  //  85 22       STA $0022
  //  D0 02       BNE done
  //  E6 22       INC $0022         // if slot is 0, increase to 1
  // done:
  //  60          RTS
  std::memcpy(rom.directWrite(0x3FAF7),
              "\x4C\xB1\xDF",
              3);
  // more of the "expanded spawn" code
  std::memcpy(rom.directWrite(0x3DFB1),
              "\x85\x22"
              "\xD0\x02"
              "\xE6\x22"
              "\x60",
              7);
              
  // disable low hp alarm
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x0390;
    *(rom.directWrite(addr)) = 0xEA;
    *(rom.directWrite(addr + 1)) = 0xEA;
  }
  
  // flash BG palette 0-1 instead of 0-0
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x03C8;
    // game writes this to all background palettes to avoid showing
    // wrong colors during forced blanking when loading new area --
    // this is no longer necessary, so direct all writes to the new slot
    for (int j = addr; j < addr + 0x10; j += 2) {
      *(rom.directWrite(j + 1)) = 0xC1;
    }
  }
  
  // reduce rate of flashing
/*  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x03AC;
    *(rom.directWrite(addr)) = 0x0F;
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x03BD;
    *(rom.directWrite(addr)) = 0x10;
  } */
  
  // all health items restore 2 hp
  *(rom.directWrite(0x3CE36)) = 0x02;
  *(rom.directWrite(0x3CE39)) = 0x02;
  
  // layla spawns lower so we can make an alternate route above her
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x1043;
    *(rom.directWrite(addr)) = 0x88;
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x13A7;
    *(rom.directWrite(addr)) = 0x88;
  }
  // floor level in mission 9
  *(rom.directWrite(0x2C000 + 0x1043)) = 0xB8;
  *(rom.directWrite(0x2C000 + 0x13A7)) = 0xB8;
  
  
  // special item pickups always restore hp
  *(rom.directWrite(0x3EA54)) = 0xEA;
  *(rom.directWrite(0x3EA55)) = 0xEA;
  *(rom.directWrite(0x3EA5F)) = 0xEA;
  *(rom.directWrite(0x3EA60)) = 0xEA;
  *(rom.directWrite(0x3EA61)) = 0xEA;
  // amount of hp = 2
  *(rom.directWrite(0x3EA57)) = 0x02;
  
  // Touching enemies causes damage based on current level number instead
  // of enemy's current HP
  // Formula: (levelnum >> 1) + 1
  // 3CE85:
  //    A5 7C   LDA $7C
  //    4A      LSR
  std::memcpy(rom.directWrite(0x3CE85),
              "\xA5\x7C\x4A",
              3);
  
  // shield pick-up gives 2 ammo instead of 5
  *(rom.directWrite(0x3EC10)) = 0x02;
  
  // pistol refire rate (default = 0x10)
  *(rom.directWrite(0x3D4A0)) = 0x0B;
  
  // machine gun refire rate (default = 0x04)
  *(rom.directWrite(0x3D4DE)) = 0x06;
  
  // flamethrower refire rate (default = 0x08)
  *(rom.directWrite(0x3D559)) = 0x09;
  
  // don't use ammo
  std::memcpy(rom.directWrite(0x3D4E1),
              "\xEA\xEA",
              2);
  std::memcpy(rom.directWrite(0x3D51F),
              "\xEA\xEA",
              2);
  std::memcpy(rom.directWrite(0x3D55C),
              "\xEA\xEA",
              2);
  std::memcpy(rom.directWrite(0x3D589),
              "\xEA\xEA",
              2);
  // what the fuck
  std::memcpy(rom.directWrite(0x3D672),
              "\xEA\xEA",
              2);
  std::memcpy(rom.directWrite(0x3D5C1),
              "\xEA\xEA",
              2);
  std::memcpy(rom.directWrite(0x3D5E9),
              "\xEA\xEA",
              2);
  
  // give 255 ammo on weapon pickup
  std::memcpy(rom.directWrite(0x3EC0A),
              "\xFF\xFF\xFF\xFF\xFF\xFF",
              6);
  
  // tone down damage done by "heavier" weapons
  // machine gun (orig: 0x03)
//  *(rom.directWrite(0x3DF52)) = 0x02;
  // flamethrower (orig: 0x05)
  *(rom.directWrite(0x3DF54)) = 0x04;
  // axe (orig: 0x06)
  *(rom.directWrite(0x3DF55)) = 0x05;
  // bazooka (orig: 0x0A)
  *(rom.directWrite(0x3DF56)) = 0x06;
  // saber (orig: 0x0F)
  *(rom.directWrite(0x3DF57)) = 0x08;
  
  // preset damage to enemy on iris contact
//  std::memcpy(rom.directWrite(0x3CE91),
//              "\xA9\x04"
//              "\xEA\xEA\xEA",
//              5);
              
  // never clear "damage enemies on iris contact" flag
//  std::memcpy(rom.directWrite(0x3EA64),
//             "\xEA\xEA\xEA",
//              3);

  // only 1 boss kill is needed to end level 5 instead of 3
  *(rom.directWrite(0x3F64E)) = 0x01;
  
  // increase flamethrower range (orig: 0x22)
//  *(rom.directWrite(0x3D554)) = 0x2A;
  *(rom.directWrite(0x3D554)) = 0x28;
  
  // "ASTEROID" -> "MISSION "
  // SELECT ASTEROID
  std::memcpy(rom.directWrite(0x3E485),
              "\x17\x13\x1D\x1D\x13\x19\x18\x00",
              8);
  // ASTEROID X
  std::memcpy(rom.directWrite(0x3E496),
              "\x00\x17\x13\x1D\x1D\x13\x19\x18",
              8);
              
  // crank up invulnerability time (orig: 0x0F)
  *(rom.directWrite(0x3CEC3)) = 0x1C;
  
  // damage done by projectiles (orig: 0x02)
//  *(rom.directWrite(0x3EDAC)) = 0x02;

  // damage done by spikes (orig: 0x02)
//  *(rom.directWrite(0x3C7BB)) = 0x02;

  // for some reason projectiles hit through damage invincibility.
  // this might be ok except that the miichin enemy shoots a new projectile
  // every time the last one it fired disappears, which means if layla
  // bumps into it it will continually fire shots and rapidly drain her
  // health
  // this makes projectiles check for invincibility first
  // DFAD:
  //   48         PHA
  //   A5 70      LDA 0070
  //   D0 04      BNE done
  //   68         PLA
  //   4C 02 E2   JMP DamageLayla
  // done:
  //   68         PLA
  //   60         RTS
  //
  // EDAD:
  //   20 AD DF   JSR DFAD
  //
  // no fucking room
  std::memcpy(rom.directWrite(0x3D6D9),
              "\x48"
              "\xA5\x70"
              "\xD0\x04"
              "\x68"
              "\x4C\x02\xE2"
              "\x68"
              "\x60",
              11);
  std::memcpy(rom.directWrite(0x3EDAD),
              "\x20\xD9\xD6",
              3);
  // CE7D
  // edad
/*  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x3DAD;
    std::memcpy(rom.directWrite(addr),
                "\x00\x17\x13\x1D\x1D\x13\x19\x18",
                8);
  } */
  
  // disable final boss invincibility
  *(rom.directWrite(0x3D6D7)) = 0x10;
  
  // don't substitute "?" when a new power-up is found
  std::memcpy(rom.directWrite(0x3EBBE),
              "\xA9\x01",
              2);
  
  // change tilemap of "?" power-up to heart
  std::memcpy(rom.directWrite(0x3E4E0),
              "\xD0\xD1\xD2\xD3",
              4);
              
  // don't draw special weapon on weapon select menu
  std::memcpy(rom.directWrite(0x3E54C),
              "\x00\x00",
              2);
  std::memcpy(rom.directWrite(0x3E56C),
              "\x00\x00",
              2);
  // don't draw special weapon ammo count
//  std::memcpy(rom.directWrite(0x3E092),
//              "\x03",
//              1);
  std::memcpy(rom.directWrite(0x3E096),
              "\x04",
              1);
  
  // shorten the right end of the HUD box to account for the missing
  // special weapon
  // "connectors"
  std::memcpy(rom.directWrite(0x3E187),
              "\x58",
              1);
  std::memcpy(rom.directWrite(0x3E191),
              "\xB8",
              1);
  // side frame 1 (not needed)
//  std::memcpy(rom.directWrite(0x3E1AA),
//              "\x78",
//              1);
  // shorten top and bottom frames
  std::memcpy(rom.directWrite(0x3E196),
              "\x17",
              1);
  // right side of frame
  // for some reason this is initialized, then overwritten later
  // when the menu is brought up; we only need to change the latter
  std::memcpy(rom.directWrite(0x3E54C),
              "\xC7",
              1);
  std::memcpy(rom.directWrite(0x3E553),
              "\x00",
              1);
  std::memcpy(rom.directWrite(0x3E56C),
              "\xC7",
              1);
  std::memcpy(rom.directWrite(0x3E573),
              "\x00",
              1);
              
  // on weapon select menu, lock screen farther to the left (to
  // account for the missing special weapon)
  // default values: 0x49 ("raw" scroll) and 0xF0 (right lock value)
  // and 0x2A (cursor offset for screen lock?)
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x0312;
    std::memcpy(rom.directWrite(addr),
                "\x3F",
                1);
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x0320;
    std::memcpy(rom.directWrite(addr),
                "\xC8",
                1);
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x0327;
    std::memcpy(rom.directWrite(addr),
                "\x20",
                1);
    
    // and while we're at it: suppress the "A button changes
    // cursor direction" thing, since it isn't used anymore
    // (frees 2CD-2DA)
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x02CB;
    std::memcpy(rom.directWrite(addr),
                "\xD0\x11",
                2);
  }
              
  // don't draw number of keys collected
  // initialization
  std::memcpy(rom.directWrite(0x3E12B),
              "\xEA\xEA\xEA",
              3);
  // refresh during gameplay
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x04E4;
    std::memcpy(rom.directWrite(addr),
                "\xEA\xEA\xEA\xEA\xEA\xEA",
                6);
  }
  
  // don't draw ammo count unless shield is equipped
/*  E23A:
  JSR E13F ; already present
  
  ; old routine to compute keys collected
  E13F:
  A5 4C     LDA $004C     ; get equipped weapon
  C9 07     CMP #07       ; shield?
  D0 08     BNE done      ; leave quantity as-is
  
  A9 00     LDA #00       ; quantity is null
  85 5E     STA $005E
  85 5E     STA $005F
  85 5E     STA $0060
  
  done:
  60        RTS */
  std::memcpy(rom.directWrite(0x3E13F),
              "\xA5\x4C"
              "\xC9\x07"
              "\xF0\x08"
              "\xA9\x00"
              "\x85\x5E"
              "\x85\x5F"
              "\x85\x60"
              "\x60",
              15);
/*  E128:
  20 35 D6  JSR D635
  
  ; old routine for firing special weapon
  D635:
  A5 4C     LDA $004C     ; get equipped weapon
  C9 07     CMP #07       ; shield?
  D0 0B     BNE done      ; leave quantity as-is
  
  A9 00     LDA #00       ; quantity is null
  9D 13 04  STA $0413,X
  9D 14 04  STA $0414,X
  9D 15 04  STA $0415,X
  
  done:
  60        RTS */
  std::memcpy(rom.directWrite(0x3E128),
              "\x20\x35\xD6",
              3);
  std::memcpy(rom.directWrite(0x3D635),
              "\xA5\x4C"
              "\xC9\x07"
              "\xF0\x0B"
              "\xA9\x00"
              "\x9D\x13\x04"
              "\x9D\x14\x04"
              "\x9D\x15\x04"
              "\x60",
              18);
  // E0A1:  ; drawing ammo quantities for pause menu
  // 20 47 D6 JSR D647
  //
  // D647:
  // 98       TYA         ; save weapon index
  // 48       PHA
  // B9 4D 00 LDA $004D,Y ; get ammo count
  // 20 6A E2 JSR E26A    ; do decimal conversion
  // 68       PLA         ; retrieve weapon index
  // 20 37 D6 JSR D637    ; set displayed quantity to null if not shield
  // 60       RTS
  std::memcpy(rom.directWrite(0x3E0A1),
              "\x20\x47\xD6",
              3);
  std::memcpy(rom.directWrite(0x3D647),
              "\x98"
              "\x48"
              "\xB9\x4D\x00"
              "\x20\x6A\xE2"
              "\x68"
              "\x20\x37\xD6"
              "\x60",
              13);
  // ; if speed is 58, draw ? over skate icon on pause menu
  // E06A:
  // C9 58    CMP #58
  // D0 07    BNE xx
  // 20 97 CF JSR CF97
  // EA EA EA EA    NOP NOP NOP NOP
  // xx:
  //
  // CF97?:
  // A9 38    LDA #38
  // 8D 17 04 STA $0417
  // A9 48    LDA #48
  // 8D 18 04 STA $0418
  // A9 39    LDA #39
  // 8D 37 04 STA $0437
  // A9 49    LDA #49
  // 8D 38 04 STA $0438
  // 60       RTS
  //
  std::memcpy(rom.directWrite(0x3E06A),
              "\xC9\x58"
              "\xD0\x07"
              "\x20\x97\xCF"
              "\xEA\xEA\xEA\xEA",
              11);
  std::memcpy(rom.directWrite(0x3CF97),
              "\xA9\x38"
              "\x8D\x17\x04"
              "\xA9\x48"
              "\x8D\x18\x04"
              "\xA9\x39"
              "\x8D\x37\x04"
              "\xA9\x49"
              "\x8D\x38\x04"
              "\x60",
              21);
  // erase default "000" speed value from tilemap
  std::memcpy(rom.directWrite(0x3E518),
              "\x00\x00\x00",
              3);
              
  // move weapon/ammo display to the right
  // when switching with select
  for (int i = 0; i < 9; i++) {
    int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x04BD;
    std::memcpy(rom.directWrite(addr),
                "\x78",
                1);
    addr = ((i + 3) * UxRomBanking::sizeOfPrgBank) + 0x04D1;
    std::memcpy(rom.directWrite(addr),
                "\x98",
                1);
  }
  // move initial pistol tilemap
  std::memcpy(rom.directWrite(0x3E5AA),
              "\x00\x00\xE2\xF2",
              4);
  std::memcpy(rom.directWrite(0x3E5CA),
              "\x00\x00\xE3\xF3\x00\x00\x00",
              7);
  // when switching from weapon menu
  std::memcpy(rom.directWrite(0x3E10A),
              "\x2D",
              1);
  std::memcpy(rom.directWrite(0x3E110),
              "\x2E",
              1);
  std::memcpy(rom.directWrite(0x3E116),
              "\x4D",
              1);
  std::memcpy(rom.directWrite(0x3E11C),
              "\x4E",
              1);
  // and move ammo display (or null) to right
  std::memcpy(rom.directWrite(0x3E124),
              "\x3A",
              1);
  
  // allow food to be picked up while invincible
  // CDC6:
  // 20 AC CF JSR XXXX
  // EA EA    NOP NOP
  //
  // XXXX:
  // BD 00 05 LDA $0500,X     ; is this object food?
  // C9 01    CMP #01
  // D0 04    BNE notFood
  // 
  // BD 20 05 LDA $0520,X     ; food: don't check invulnerability
  // 60       RTS
  //
  // notFood:
  // BD 20 05 LDA $0520,X
  // 05 70    ORA $0070       ; anything else: check invulnerability
  // 60       RTS
  std::memcpy(rom.directWrite(0x3CDC6),
              "\x20\xAC\xCF"
              "\xEA\xEA",
              5);
  std::memcpy(rom.directWrite(0x3CFAC),
              "\xBD\x00\x05"
              "\xC9\x01"
              "\xD0\x04"
              "\xBD\x20\x05"
              "\x60"
              "\xBD\x20\x05"
              "\x05\x70"
              "\x60",
              17);
  
  // E0A1 = prep numbers for draw during pause
  // E0A1:
  // JSR XXXX
  //
  // XXXX:  ; E125?
  // 20 XX XX JSR XXXX     ; init to 00 00 00
  // XX XX    CMP #07 ; whatever shield is
  // XX XX    BCS done     
  // 20 6A E2 JSR ConvertToDecimal3Digit
  // done:
  // 60       RTS
  
  // restore speed value correctly from password
  // original game only encodes 7 bits, but we need 8; speed should be
  // either 58 (no upgrade) or 88 (upgrade), and if it's not 58 after
  // restoring from password we can assume it should be 88
  // A45F:
  // 20 E0 BF   JSR BFE0
  //
  // BFE0:
  // 20 E1 A4   JSR $A4E1  ; fetch encoded speed value
  // C9 58      CMP #58    ; if valid, we're done
  // F0 02      BEQ done
  // A9 88      LDA #88    ; otherwise, prepare upgraded value
  //
  // done:
  // 60         RTS
/*  std::memcpy(rom.directWrite(0xA45F),
              "\x20\xE0\xBF",
              3);
  std::memcpy(rom.directWrite(0xBFE0),
              "\x20\xE1\xA4"
              "\xC9\x58"
              "\xF0\x02"
              "\xA9\x88"
              "\x60",
              10); */
  std::memcpy(rom.directWrite(0xA4C1),
              "\xEA\xEA",
              2);
  std::memcpy(rom.directWrite(0xA4CA),
              "\x88",
              1);
  std::memcpy(rom.directWrite(0xA4D0),
              "\x88",
              1);
  
  // lower volume of sound effects
  int sfxVolOpAddr[] = {
    0xCA4,
    0xE0E,
    0xE42,
    0xE4E,
    0xE70,
    0xE7A,
    0xE9C,
    0xEA6,
    0xEB6,
    0xEC0,
    0xED0,
    0xEDA,
    0xEEA,
    0xEFA,
    0xF0A,
    0xF1A,
    0xF2A,
    0xF3C,
    0xF50,
    0xF64
  };
  std::vector<int> soundEffectVolumeOpcodeAddresses(
   sfxVolOpAddr, sfxVolOpAddr + (sizeof(sfxVolOpAddr) / sizeof(int)));
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < soundEffectVolumeOpcodeAddresses.size(); j++) {
      int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank)
         + soundEffectVolumeOpcodeAddresses[j];
      int op = *(rom.directRead(addr));
      if (op != 0x81) {
        std::cerr << "***NOT ADJUSTING A VOLUME OP!!***" << std::endl;
      }
      
      ++addr;
      
      int val = *(rom.directRead(addr));
      
//      std::memcpy(rom.directWrite(addr),
//                  "\xEA\xEA\xEA\xEA\xEA\xEA",
//                  6);
      *(rom.directWrite(addr)) = (val - 2);
    }
  }
  
  // some sound effects override CHANVOL commands with REG2
  int sfxRegOpAddr[] = {
    0xCA6,
    0xCEC,
    0xD28,
    0xD3B,
    0xD50,
    0xD62,
    0xD77,
    0xD90,
    0xDA5,
    0xDC1,
    0xE10,
    0xE50,
    0xE7C,
    0xEC2,
    0xEDC,
    0xEF4,
    0xF46,
    0xF5A
  };
  std::vector<int> soundEffectRegOpcodeAddresses(
   sfxRegOpAddr, sfxRegOpAddr + (sizeof(sfxRegOpAddr) / sizeof(int)));
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < soundEffectRegOpcodeAddresses.size(); j++) {
      int addr = ((i + 3) * UxRomBanking::sizeOfPrgBank)
         + soundEffectRegOpcodeAddresses[j];
      int op = *(rom.directRead(addr));
      if (op != 0x91) {
        std::cerr << "***NOT ADJUSTING A REG OP!!***" << std::endl;
      }
      
      ++addr;
      
      int val = *(rom.directRead(addr));
      
      int vol = val & 0x0F;
      if (vol > 2) {
        vol -= 2;
      }
      
      val = (val & 0xF0);
      val = (val | vol);
      
//      std::memcpy(rom.directWrite(addr),
//                  "\xEA\xEA\xEA\xEA\xEA\xEA",
//                  6);

      *(rom.directWrite(addr)) = val;
    }
  }
  
  // when mission 9 is started, remove all weapons, speed upgrade,
  // and layla
  // 2C080:
  // 20 B0 BF JSR BFB0
  //
  // 2FFB0:
  // A9 00    LDA #00
  // ; weapons
  // 85 4E    STA $004E
  // 85 4F    STA $004F
  // 85 50    STA $0050
  // 85 51    STA $0051
  // 85 52    STA $0052
  // 85 53    STA $0053
  // ; layla
  // 8D 6A 04 STA $046A
  // ; speed upgrade
  // A9 58    LDA #58
  // 85 34    STA $0034
  // ; make up work
  // 20 5D C0 JSR $C05D
  // 60       RTS
  std::memcpy(rom.directWrite(0x2C080),
              "\x20\xB0\xBF",
              3);
  std::memcpy(rom.directWrite(0x2FFB0),
              "\xA9\x00"
              "\x85\x4E"
              "\x85\x4F"
              "\x85\x50"
              "\x85\x51"
              "\x85\x52"
              "\x85\x53"
              "\x8D\x6A\x04"
              "\xA9\x58"
              "\x85\x34"
              "\x20\x5D\xC0"
              "\x60",
              25);
  
  // level end check for mission 9
  // 8140:  ; immediately after interrupt wait
  // 20 D0 BF JSR BFD0
  //
  // BFD0:
  // ; is the high bit of the y-flag set?
  // AD 12 04 LDA $0412
  // 2A       ROL
  // 90 06    BCC done
  // ; mark level as done
  // A5 44    LDA $0044
  // 09 80    ORA #80
  // 85 44    STA $0044
  // 
  // done:
  // ; make up work
  // A5 49    LDA $0049
  // 4A       LSR
  // 60       RTS
/*  std::memcpy(rom.directWrite(0x2C140),
              "\x20\xD0\xBF",
              3);
  std::memcpy(rom.directWrite(0x2FFD0),
              "\xAD\x12\x04"
              "\x2A"
              "\x90\x06"
              "\xA5\x44"
              "\x09\x80"
              "\x85\x44"
              "\xA5\x49"
              "\x4A"
              "\x60",
              16); */
  // version 2: check if a boss has been defeated
  // 8140:  ; immediately after interrupt wait
  // 20 D0 BF JSR BFD0
  //
  // BFD0:
  // ; game reads past end of boss count table and thinks there are
  // 0x98 bosses for the level, so we check if there are 0x97 left
  // AD 61 04 LDA $0461
  // C9 97    CMP #97
  // D0 06    BNE done
  // ; mark level as done
  // A5 44    LDA $0044
  // 09 80    ORA #80
  // 85 44    STA $0044
  //
  // ; temporarily freeze
  // A9 FF        LDA #FF
  // 85 82        STA $0082      ; driver tempo = 255 ("freeze" sound)
  // A2 80        LDX #80        ; # frames to delay
  // start:
  // A9 80        LDA #80        ; interrupt flags
  // 20 F7 83     JSR 83F7       ; wait for interrupt
  // CA           DEX
  // D0 F8        BNE start
  // 
  // done:
  // ; make up work
  // A5 49    LDA $0049
  // 4A       LSR
  // 60       RTS
  std::memcpy(rom.directWrite(0x2C140),
              "\x20\xD0\xBF",
              3);
  std::memcpy(rom.directWrite(0x2FFD0),
              "\xAD\x61\x04"
              "\xC9\x97"
              "\xD0\x14"
              "\xA5\x44"
              "\x09\x80"
              "\x85\x44"
              
              "\xA9\xFF"
              "\x85\x82"
              "\xA2\xA0"
              "\xA9\x80"
              "\x20\xF7\x83"
              "\xCA"
              "\xD0\xF8"
              
              "\xA5\x49"
              "\x4A"
              "\x60",
              31);
  
  // alter title palettes
  *(rom.directWrite(0xA988 + 0x05)) = 0x0B;
  *(rom.directWrite(0xA988 + 0x06)) = 0x19;
  *(rom.directWrite(0xA988 + 0x0D)) = 0x0B;
  
//  Graphic g;
//  NesPaletteQuad q;
//  std::map<Tcolor, NesColor> c;
//  graphicToTile(g, q, c);

  // copy title bank to bank 12 (new ending)
  std::memcpy(rom.directWrite(0x30000),
              (char*)(titleBankOrig),
              UxRomBanking::sizeOfPrgBank);

  // copy title bank to bank 14 (intro)
  std::memcpy(rom.directWrite(0x38000),
              (char*)(titleBankOrig),
              UxRomBanking::sizeOfPrgBank);
  
  // jump to new ending when level 7 (mission 8) is completed
  // 28193:
  // jump to this bank when level complete
  // since the entire title bank is copied, we'll end up at the right place
  *(rom.directWrite(0x2819A)) = 0x0C;
  // level 8 (mission 9)
  *(rom.directWrite(0x2C19A)) = 0x0D;
  // for mission 9, don't check level num
//  std::memcpy(rom.directWrite(0x2C197),
//              "\xEA\xEA",
//              2);
  // do check level num; otherwise, mission exit code is equivalent to
  // beating level
  std::memcpy(rom.directWrite(0x2C196),
              "\x08",
              1);
              
  // bump up mission 3 boss hp
  // fun fact, this is apparently also the value that decides how long
  // the boss takes to spawn!
/*  std::memcpy(rom.directWrite(0x175F3),
              "\x5E",
              1); */
              
  // bump up mission 9 boss hp
  std::memcpy(rom.directWrite(0x2F5F3),
              "\x2E",
              1);
              
  // screw with stuff when mission 9 boss spawns
  // 2F4FA:
  // 20 70 BF   JSR BF70
  // 
  // BF70:
  // A9 02      LDA #02       ; tempo
  // 85 82      STA $0082
  // A9 29      LDA #29       ; master tone shift
  // 85 8B      STA $008B
  // A9 0A      LDA #0A       ; ch1 reg2
  // 8D 8E 01   STA $018E
  // A9 09      LDA #09       ; ch2 reg2
  // 8D AE 01   STA $01AE
  //
  // ; iris y-position
  // A5 21      LDA $0021     ; this might be the rng?
  // 8D 7C 04   STA $047C
  //
  // ; shake screen
  // A5 49      LDA $0049     ; frame limiter
  // 4A         LSR
  // 4A         LSR
  // 90 04      BCC next
  // A9 FF      LDA #FF
  // B0 02      BCS next2
  // next:
  // A9 01      LDA #01
  // next2:
  // 85 19      STA $0019
  //
  // ; just for the hell of it, turn on scrolling once we've
  // ; completely spawned
  // BD 88 05   LDA $0588,X   ; zero until fully spawned
  // F0 08      BEQ done
  // A9 00      LDA #00       ; scrolling is enabled when zero
  // 8D 5A 04   STA $045A
  //
  // BD 88 05   LDA $0588,X   ; make up work
  // done:
  // 60         RTS
  std::memcpy(rom.directWrite(0x2F4FA),
              "\x20\x70\xBF",
              3);
  std::memcpy(rom.directWrite(0x2FF70),
//              "\xA9\x02"
              "\xA5\x7E"
              "\x29\x03"  // AND 03 (tempo)
              "\x85\x82"
              "\xE6\x82"  // INC $0082 (tempo)
//              "\xA9\x29"
              "\xA9\x18"
              "\x85\x8B"
//              "\xA9\x0A"
              "\xA5\x7E"
              "\x8D\x8E\x01"
//              "\xA9\x09"
              "\xA5\x21"
              "\x8D\xAE\x01"
              
              "\xA5\x21"
              "\x8D\x7C\x04"
              
//              "\xA5\x49"
              "\xA5\x7E"
              "\x4A"
              "\x4A"
              "\x4A"
              "\x90\x04"
              "\xA9\xFF"
              "\xB0\x02"
              "\xA9\x01"
              "\x85\x19"
              
              "\xBD\x88\x05"
              "\xF0\x08"
              "\xA9\x00"
              "\x8D\x5A\x04"
              "\xBD\x88\x05"
              "\x60",
              56);
  
  // modify new ending to switch to bank 2 before restarting
  // 32AAA:
  // 4C 16 80 JMP $8016 ; no idea what's here, but we shouldn't need it
  // 
  // 30016:
  // ; set return address
  // A9 80    LDA #80
  // 48       PHA
  // A9 02    LDA #02
  // 48       PHA
  // ; change to bank 2 and ret to reset
  // A9 02    LDA #02
  // 4C 85 F8 JMP $F885
  std::memcpy(rom.directWrite(0x32AAA),
              "\x4C\x16\x80",
              3);
  std::memcpy(rom.directWrite(0x30016),
              "\xA9\x80"
              "\x48"
              "\xA9\x02"
              "\x48"
              "\xA9\x02"
              "\x4C\x85\xF8",
              11);
  
/*  std::vector<std::string> endingStrings;
  endingStrings.push_back(
    stringEncode("FIRST\xE2"));
  endingStrings.push_back(
    stringEncode("SECOND\xE2"));
  endingStrings.push_back(
    stringEncode("THIRD\xE2"));
  for (int i = 0; i < 0x40; i++) {
  endingStrings.push_back(
    stringEncode("\x08NEXT\xE4"));
  }
  endingStrings.push_back("\xFF"); */
  
/*  std::string endingTable;
  buildStringTable(endingTable,
                   endingStrings,
                   0xAC7A);
  std::memcpy(rom.directWrite(0x32C7A),
              endingTable.c_str(),
              endingTable.size()); */
              
  // Layla portrait:
  // 88 98 C6 D6
  // 89 99 C7 D7
  // A8 B8 C8 D8
  // A9 B9 C9 D9
  
  // Iris portrait:
  // 84 94 A4 B4
  // 85 95 A5 B5
  // 86 96 A6 B6
  // 87 97 A7 B7
  
  // tilemap data
  std::string endingStringsRaw[] = {
/*    "  FIRST\xE2",
    "  SECOND\xE2",
    "  THIRD\xE2",
    "  FOURTH\xE2",
    "  FIFTH\xE2",
    "  SIXTH\xE2",
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9\xE1",
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7\xE1",*/
    
    "\xE4",
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 ...IRIS REPORTING IN.\xE2",
    "   YEAH, HE GOT AWAY.\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 (AGAIN.)\xE4",

    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 YOU SHUT IT! ANYWAY,\xE2",
    "   MISSION COMPLETE. GO TELL\xE2",
    "   HQ TO GET READY TO BLOW A\xE2",
    "   GASKET. OUT.\xE4",
          
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 GOD, WE BLEW IT AGAIN.\xE2",
    "   HOW'S THAT GUY DO IT?\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 DUNNO, BUT MAN, DO I\xE2",
    "   WANT ONE OF THOSE LITTLE\xE2",
    "   PERSONAL UFO THINGS HE\xE2",
    "   FLIES AROUND ON!\xE4",
          
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 ISN'T A PERSONAL SPACE\xE2",
    "   MOTORBIKE GOOD ENOUGH?\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 ...ACTUALLY, YEAH.\xE4",
          
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 ANYWAY, LOOKS LIKE WE'RE\xE2",
    "   OUT OF STUFF TO BLOW UP\xE2",
    "   AGAIN. WHAT'S OUR CONTINGEN-\xE2",
    "   CY PLAN?\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 VR SIM? DANGER ROOM?\xE2",
    "   ...LARP?\xE4",
          
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 ...UH, ACTUALLY, I THINK\xE2",
    "   I'LL TAKE THE BIKE OUT FOR\xE2",
    "   A SPIN, SEE IF I CAN FIND\xE2",
    "   SOME SPACE BLOBS TO INCINER-\xE2",
    "   ATE. WEIRD HOW WE DIDN'T RUN\xE2",
    "   INTO ANY THIS TIME. THAT \xE2",
    "   LAST PLACE WAS SWARMING WITH\xE2",
    "   THEM!\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 DON'T REMIND ME.\xF0",

/*    "  AND SO, CRISIS AVERTED, EV-\xE2",
    "  ERYTHING WENT BACK TO NORMAL.\xE2",
    "  OR AT LEAST AS NORMAL AS IT\xE2",
    "  GETS FOR TWO HEAVILY-ARMED,\xE2",
    "  TRIGGER-HAPPY SPACE COMMAN-\xE2",
    "  DOS...\xE4", */

    "  SOMEWHERE IN THE UNIVERSE, A\xE2",
    "  CERTAIN MAN GRINDS HIS TEETH,\xE2",
    "  SPUTTERS, SPITS, AND SWEARS\xE2",
    "  REVENGE.\xE2",
    "  ...BUT NO ONE IS AROUND TO\xE2",
    "  LISTEN.\xE4",
    
    // dummy -- skipped
    "\x02" "\xE2",
    
    "\x02" "\xEA",
    "\x04" "- ORIGINAL LAYLA STAFF -\xEE",
    "\x0C" "PRODUCER\xE4",
      "\x09" "NAOTO SHINADA\xEC",
    "\x0B" "DIRECTORS\xE4",
      "\x0A" "HIROSHI ONO\xE2",
      "\x08" "SEIICHI MATSUNO\xEC",
    "\x08" "PRODUCT MANAGER\xE4",
      "\x09" "MEGUMI YOSHIDA\xEC",
    "\x0A" "PROGRAMMERS\xE4",
      "\x09" "NAOTO SHINADA\xE2",
      "\x0A" "HIROSHI ONO\xE2",
      "\x07" "YUKIHARU NAKAJIMA\xEC",
    "\x08" "GRAPHIC DESIGNER\xE4",
      "\x09" "BACKY MATSUNO\xEC",
    "\x09" "MUSIC DIRECTOR\xE4",
      "\x09" "YASUHITO SAITO\xEC",
    "\x08" "PACKAGE DESIGNER\xE4",
      "\x0A" "AYAKO MATSUI\xF0",

    "\x05" "- THE IRIS MISSIONS -\xEE",
    "\x06" "NEW GRAPHICS, MUSIC,\xE2",
    "\x04" "LEVELS, AND PROGRAMMING\xE4",
      "\x0D" "SUPPER\xEC",

    "\x09" "ORIGINAL GAME\xE2",
    "\x09" "@1986 dB-SOFT\xF0",

    "\x0D" "LAYLA\xE2",
    "\x07" "THE IRIS MISSIONS\xE4",
/*    "\x04" "VERSION 1 -- 20 DEC 2016\xE4",
    "\x04" "(HAPPY 30TH ANNIVERSARY,\xE2",
    "\x0C" "LAYLA!)\xE8", */
    "\x04" "VERSION 2 -- 1 JAN 2017\xE4",
    "\x07" "(HAPPY NEW YEAR!)\xE2",
    "\x0C" "\xE8",

    "\x06" "THANKS FOR PLAYING!\xE8",

    "\x06" "IN MEMORY OF dB-SOFT\xE2",
    "\x0A" "(1980-2003)\xFE",

    "\x0C" "THE END\xFE",

/*    "\x0C" "\xE2",
    "\x0A" "...OR IS IT?\xE2",
    "\x09" "IN MISSION 4,\xE2",
    "\x07" "ENTER THE PASSWORD\xE4",
    "\x0B" "'ERICC!!'\xE4",
    "\x02" "THEN...STAY AWAY FROM LAYLA!\xE8",
    "\xFF" */

//    "\x0C" "\xE2",
    "\x0A" "...OR IS IT?\xE2",
    "\x09" "\xE2",
    "\x07" "\xE4",
    "\x0A" "PUSH START!\xE4",
    "\x02" "\xE6",
    "\xFF"
  };
  
  int numEndingStrings = sizeof(endingStringsRaw) / sizeof(std::string);
  std::vector<std::string> endingStrings(endingStringsRaw,
                                        endingStringsRaw + numEndingStrings);
  for (int i = 0; i < endingStrings.size(); i++) {
    endingStrings[i] = stringEncode(endingStrings[i]);
  }
  
  // tilemap data
  std::string endingTable;
  buildStringTable(endingTable,
                   endingStrings,
                   0x8ED2);
  std::memcpy(rom.directWrite(0x30ED2),
              endingTable.c_str(),
              endingTable.size());
  
  // update tilemap load address
  std::memcpy(rom.directWrite(0x32C46),
              "\xD2\x8E",
              2);
  std::memcpy(rom.directWrite(0x32C4B),
              "\xD3\x8E",
              2);
  
  // attribute table data
  std::string endingAttributesRaw[] = {
    // ...IRIS
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    // (AGAIN.)
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    // YOU SHUT IT!
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // GOD, WE
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    // DUNNO, BUT
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // ISN'T A
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    // ...ACTUALLY
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    // ANYWAY
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // VR SIM
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // ...UH, ACTUALLY
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // DON'T
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    // AND SO
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // dummy -- skipped
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // credits
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // ORIGINAL LAYLA STAFF
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    // PRODUCER
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // DIRECTORS
    std::string("\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // PRODUCT MANAGER
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // PROGRAMMERS
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // GRAPHIC DESIGNER
    std::string("\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // MUSIC DIRECTOR
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // PACKAGE DESIGNER
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // - THE IRIS MISSIONS -
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    // NEW
    std::string("\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", 8),
    std::string("\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // ORIGINAL GAME
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    // LAYLA
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // fuck attribute tables
//    std::string("\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", 8),
//    std::string("\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    // THANKS
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // IN MEMORY
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    // THE END
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    // OR IS IT?
//    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
//    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
//    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
//    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8)
  };
  
  int numEndingAttributes = sizeof(endingAttributesRaw) / sizeof(std::string);
  std::vector<std::string> endingAttributes(endingAttributesRaw,
                 endingAttributesRaw + numEndingAttributes);
  
  // attribute data
  std::string endingAttributeTable;
  buildStringTable(endingAttributeTable,
                   endingAttributes,
                   0x9622);
  std::memcpy(rom.directWrite(0x31622),
              endingAttributeTable.c_str(),
              endingAttributeTable.size());
  
  // update attribute load address
  std::memcpy(rom.directWrite(0x32C54),
              "\x22\x96",
              2);
  std::memcpy(rom.directWrite(0x32C59),
              "\x23\x96",
              2);
              
  // extend letter-by-letter portion of credits
  std::memcpy(rom.directWrite(0x32A2E),
              "\x8E",
              1);
  std::memcpy(rom.directWrite(0x32AE6),
              "\x90",
              1);
              
  // slow down scrolling
  std::memcpy(rom.directWrite(0x32B16),
              "\x03",
              1);
              
  // copy modified ending to bank 13 (bonus ending)
  // we'll change it as needed for the new ending
  std::memcpy(rom.directWrite(0x34000),
              (char*)(rom.directRead(0x30000)),
              UxRomBanking::sizeOfPrgBank);
              
  // in ending 1 only, return to mission select when start is pressed
  // after credits end
  // 32A9B:
  // A5 4E    LDA $004E   ; level select expects this on stack
  // 48       PHA
  // A9 81    LDA #HI     ; target address: 81AB
  // 48       PHA
  // A9 AA    LDA #LO-1
  // 48       PHA
  // A9 02    LDA #02     ; target bank
  // 4C 85 F8 JMP $F885
  std::memcpy(rom.directWrite(0x32A9B),
              "\xA5\x4E"
              "\x48"
              "\xA9\x81"
              "\x48"
              "\xA9\xAA"
              "\x48"
              "\xA9\x02"
              "\x4C\x85\xF8",
              14);
  
  // tilemap data
  std::string ending2StringsRaw[] = {
/*    "  FIRST\xE2",
    "  SECOND\xE2",
    "  THIRD\xE2",
    "  FOURTH\xE2",
    "  FIFTH\xE2",
    "  SIXTH\xE2",
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9\xE1",
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7\xE1",*/
    
    "\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 IRIS!\xE2",
    "   WHAT THE HELL, I THOUGHT YOU\xE2",
    "   SAID YOU WERE TAKING THE\xE2",
    "   BIKE OUT!?\xE4",
    
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 ...UGH... LITTLE NAU-\xE2",
    "   SEOUS HERE...\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 DON'T TELL ME YOU'VE\xE2",
    "   BEEN PLUGGING YOUR STUPID\xE2",
    "   ANTIQUE GAMES INTO THE VR\xE2",
    "   SIM AGAIN! YOU REMEMBER HOW\xE2",
    "   MUCH OUR PAY GOT DOCKED WHEN\xE2",
    "   THE HIGHER-UPS FOUND OUT!?\xE4",
    
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 OKAY, OKAY, JESUS!\xE2",
    "   DON'T YANK THE CORD LIKE\xE2",
    "   THAT. IT MAKES EVERYTHING\xE2",
    "   BLOW UP.\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 SERVES YOU RIGHT.\xE2",
    "   HONESTLY, I CAN'T BELIEVE\xE2",
    "   THIS OLD JUNK EVEN WORKS\xE2",
    "   ANYMORE.\xE4",
    
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 OH, THESE ARE REPROS.\xE2",
    "   THE ORIGINALS ALL BITROTTED\xE2",
    "   AWAY CENTURIES AGO. SOME OF\xE2",
    "   THESE ARE IN ORIGINAL PRO-\xE2",
    "   DUCTION CASES, THOUGH!\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 ...RIGHT.\xE2",
    "   MAN, YOU HAVE THE WEIRDEST\xE2",
    "   TASTES. WHY NOT PLAY SOME-\xE2",
    "   THING A LITTLE MORE, YOU\xE2",
    "   KNOW, RECENT?\xE4",
    
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 VIDEO GAMES HAVE BEEN\xE2",
    "   DEAD TO ME SINCE THEY TURNED\xE2",
    "   INTO SUBSTITUTE GAMBLING.\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 OH, WHEN DID THAT\xE2",
    "   HAPPEN?\xE4",
    
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 1972.\xE2",
    "   ...BUT AT LEAST THEY\xE2",
    "   COULDN'T SIPHON YOUR BANK\xE2",
    "   ACCOUNT BACK THEN.\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 YOU'RE HOPELESS, BUT\xE2",
    "   YOU KNOW THAT.\xE4",
    
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 YEP.\xE2",
    "   ...HOW 'BOUT SOME DOUBLE\xE2",
    "   DRAGON?\xE4",
          
    "  \x88\x98\xC6\xD6\xE1",
    "  \x89\x99\xC7\xD7\xE1",
    "  \xA8\xB8\xC8\xD8\xE1",
    "  \xA9\xB9\xC9\xD9 (SIGH)\xE2",
    "   OKAY, BUT ONLY IF I GET TO\xE2",
    "   BE PLAYER ONE.\xE4",
    
    "  \x84\x94\xA4\xB4\xE1",
    "  \x85\x95\xA5\xB5\xE1",
    "  \x86\x96\xA6\xB6\xE1",
    "  \x87\x97\xA7\xB7 UGH, YOU ALWAYS HAVE\xE2",
    "   TO BE PLAYER ONE!\xFE",
    
    // dummy -- skipped
    "\x02" "\xE2",
    
    "\x02" "\xE4",
    "\x03" "- SUPER SPECIAL CREDITS -\xF8",
    "\x07" "SPECIAL THANKS TO\xF0",
      "\x01" "- NESDEV'S NES DOCS\xF0",
      "\x01" "- BLUECHIP'S 6502 REFERENCE\xF0",
      "\x01" "- EVERYONE OVER AT TCRF FOR\xE2",
      "\x01" "DISTRACTING ME FROM THIS\xF0",
      "\x01" "- THE WEBMASTER OF ASTERIA NO\xE2",
      "\x01" "LAYLA. CHECK IT OUT!\xE4",
      "\x05" "LAYLAIRIS.WEB.FC2.COM\xF0",
      "\x01" "- GAME CENTER CX, FOR BRING-\xE2",
      "\x01" "ING THIS GAME TO MY ATTENTION\xF0",
      "\x01" "- dB-SOFT, FOR MAKING IT IN\xE2",
      "\x01" "THE FIRST PLACE\xF0",
      "\x01" "- ARSTANNECKBEARD, FOR TESTING\xF0",
      "\x01" "- YOU!\xF2",
      "\x06" "THANKS FOR PLAYING!\xFE",
      "\x02" "\xE6",
      "\x0C" "THE END\xEF",
    "\xFF"
  };
  
  int numEnding2Strings = sizeof(ending2StringsRaw) / sizeof(std::string);
  std::vector<std::string> ending2Strings(ending2StringsRaw,
                                        ending2StringsRaw + numEnding2Strings);
  for (int i = 0; i < ending2Strings.size(); i++) {
    ending2Strings[i] = stringEncode(ending2Strings[i]);
  }
  
  // tilemap data
  std::string ending2Table;
  buildStringTable(ending2Table,
                   ending2Strings,
                   0x8ED2);
  std::memcpy(rom.directWrite(0x34ED2),
              ending2Table.c_str(),
              ending2Table.size());
  
  // attribute table data
  std::string ending2AttributesRaw[] = {
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // IRIS!
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x05\x01\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // UGH
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    // DON'T TELL ME
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x05\x01\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // OKAY, OKAY
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // SERVES YOU RIGHT
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x05\x01\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // OH, THESE
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // ...RIGHT
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // VIDEO GAMES
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // OH, WHEN
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // 1972
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // YOU'RE HOPELESS
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // YEP
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // (SIGH)
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x55\x11\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    // UGH
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\x33\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x0F\x03\x00\x00\x00\x00\x00\x00", 8),
    
    // dummy -- skipped
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // credits
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // SUPER SPECIAL
    std::string("\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", 8),
    // SPECIAL THANKS
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // YOU!
//    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    
    // THANKS FOR PLAYING!
//    std::string("\x00\x00\x00\x00\x00\x00\x00\x00", 8),
    std::string("\x55\x55\x55\x55\x55\x55\x55\x55", 8),
    
    // THE END
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8),
    std::string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8)
  };
  
  int numEnding2Attributes = sizeof(ending2AttributesRaw) / sizeof(std::string);
  std::vector<std::string> ending2Attributes(ending2AttributesRaw,
                 ending2AttributesRaw + numEnding2Attributes);
  
  // attribute data 2
  std::string ending2AttributeTable;
  buildStringTable(ending2AttributeTable,
                   ending2Attributes,
                   0x9622);
  std::memcpy(rom.directWrite(0x35622),
              ending2AttributeTable.c_str(),
              ending2AttributeTable.size());
              
  // extend letter-by-letter portion of credits 2
  std::memcpy(rom.directWrite(0x36A2E),
              "\xB8",
              1);
  std::memcpy(rom.directWrite(0x36AE6),
              "\xBA",
              1);
  
              
              
  //************title*******************
              
  // don't print scores on title screen
/*  std::memcpy(rom.directWrite(0xA23C),
              "\xEA\xEA\xEA",
              3);
  std::memcpy(rom.directWrite(0xA24D),
              "\xEA\xEA\xEA",
              3); */
//  std::memcpy(rom.directWrite(0xA22D),
//              "\xEA\xEA\xEA",
//              3);
              
  // new tilemap: "SUPPER 2016" (replacing score/hi-score)
/*  std::memcpy(rom.directWrite(0xA7D6),
              (std::string("\x10\x01")
              + stringEncode("SUPPER 2016     ")).c_str(),
              18);
  // change write location (ppu address)
  std::memcpy(rom.directWrite(0xA22A),
              "\x23",
              1);
  std::memcpy(rom.directWrite(0xA22C),
              "\x6C",
              1); */
              
  // move dB-soft copyright down a line to make room for extra
  std::memcpy(rom.directWrite(0xA21D),
              "\x6A",
              1);
              
  // remove unnecessary period from copyright
/*  std::memcpy(rom.directWrite(0xA7D5),
              "\x00",
              1); */
  // adjust copyright message
  std::memcpy(rom.directWrite(0xA7C7),
              (stringEncode("@1986   dB-SOFT")).c_str(),
              15);
  
  // add extra line to copyright
  // A25F:
  // 20 50 BF   JSR BF50
  //
  // BF50:
  // 20 6C F9   JSR F96C
  // A9 XX      LDA #SRC_HI
  // 85 05      STA $0005
  // A9 XX      LDA #SRC_LO
  // 85 04      STA $0004
  // A2 XX      LDX #DST_HI
  // A0 XX      LDY #DST_LO
  // 20 24 E3   JSR $E324
  // 60         RTS
/*  std::memcpy(rom.directWrite(0xBF70),
              (std::string("\x10\x01")
                + stringEncode("ORIGINAL GAME   ")).c_str(),
              18); */
  std::memcpy(rom.directWrite(0xBF70),
              (std::string("\x10\x01")
                + stringEncode("ORIGINAL GAME   ")).c_str(),
              18);
  std::memcpy(rom.directWrite(0xA25F),
              "\x20\x50\xBF",
              3);
  std::memcpy(rom.directWrite(0xBF50),
              "\x20\x6C\xF9"
              // data src: BF70
              "\xA9\xBF"
              "\x85\x05"
              "\xA9\x70"
              "\x85\x04"
              // ppu dst: 234A
              "\xA2\x23"
              "\xA0\x4A"
              "\x20\x24\xE3"
              "\x60",
              19);
              
  // adjust db-soft copyright placement (it's off center due to adjustments)
  std::memcpy(rom.directWrite(0xA21D),
              "\x69",
              1);
              
  // ; initialize title screen timer
  // A27F:
  // 20 00 BF   JSR $BF00
  // EA         NOP         ; stray byte
  //
  // BF00:
  // ; make up previous work
  // 85 0A      STA $000A
  // 85 11      STA $0011
  // ; init timer
  // A9 LO      LDA #LO
  // 8D A0 06   STA $06A0
  // A9 HI      LDA #HI
  // 8D A1 06   STA $06A1
  // 60         RTS
  std::memcpy(rom.directWrite(0xA27F),
              "\x20\x00\xBF"
              "\xEA",
              4);
  std::memcpy(rom.directWrite(0xBF00),
              "\x85\x0A"
              "\x85\x11"
              // low
//              "\xA9\x48"
              "\xA9\xFF"
              "\x8D\xA0\x06"
              // high
//              "\xA9\x04"
              "\xA9\x05"
              "\x8D\xA1\x06"
              "\x60",
              15);
  
  // ; decrement title screen timer and start intro if needed
  // A28D?:
  // 20 20 BF   JSR $BF20
  // 
  // BF20:
  // 20 F4 F8   JSR $F8F4   ; make up previous work (read controllers?)
  // CE A0 06   DEC $06A0
  // D0 18      BNE +
  // CE A1 06   DEC $06A1
  // D0 13      BNE +
  //
  // timerDone:
  // ; don't start intro if password entry has been triggered
  // A5 0A      LDA $000A
  // D0 0F      BNE +
  // ; destroy pending RTS
  // 68         PLA
  // 68         PLA
  // ; and pending RTS from title loop (we're not coming back from this)
  // 68         PLA
  // 68         PLA
  // ; prepare virtual RTS addr
  // A9 HI      LDA #HI
  // 48         PHA
  // A9 LO-1    LDA #LO-1
  // 48         PHA
  // ; intro bank number
  // A9 0E      LDA #0E
  // ; change to intro bank and RTS to new code
  // 4C 85 F8   JMP $F885
  // 
  // +:
  // 60         RTS
  std::memcpy(rom.directWrite(0xA28D),
              "\x20\x20\xBF",
              3);
  std::memcpy(rom.directWrite(0xBF20),
              "\x20\xF4\xF8"
              "\xCE\xA0\x06"
              "\xD0\x18"
              "\xCE\xA1\x06"
              "\xD0\x13"
              "\xA5\x0A"
              "\xD0\x0F"
              "\x68"
              "\x68"
              "\x68"
              "\x68"
              // HI
              "\xA9\xA9"
              "\x48"
              // LO-1
              "\xA9\xA7"
              "\x48"
              "\xA9\x0E"
              "\x4C\x85\xF8"
              "\x60",
              33);
  
  // don't start ending music during intro
  std::memcpy(rom.directWrite(0x3A9EE),
              "\xEA\xEA\xEA",
              3);
  // in fact, init line number instead
  std::memcpy(rom.directWrite(0x3A9EC),
              "\xA9\x02"
              "\x85\x04\xEA",
              5);
  // and don't overwrite it later
  std::memcpy(rom.directWrite(0x3A9F9),
              "\xEA\xEA",
              2);
  // don't start title music
  std::memcpy(rom.directWrite(0x3AB0D),
              "\xEA\xEA\xEA",
              3);
  
  // trigger scrolling text immediately
  std::memcpy(rom.directWrite(0x3AA2E),
              "\x02",
              1);
  std::memcpy(rom.directWrite(0x3AADB),
              "\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA",
              10);
  // make sure 0011 is zeroed immediately
  std::memcpy(rom.directWrite(0x3AADB),
              "\xA9\x00\x85\x11",
              4);
  
  // no music fade during intro
  std::memcpy(rom.directWrite(0x3AAD2),
              "\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA",
              9);
              
  // slow down scrolling
  std::memcpy(rom.directWrite(0x3AB16),
              "\x03",
              1);
  
  // start printing from line 2 (line 0 is reserved for "credits finished")
  std::memcpy(rom.directWrite(0x3AAE6),
              "\x02",
              1);
  
  // tilemap data
  std::string introStringsRaw[] = {
    // dummy
    "\x02" "\xE4",
    // initial line breaks
    "\x02" "\xE2",
    "\x02" "-- ASTERIA, THE EARTH'S STAR\xE2",
    "\x02" "COLONY.\xEA",
    
    "\x02" "USING HIS GENETIC ENHANCEMENT\xE2",
    "\x02" "MACHINE, 'JIL', THE SCIENTIST\xE2",
    "\x02" "CHINELKOV MANITOKHA TERROR-\xE2",
    "\x02" "IZED THE SYSTEM WITH HORDES\xE2",
    "\x02" "OF THE MUTANT CHIMAIRAN.\xEA",
    
    "\x02" "FACED WITH THIS THREAT, THE\xE2",
    "\x02" "MILITARY FORMED A SPECIAL\xE2",
    "\x02" "UNIT, THE CHIMAIRAN ATTACK\xE2",
    "\x02" "TEAM (CAT), TO COMBAT MANI-\xE2",
    "\x02" "TOKHA.\xEA",
    
    "\x02" "IN A DESPERATE AND HARROWING\xE2",
    "\x02" "MISSION, THE CAT OPERATIVES\xE2",
    "\x02" "LAYLA AND IRIS SUCCEEDED IN\xE2",
    "\x02" "DESTROYING JIL, BUT WERE UN-\xE2",
    "\x02" "ABLE TO CAPTURE MANITOKHA.\xE2",
    "\x02" "ESCAPING INTO THE REMOTE\xE2",
    "\x02" "DEPTHS OF THE ASTEROID BELT,\xE2",
    "\x02" "HE HAS SLOWLY REBUILT HIS\xE2",
    "\x02" "FORCES IN PREPARATION FOR A\xE2",
    "\x02" "NEW ASSAULT.\xEA",
    
    "\x02" "AFTER MONTHS OF INTENSIVE\xE2",
    "\x02" "SEARCHING, A CAT RECONNAI-\xE2",
    "\x02" "SSANCE TEAM HAS FINALLY\xE2",
    "\x02" "LOCATED MANITOKHA'S HIDDEN\xE2",
    "\x02" "FACILITIES. LAYLA HAS BEEN\xE2",
    "\x02" "DISPATCHED TO CARRY OUT A\xE2",
    "\x02" "COVERT STRIKE AGAINST THE\xE2",
    "\x02" "RECONSTRUCTED JIL.\xEA",
    
    "\x02" "YOU, IRIS, HAVE BEEN SEPA-\xE2",
    "\x02" "RATELY TASKED WITH ELIMINA-\xE2",
    "\x02" "TING A SERIES OF PERIPHERAL\xE2",
    "\x02" "CHIMAIRAN TARGETS AND MAKING\xE2",
    "\x02" "A RENDEZVOUS WITH LAYLA FOR\xE2",
    "\x02" "A FINAL ATTACK AGAINST\xE2",
    "\x02" "MANITOKHA.\xEA",
    
    "\x02" "THERE ARE EIGHT KNOWN TARGETS\xE2",
    "\x02" "IN THE COMPOUND. FIND THEM\xE2",
    "\x02" "AND TAKE THEM OUT ANY WAY YOU\xE2",
    "\x02" "CAN.\xFC",
    
    "\x02" "LITTLE TIME REMAINS. DESTROY\xE2",
    "\x02" "THE CHIMAIRAN AND STOP\xE2",
    "\x02" "MANITOKHA!\xEC",
    "\xFF"
  };
  
  int numIntroStrings = sizeof(introStringsRaw) / sizeof(std::string);
  
  
  std::vector<std::string> introStrings(introStringsRaw,
                                        introStringsRaw + numIntroStrings);
  for (int i = 0; i < introStrings.size(); i++) {
    introStrings[i] = stringEncode(introStrings[i]);
  }
  std::string introTable;
  buildStringTable(introTable,
                   introStrings,
                   0x8ED2);
  std::memcpy(rom.directWrite(0x38ED2),
              introTable.c_str(),
              introTable.size());
              
  // set all line attribute table entries to palette 0
  for (int i = 0x3B1B9; i < 0x3B263; i += 2) {
    std::memcpy(rom.directWrite(i),
                "\x63\xB2",
                2);
  }
  
  // update tilemap load address
  std::memcpy(rom.directWrite(0x3AC46),
              "\xD2\x8E",
              2);
  std::memcpy(rom.directWrite(0x3AC4B),
              "\xD3\x8E",
              2);
              
  // don't wait for start after finishing messages + music
  std::memcpy(rom.directWrite(0x3AA91),
              "\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA\xEA",
              10);
              
  // if start is pressed at any time, we're done
  // AA84:
  // JMP XXXX
  //
  // XXXX:
  // JSR $F8F4
  // LDA $08
  // BIT $something,something
  // BNE +7
  // BIT $something,something
  // BNE +3
  // JMP $AA9B
  // JMP $AA87
  std::memcpy(rom.directWrite(0x3AA84),
              "\x4C\x00\xBF",
              3);
  std::memcpy(rom.directWrite(0x3BF00),
              "\x20\xF4\xF8"
              "\xA9\x08"
              "\x24\x14"
              "\xD0\x07"
              "\x24\x12"
              "\xF0\x03"
              "\x4C\x9B\xAA"
              "\x4C\x87\xAA",
              19);
  
  
}

bool LaylaData::tileIsBlank(const NesTile& tile) {
  for (int j = 0; j < tile.height; j++) {
    for (int i = 0; i < tile.width; i++) {
      if (tile.data(i, j) != 0) {
        return false;
      }
    }
  }
  
  return true;
}

bool colorInPalette(NesColor color, const NesPalette& palette) {
  for (int i = 0; i < palette.numColors(); i++) {
    if (color == palette.color(i)) {
      return true;
    }
  }
  
  return false;
}

bool matchPalette(std::unordered_map<NesColor, Tcolor, NesColorHash> colors,
                  const NesPalette& palette) {
  for (std::unordered_map<NesColor, Tcolor, NesColorHash>::iterator it = colors.begin();
       it != colors.end();
       ++it) {
    if (!(colorInPalette(it->first, palette))) {
      return false;
    }
  }
  
  return true;
}

int matchPalettes(std::unordered_map<NesColor, Tcolor, NesColorHash> colors,
                  const NesPaletteQuad& palettes) {
  for (int i = 0; i < palettes.numPalettes(); i++) {
    if (matchPalette(colors, palettes.palette(i))) {
      return i;
    }
  }
  
  return -1;
}

int matchColor(NesColor color, const NesPalette& palette) {
  for (int i = 0; i < palette.numColors(); i++) {
    if (palette.color(i) == color) {
      return i;
    }
  }
  
  return -1;
}

std::unordered_map<Tcolor, int, TcolorHash> mapColors(std::unordered_map<NesColor, Tcolor, NesColorHash> usedColors,
                                const NesPalette& palette) {
  std::unordered_map<Tcolor, int, TcolorHash> result;
  
  for (std::unordered_map<NesColor, Tcolor, NesColorHash>::iterator it = usedColors.begin();
       it != usedColors.end();
       ++it) {
    result[it->second] = matchColor(it->first, palette);
  }
  
  return result;
}

NesTile LaylaData::graphicToTile(Graphic& src,
                                 const NesPaletteQuad& palettes,
                                 std::unordered_map<Tcolor, NesColor, TcolorHash> colorMap) {
  NesTile tile;
  
  if ((src.w() != NesTile::width)
      || (src.h() != NesTile::height)) {
    throw TGenericException(TALES_SRCANDLINE,
                            "LaylaData::graphicToTile(const Graphic&)",
                            "Bad source image size");
  }
  
  // 1. Analyze image color data. If there are more than four colors, die.
  // 2. Match each real color to a color in the map.
  // 3. Try to find a palette containing all the colors.
  // 4. If no palette contains all the colors, die.
  // 5. Map each real color to a sub-palette index.
  // 6. Fill in the tile and return.
  
  std::unordered_map<NesColor, Tcolor, NesColorHash> usedColors;
  
  for (int j = 0; j < src.h(); j++) {
    for (int i = 0; i < src.w(); i++) {
      std::unordered_map<Tcolor, NesColor, TcolorHash>::iterator it
        = colorMap.find(src.getPixel(i, j));
      
      if (it == colorMap.end()) {
        throw TGenericException(TALES_SRCANDLINE,
                                "LaylaData::graphicToTile(const Graphic&)",
                                "Encountered color not in colorMap");
      }
        
//      std::cout << (unsigned int)(it->first.g())
//        << " " << (unsigned int)(it->second.nativeValue()) << std::endl;
      
      usedColors[it->second] = it->first;
      
    }
  }
  
  if (usedColors.size() > 4) {
    throw TGenericException(TALES_SRCANDLINE,
                            "LaylaData::graphicToTile(const Graphic&)",
                            "Too many colors in source image");
  }
  
  int paletteNum = matchPalettes(usedColors, palettes);
  
  if (paletteNum == -1) {
    throw TGenericException(TALES_SRCANDLINE,
                            "LaylaData::graphicToTile(const Graphic&)",
                            "No matching palette for tile");
  }
  
  std::unordered_map<Tcolor, int, TcolorHash> realColorMap
    = mapColors(usedColors, palettes.palette(paletteNum));
  
  for (int j = 0; j < tile.height; j++) {
    for (int i = 0; i < tile.width; i++) {
      tile.setData(i, j, realColorMap[src.getPixel(i, j)]);
    }
  }
  
  return tile;
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
