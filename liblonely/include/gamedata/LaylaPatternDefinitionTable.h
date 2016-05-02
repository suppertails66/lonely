#ifndef LAYLAPATTERNDEFINITIONTABLE_H
#define LAYLAPATTERNDEFINITIONTABLE_H


#include "gamedata/LaylaPattern.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "nes/NesRom.h"
#include <vector>

namespace Lonely {


class LaylaPatternDefinitionTable {
public:
  LaylaPatternDefinitionTable();
  
  LaylaPatternDefinitionTable(const NesRom& src,
                              int numEntries,
                              Taddress startAddress);
  
  void readFromData(const NesRom& src,
                    int numEntries,
                    Taddress startAddress);
  
  void writeToData(NesRom& dst,
                   Taddress mainBlockAddress,
                   int mainBlockLength,
                   Taddress& supplementaryBlockAddress,
                   int& supplementaryBlockLength) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  int numPatterns() const;
  
  LaylaPattern& pattern(int index);
  
  void insertPattern(int index);
  
  void removePattern(int index);
protected:
  typedef std::vector<LaylaPattern> PatternCollection;
  
  PatternCollection patterns_;
};


};


#endif
