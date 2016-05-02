#include "parser/ClaptonParser.h"
#include "parser/SoundProgramToken.h"
#include "gamedata/LaylaData.h"
#include "nes/NesRom.h"
#include "util/FileManip.h"
#include <fstream>
#include <iostream>

namespace Lonely {


ClaptonParser::ClaptonParser() { };
  
void ClaptonParser::parseFile(const std::string& filename) {
//  tokens.clear();

  std::ifstream ifs(filename);
  
  unsigned int fileSize = FileManip::getFileSize(ifs);
  
  if ((int)fileSize <= 0) {
    return;
  }
  
  char* buffer = new char[fileSize];
  ifs.read(buffer, fileSize);
  std::string inputString(buffer, fileSize);
  delete buffer;
  ifs.close();
  
  parseString(inputString);
}
  
void ClaptonParser::parseString(std::string& src) {
  int pos = 0;
  int lineNum = 1;
  
  // Do a top-down parse. The resulting token sequence will be stored
  // in parseStateData_.
  bool result = soundProgram_.fromString(src, pos, lineNum, parseStateData_);
}
  
void ClaptonParser::writeRawData(const std::string& filepath) {
  parseStateData_.outputToFiles("output/");
}

void ClaptonParser::writeToRom(const std::string& romfilename,
                               const std::string& outputfilename) {
  NesRom rom(romfilename);
  
  parseStateData_.outputToRom(rom);
  
//  std::cout << romfilename << " " << outputfilename << std::endl;
  
//  std::cout << NesRom::nametablesHorizontal
//    << " " << LaylaData::exportNametableFlag() << std::endl;
  
  rom.exportToFile(outputfilename,
                   rom.size() / UxRomBanking::sizeOfPrgBank,
                   LaylaData::numExportChrBanks(),
                   LaylaData::exportNametableFlag(),
                   LaylaData::exportMapperNum());
}


};
