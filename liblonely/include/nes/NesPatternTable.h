#ifndef NESPATTERNTABLE_H
#define NESPATTERNTABLE_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "nes/NesTile.h"

namespace Lonely {


class NesPatternTable {
public:
  const static int numTiles = 0x100;
  const static int dataSize = numTiles * NesTile::size;

  NesPatternTable();
  
  void fromUncompressedData(const Tbyte* data);
  
  void toUncompressedData(Tbyte* data) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  NesTile& tile(int index);
  
  const NesTile& tile(int index) const;
protected:
  NesTile tiles_[numTiles];
  
};


};


#endif
