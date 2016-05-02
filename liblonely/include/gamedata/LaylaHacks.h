#ifndef LAYLAHACKS_H
#define LAYLAHACKS_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "nes/NesRom.h"
#include "nes/UxRomBanking.h"

namespace Lonely {


class LaylaHacks {
public:
  /**
   * "Properly" expands an unexpanded Layla ROM.
   * This doubles the ROM size, moves the fixed bank to the new last bank,
   * updates the bankswitching code to accomodate the changes,
   * and updates the formula for computing the bank number of a level
   * in anticipation of the new organizational system used for that purpose.
   */
  static void expandAndAdjustRom(
                  NesRom& rom);
                  
  static void applyExpandedBankingHack(
                  NesRom& rom,
                  int bankNum);
protected:
  // ==================================
  // Expanded banking hack
  // ==================================
  
  const static Taddress expBankingPatch1BaseOffset_ = 0x2ED0;
  const static int expBankingPatch1Length_ = 6;
  const static Tbyte expBankingPatch1Data_[];
  
  const static Taddress expBankingPatch2BaseOffset_ = 0x0732;
  const static int expBankingPatch2Length_ = 0x10;
  const static Tbyte expBankingPatch2Data_[];
  
  const static Taddress expBankingPatch3BaseOffset_ = 0x388E;
  const static int expBankingPatch3Length_ = 2;
  const static Tbyte expBankingPatch3Data_[];
  
  // ==================================
  // ROM expansion constants
  // ==================================
  const static int romExpNewRomSize_ = 0x40000;
  const static int romExpOrigLastBankNum_ = 0x07;
  const static Taddress romExpOrigLastBankAddress_
    = romExpOrigLastBankNum_ * UxRomBanking::sizeOfPrgBank;
  const static int romExpNewLastBankNum_ = 0x0F;
  const static Taddress romExpNewLastBankAddress_
    = romExpNewLastBankNum_ * UxRomBanking::sizeOfPrgBank;
  
};


};


#endif
