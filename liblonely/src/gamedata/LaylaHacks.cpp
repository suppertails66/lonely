#include "gamedata/LaylaHacks.h"
#include "nes/UxRomBanking.h"
#include <cstring>

namespace Lonely {


void LaylaHacks::expandAndAdjustRom(NesRom& rom) {
  rom.changeSize(romExpNewRomSize_);
  
  // Copy fixed bank to end of ROM
  std::memcpy(rom.directWrite(romExpNewLastBankAddress_),
              rom.directRead(romExpOrigLastBankAddress_),
              UxRomBanking::sizeOfPrgBank);
  
  // Apply expanded banking hack
  applyExpandedBankingHack(rom, romExpNewLastBankNum_);
}

void LaylaHacks::applyExpandedBankingHack(
                NesRom& rom,
                int bankNum) {
  Taddress baseAddress = UxRomBanking::bankedToDirectAddressFixed(
                            bankNum, UxRomBanking::fixedBankOffset);
  
  std::memcpy(rom.directWrite(baseAddress + expBankingPatch1BaseOffset_),
              expBankingPatch1Data_,
              expBankingPatch1Length_);
  
  std::memcpy(rom.directWrite(baseAddress + expBankingPatch2BaseOffset_),
              expBankingPatch2Data_,
              expBankingPatch2Length_);
  
  std::memcpy(rom.directWrite(baseAddress + expBankingPatch3BaseOffset_),
              expBankingPatch3Data_,
              expBankingPatch3Length_);
}

const Tbyte LaylaHacks::expBankingPatch1Data_[
    expBankingPatch1Length_] =
  {
    // new formula for computing bank number from level number
    0x18,                 // CLC
    0x69, 0x03,           // ADC #$03   ; add 3 to area num to get bank num
    0xEA, 0xEA, 0xEA      // NOP NOP NOP; overwrite unneeded code
  };

const Tbyte LaylaHacks::expBankingPatch2Data_[
    expBankingPatch2Length_] =
  {
    // Expanded bank change table -- writes to each position load that bank
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
  };

const Tbyte LaylaHacks::expBankingPatch3Data_[
    expBankingPatch3Length_] =
  {
    // Address of the bank change table in patch 2
    0x32, 0xC7
  };


};
