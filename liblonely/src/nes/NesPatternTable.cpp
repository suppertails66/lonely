#include "nes/NesPatternTable.h"

namespace Lonely {


NesPatternTable::NesPatternTable() { };
  
void NesPatternTable::fromUncompressedData(const Tbyte* data) {
  for (int i = 0; i < numTiles; i++) {
    NesTile tile;
    data += tile.fromUncompressedData(data);
    tiles_[i] = tile;
  }
}
  
void NesPatternTable::toUncompressedData(Tbyte* data) const {
  for (int i = 0; i < numTiles; i++) {
    data += tiles_[i].toUncompressedData(data);
  }
}
  
int NesPatternTable::save(Tstring& data) const {
  Tbyte buffer[dataSize];
  
  toUncompressedData(buffer);
  data += std::string((char*)(buffer), dataSize);
  
  return dataSize;
}

int NesPatternTable::load(const Tbyte* data) {
  fromUncompressedData(data);
  
  return dataSize;
}
  
NesTile& NesPatternTable::tile(int index) {
  return tiles_[index];
}

const NesTile& NesPatternTable::tile(int index) const {
  return tiles_[index];
}


};
