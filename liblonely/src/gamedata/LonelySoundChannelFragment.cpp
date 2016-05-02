#include "gamedata/LonelySoundChannelFragment.h"
#include "structs/Tarray.h"

namespace Lonely {


LonelySoundChannelFragment::LonelySoundChannelFragment()
  : address_(0) { };
LonelySoundChannelFragment::LonelySoundChannelFragment(Taddress address__)
  : address_(address__) { };
  
Taddress LonelySoundChannelFragment::address() const {
  return address_;
}

void LonelySoundChannelFragment::setAddress(Taddress address__) {
  address_ = address__;
}
  
void LonelySoundChannelFragment::addToken(LaylaSoundToken& token__) {
  tokens_.push_back(&token__);
}
  
void LonelySoundChannelFragment::clear() {
  tokens_.deleteData();
  tokens_.clear();
}
  
int LonelySoundChannelFragment::totalOutputSize() const {
  int total = 1;
  for (int i = 0; i < tokens_.size(); i++) {
    total += tokens_[i]->size();
  }
  return total;
}
  
int LonelySoundChannelFragment::totalOutputSizeWithoutTerminator() const {
  return totalOutputSize() - 1;
}
  
void LonelySoundChannelFragment::writeToString(Tstring& dst) const {
  Tarray<Tbyte> data;
  data.resize(totalOutputSize());
  
  int pos = 0;
  for (int i = 0; i < tokens_.size(); i++) {
    pos += tokens_[i]->writeToData(data.data() + pos);
  }
  
  // Data terminator
  data[data.size() - 1] = 0;
  
  dst = Tstring((char*)(data.data()), data.size());
}
  
int LonelySoundChannelFragment::numTokens() const {
  return tokens_.size();
}


};
