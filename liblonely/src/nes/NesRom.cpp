#include "nes/NesRom.h"
#include "util/FileManip.h"
#include "exception/TGenericException.h"
#include "util/ByteConversion.h"
#include "util/StringConversion.h"
#include <cstring>
#include <fstream>

namespace Lonely {


const Tbyte NesRom::inesIdentifier[inesIdentifierSize_]
  = { 'N', 'E', 'S', 0x1A };
  
NesRom::NesRom(const Tstring& filename)
  : rom_(),
    mapperType_(mapperUxRom) {
  readFromFile(filename);
}
  
void NesRom::exportToFile(const Tstring& filename,
                    int numPrgBanks,
                    int numChrBanks,
                    NametableArrangementFlag nametablesFlag,
                    MapperType mapperNum) {
  std::ofstream ofs(filename, std::ios_base::binary
                                | std::ios_base::trunc);
  
  Tbyte buffer[ByteSizes::uint32Size];
  
  // iNES header
  
  // "NES" identifier
  ofs.write((const char*)inesIdentifier, inesIdentifierSize_);
  
  // ROM size
  ByteConversion::toBytes(numPrgBanks,
                          buffer,
                          ByteSizes::uint8Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  ofs.write((char*)buffer, ByteSizes::uint8Size);
  
  ByteConversion::toBytes(numChrBanks,
                          buffer,
                          ByteSizes::uint8Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  ofs.write((char*)buffer, ByteSizes::uint8Size);
  
  // Flags
  Tbyte flags6 = 0;
  Tbyte flags7 = 0;
  
  flags6 |= nametablesFlag;
  flags6 |= ((mapperNum & 0x0F) << 4);
  flags7 |= ((mapperNum & 0xF0) << 4);
  
  ofs.write((char*)(&flags6), ByteSizes::uint8Size);
  ofs.write((char*)(&flags7), ByteSizes::uint8Size);
  
  // Bytes 8-15 aren't implemented
  for (int i = 8; i < 16; i++) {
    char c = 0x00;
    ofs.write((char*)(&c), ByteSizes::uint8Size);
  }
  
  // Data
  ofs.write((char*)(rom_.data()), rom_.size());
}
  
const Tbyte* NesRom::directRead(Taddress address) const {
  return rom_.data() + address;
}

Tbyte* NesRom::directWrite(Taddress address) {
  return rom_.data() + address;
}
  
NesRom::MapperType NesRom::mapperType() const {
  return mapperType_;
}

void NesRom::setMapperType(MapperType mapperType__) {
  mapperType_ = mapperType__;
}

void NesRom::readFromFile(const Tstring& filename) {
  if (!FileManip::fileExists(filename)) {
    throw TGenericException(TALES_SRCANDLINE,
                           "NesRom::readFromFile(const Tstring&)",
                           "Tried to read nonexistent file");
  }
  
  std::ifstream ifs(filename,
                    std::ios_base::binary);
  int fileSize = FileManip::getFileSize(ifs);
  
  if (fileSize < minFileSize_) {
    throw TGenericException(TALES_SRCANDLINE,
                           "NesRom::readFromFile(const Tstring&)",
                           "Tried to read invalid-sized file");
  }
  
  // Check for presence of iNES header
  Tbyte inesCheck[inesIdentifierSize_];
  ifs.read((char*)inesCheck, inesIdentifierSize_);
  
  if (std::memcmp(inesIdentifier, inesCheck, inesIdentifierSize_)
        == 0) {
    // Read iNES data?
    // ...
    
    // Skip to start of actual data
    ifs.seekg(inesHeaderOffset_ + inesHeaderSize_);
  }
  else {
    // No header: return to start of file
    ifs.seekg(0);
  }
  
  // Compute ROM size (accounting for presence or absence of header)
  int romSize = fileSize - ifs.tellg();
  rom_.resize(romSize);
  
  // Read entire file
  ifs.read((char*)(rom_.data()), romSize);
  
}
  
int NesRom::size() const {
  return rom_.size();
}
  
void NesRom::changeSize(int newSize) {
  Tarray<Tbyte> newData(newSize);
  
  int amount = (newSize < rom_.size()) ? newSize : rom_.size();
  
  std::memcpy(newData.data(),
              rom_.data(),
              amount);
  
  rom_ = newData;
}


};
