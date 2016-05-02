#include "gamedata/LaylaHpArray.h"
#include "util/ArrayOps.h"
#include <cstring>

namespace Lonely {


LaylaHpArray::LaylaHpArray() {
  std::memset(hps_,
              0,
              numEntries);
}
  
int LaylaHpArray::readFromData(const Tbyte* src) {
  std::memcpy(hps_,
              src,
              numEntries);
  return numEntries;
}
  
int LaylaHpArray::writeToData(Tbyte* dst) const {
  std::memcpy(dst,
              hps_,
              numEntries);
  return numEntries;
}
  
int LaylaHpArray::save(Tstring& dst) const {
  return ArrayOps::saveArray(dst,
                      hps_,
                      numEntries);
}

int LaylaHpArray::load(const Tbyte* src) {
  return ArrayOps::loadArray(hps_,
                      src,
                      numEntries);
}
  
Tbyte& LaylaHpArray::hp(int num) {
  return hps_[num];
}

const Tbyte& LaylaHpArray::hp(int num) const {
  return hps_[num];
}


};
