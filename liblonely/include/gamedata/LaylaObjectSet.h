#ifndef LAYLAOBJECTSET_H
#define LAYLAOBJECTSET_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tarray.h"
#include <vector>

namespace Lonely {


class LaylaObjectSet {
public:
  LaylaObjectSet();
  
  LaylaObjectSet(int objectSetNum__,
                 const Tarray<Tbyte>& bossInitTable__,
                 const Tarray<Tbyte>& objectInitTable__,
                 Taddress objectCodeBlockBaseOffset__,
                 const Tarray<Tbyte>& objectCodeBlock__);
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  int objectSetNum() const;
  const Tarray<Tbyte>& bossInitTable() const;
  const Tarray<Tbyte>& objectInitTable() const;
  Taddress objectCodeBlockBaseOffset() const;
  const Tarray<Tbyte>& objectCodeBlock() const;
protected:
  int objectSetNum_;
  Tarray<Tbyte> bossInitTable_;
  Tarray<Tbyte> objectInitTable_;
  Taddress objectCodeBlockBaseOffset_;
  Tarray<Tbyte> objectCodeBlock_;
  
};

typedef std::vector<LaylaObjectSet> LaylaObjectSetCollection;


};


#endif
