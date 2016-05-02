#ifndef LONELYSOUNDCHANNEL_H
#define LONELYSOUNDCHANNEL_H


#include "gamedata/LonelySoundChannelFragment.h"
#include "structs/Taddress.h"
#include "structs/Tstring.h"

namespace Lonely {


class LonelySoundChannel {
public:
  LonelySoundChannel();
  
  Taddress activeFragmentBaseAddress() const;
  Taddress activeFragmentCurrentAddress() const;
  
  void appendToActiveFragment(LaylaSoundToken& token);
  
  void startNewFragment(Taddress baseAddress);
  
  int numFragments() const;
  Taddress fragmentAddress(int fragmentNum) const;
  bool fragmentHasData(int index) const;
  void writeFragmentToString(int fragmentNum,
                             Tstring& dst) const;
protected:
  typedef std::vector<LonelySoundChannelFragment>
      LonelySoundChannelFragmentCollection;
  
  LonelySoundChannelFragment& activeFragment();
  const LonelySoundChannelFragment& activeFragment() const;
    
  LonelySoundChannelFragmentCollection fragments_;
};


};


#endif
