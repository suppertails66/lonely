#include "gamedata/LonelySoundChannel.h"

namespace Lonely {


LonelySoundChannel::LonelySoundChannel() {
  startNewFragment(0);
}
  
Taddress LonelySoundChannel::activeFragmentBaseAddress() const {
  return activeFragment().address();
}

Taddress LonelySoundChannel::activeFragmentCurrentAddress() const {
  return activeFragmentBaseAddress()
      + activeFragment().totalOutputSizeWithoutTerminator();
}

void LonelySoundChannel::appendToActiveFragment(LaylaSoundToken& token) {
  activeFragment().addToken(token);
}

void LonelySoundChannel::startNewFragment(Taddress baseAddress) {
  fragments_.push_back(LonelySoundChannelFragment(baseAddress));
}
  
LonelySoundChannelFragment& LonelySoundChannel::activeFragment() {
  return fragments_[fragments_.size() - 1];
}

const LonelySoundChannelFragment& LonelySoundChannel::activeFragment() const {
  return fragments_[fragments_.size() - 1];
}
  
int LonelySoundChannel::numFragments() const {
  return fragments_.size();
}

Taddress LonelySoundChannel::fragmentAddress(int fragmentNum) const {
  return fragments_[fragmentNum].address();
}

bool LonelySoundChannel::fragmentHasData(int index) const {
  if (fragments_[index].numTokens() > 0) {
    return true;
  }
  
  return false;
}

void LonelySoundChannel::writeFragmentToString(int fragmentNum,
                           Tstring& dst) const {
  dst = "";
  
  fragments_[fragmentNum].writeToString(dst);
}


};
