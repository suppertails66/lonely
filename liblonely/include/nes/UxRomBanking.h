#ifndef UXROMBANKING_H
#define UXROMBANKING_H


#include "structs/Taddress.h"

namespace Lonely {


class UxRomBanking {
public:
  const static int movableBankOffset = 0x8000;
  const static int fixedBankOffset = 0xC000;
  const static int sizeOfPrgBank = 0x4000;
  
  static int prgBankSize();
  
  static Taddress directToBankedAddressMovable(
      Taddress directAddress);
  static Taddress directToBankNumMovable(
      Taddress directAddress);
  static Taddress bankedToDirectAddressMovable(
      int bankNum, Taddress bankedAddress);
  
  static Taddress directToBankedAddressFixed(
      Taddress directAddress);
  static Taddress bankedToDirectAddressFixed(
      int bankNum, Taddress bankedAddress);
protected:
  const static int prgBankSize_ = sizeOfPrgBank;
};


};


#endif
