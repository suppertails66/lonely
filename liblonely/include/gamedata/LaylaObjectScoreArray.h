#ifndef LAYLAOBJECTSCOREARRAY_H
#define LAYLAOBJECTSCOREARRAY_H


#include "gamedata/LaylaObjectSets.h"
#include "structs/Tbyte.h"
#include "structs/Tstring.h"

namespace Lonely {


class LaylaObjectScoreArray {
public:
  const static int numScores = LaylaObjectSets::objectsPerSet;
  const static int numEntries = numScores;
  
  LaylaObjectScoreArray();
  
  int readFromData(const Tbyte* src);
  
  int writeToData(Tbyte* dst) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  Tbyte& score(int num);
  
  const Tbyte& score(int num) const;
protected:
  Tbyte scores_[numScores];

};


};


#endif
