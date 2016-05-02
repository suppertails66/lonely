#ifndef NESROM_H
#define NESROM_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tstring.h"
#include "structs/Tarray.h"

namespace Lonely {


class NesRom {
public:
  enum MapperType {
    mapperUxRom = 0x02
  };
  
  enum NametableArrangementFlag {
    nametablesVertical   = 0x00,
    nametablesHorizontal = 0x01
  };

  NesRom(const Tstring& filename);
  
  void exportToFile(const Tstring& filename,
                    int numPrgBanks,
                    int numChrBanks,
                    NametableArrangementFlag nametablesFlag,
                    MapperType mapperNum);
  
  const Tbyte* directRead(Taddress address) const;
  Tbyte* directWrite(Taddress address);
  
  MapperType mapperType() const;
  void setMapperType(MapperType mapperType__);
  
  int size() const;
  
  void changeSize(int newSize);
protected:
  const static int minFileSize_ = 16;
  
  const static int inesHeaderOffset_ = 0;
  const static int inesHeaderSize_ = 16;
  const static int inesIdentifierSize_ = 4;
  const static Tbyte inesIdentifier[];
  
  MapperType mapperType_;

  void readFromFile(const Tstring& filename);

  Tarray<Tbyte> rom_;
};


};


#endif
