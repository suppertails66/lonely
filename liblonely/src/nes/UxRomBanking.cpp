#include "nes/UxRomBanking.h"

namespace Lonely {


int UxRomBanking::prgBankSize() {
  return prgBankSize_;
}
  
Taddress UxRomBanking::directToBankedAddressMovable(
    Taddress directAddress) {
  return (directAddress % prgBankSize_) + movableBankOffset;
}

Taddress UxRomBanking::directToBankNumMovable(
    Taddress directAddress) {
  return (directAddress / prgBankSize_);
}

Taddress UxRomBanking::bankedToDirectAddressMovable(
    int bankNum, Taddress bankedAddress) {
  return (bankedAddress - movableBankOffset)
      + (bankNum * prgBankSize_);
}

Taddress UxRomBanking::directToBankedAddressFixed(
    Taddress directAddress) {
  return (directAddress % prgBankSize_) + fixedBankOffset;
}

Taddress UxRomBanking::bankedToDirectAddressFixed(
    int bankNum, Taddress bankedAddress) {
  return (bankedAddress - fixedBankOffset)
      + (bankNum * prgBankSize_);
}


};
