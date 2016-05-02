#ifndef LAYLAMETATILESET_H
#define LAYLAMETATILESET_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tarray.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "gamedata/LaylaMetatile.h"
#include "nes/NesRom.h"
#include <vector>

namespace Lonely {


class LaylaMetatileSet {
public:
  LaylaMetatileSet();
  
  LaylaMetatileSet(const NesRom& rom,
                   int numMetatiles,
                   Taddress evenTilesTable,
                   Taddress oddTilesTable,
                   Taddress attributesTable,
                   Taddress propertiesTable);
                   
  void readFromData(const NesRom& rom,
                    int numMetatiles,
                    Taddress evenTilesTable,
                    Taddress oddTilesTable,
                    Taddress attributesTable,
                    Taddress propertiesTable);
                   
  void writeToData(NesRom& rom,
                   int maxNumMetatiles,
                   Taddress evenTilesTable,
                   Taddress oddTilesTable,
                   Taddress attributesTable,
                   Taddress propertiesTable) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  int size() const;
  
  LaylaMetatile& metatile(int index);
  
  const LaylaMetatile& metatile(int index) const;
  
  void removeMetatile(int index);
  
  int addMetatile(int index);
protected:
  typedef std::vector<LaylaMetatile> LaylaMetatileCollection;
  
  LaylaMetatileCollection metatiles_;
  
  static Tbyte fetchUpperLeftTile(const Tbyte* evenTilesTable,
                           int index);
  
  static Tbyte fetchLowerLeftTile(const Tbyte* evenTilesTable,
                           int index);
  
  static Tbyte fetchUpperRightTile(const Tbyte* oddTilesTable,
                           int index);
  
  static Tbyte fetchLowerRightTile(const Tbyte* oddTilesTable,
                           int index);
  
  static Tbyte fetchProperties(const Tbyte* propertiesTable,
                        int index);
  
  static Tbyte fetchAttribute(const Tbyte* attributesTable,
                       int index);
                       
  static void writeEvenTiles(Tbyte* evenTilesTable,
                             const LaylaMetatile& metatile,
                             int index);
                       
  static void writeOddTiles(Tbyte* oddTilesTable,
                             const LaylaMetatile& metatile,
                             int index);
                       
  static void writeProperties(Tbyte* propertiesTable,
                              const LaylaMetatile& metatile,
                              int index);
                       
  static void writeAttribute(Tbyte* attributeTable,
                             const LaylaMetatile& metatile,
                             int index);
};


};


#endif
