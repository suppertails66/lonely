#include "gamedata/LaylaObjectScoreArray.h"
#include <cstring>

namespace Lonely {


LaylaObjectScoreArray::LaylaObjectScoreArray() {
  std::memset(scores_,
              0,
              numScores);
}
int LaylaObjectScoreArray::readFromData(const Tbyte* src) {
  std::memcpy(scores_,
              src,
              numScores);
  return numScores;
}
  
int LaylaObjectScoreArray::writeToData(Tbyte* dst) const {
  std::memcpy(dst,
              scores_,
              numScores);
  return numScores;
}
  
int LaylaObjectScoreArray::save(Tstring& data) const {
  Tbyte buffer[numScores];
  writeToData(buffer);
  data += Tstring((char*)buffer, numScores);
  return numScores;
}

int LaylaObjectScoreArray::load(const Tbyte* data) {
  readFromData(data);
  return numScores;
}

Tbyte& LaylaObjectScoreArray::score(int num) {
  return scores_[num];
}

const Tbyte& LaylaObjectScoreArray::score(int num) const {
  return scores_[num];
}


};
