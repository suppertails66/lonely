#include "gamedata/LaylaObjectSet.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"

namespace Lonely {


LaylaObjectSet::LaylaObjectSet() { };
  
LaylaObjectSet::LaylaObjectSet(int objectSetNum__,
               const Tarray<Tbyte>& bossInitTable__,
               const Tarray<Tbyte>& objectInitTable__,
               Taddress objectCodeBlockBaseOffset__,
               const Tarray<Tbyte>& objectCodeBlock__)
  : objectSetNum_(objectSetNum__),
    bossInitTable_(bossInitTable__),
    objectInitTable_(objectInitTable__),
    objectCodeBlockBaseOffset_(objectCodeBlockBaseOffset__),
    objectCodeBlock_(objectCodeBlock__) { };

int LaylaObjectSet::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LaylaObjectSet,
                   0);
  
  SaveHelper::saveInt(data, objectSetNum_,
                      ByteSizes::uint8Size);
  
  bossInitTable_.save(data);
  
  objectInitTable_.save(data);
  
  SaveHelper::saveInt(data, objectCodeBlockBaseOffset_,
                      ByteSizes::uint32Size);
  
  objectCodeBlock_.save(data);
  
  return saver.finalize();
}

int LaylaObjectSet::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);
  
  objectSetNum_ = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint8Size);
  
  byteCount += bossInitTable_.load(data + byteCount);
  
  byteCount += objectInitTable_.load(data + byteCount);
  
  objectCodeBlockBaseOffset_ = LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size);
  
  byteCount += objectCodeBlock_.load(data + byteCount);
  
  return byteCount;
}
  
int LaylaObjectSet::objectSetNum() const {
  return objectSetNum_;
}

const Tarray<Tbyte>& LaylaObjectSet::bossInitTable() const {
  return bossInitTable_;
}

const Tarray<Tbyte>& LaylaObjectSet::objectInitTable() const {
  return objectInitTable_;
}

Taddress LaylaObjectSet::objectCodeBlockBaseOffset() const {
  return objectCodeBlockBaseOffset_;
}

const Tarray<Tbyte>& LaylaObjectSet::objectCodeBlock() const {
  return objectCodeBlock_;
}


};
