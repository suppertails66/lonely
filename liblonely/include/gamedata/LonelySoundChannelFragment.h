#ifndef SOUNDCHANNELFRAGMENT_H
#define SOUNDCHANNELFRAGMENT_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tstring.h"
#include "gamedata/LaylaSoundToken.h"
#include <vector>

namespace Lonely {


class LonelySoundChannelFragment {
public:
  LonelySoundChannelFragment();
  LonelySoundChannelFragment(Taddress address__);
  
  Taddress address() const;
  void setAddress(Taddress address__);
  
  void addToken(LaylaSoundToken& token__);
  
  void clear();
  
  int totalOutputSize() const;
  
  int totalOutputSizeWithoutTerminator() const;
  
  void writeToString(Tstring& dst) const;
  
  int numTokens() const;
protected:
  Taddress address_;
  LaylaSoundTokenCollection tokens_;
};


};


#endif
