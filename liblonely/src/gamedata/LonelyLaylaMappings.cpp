#include "gamedata/LonelyLaylaMappings.h"
#include "gamedata/LaylaPatternToken.h"
#include <iostream>

namespace Lonely {


LonelyLaylaMapping LonelyLaylaMappings::lonelyLaylaMapping() {
  return LonelyLaylaMapping(laylaMappingData_);
}
  
LonelyLaylaMapping LonelyLaylaMappings::lonelyObjectMapping(
    LonelyLaylaObjects::LonelyLaylaObject object) {
  return lonelyObjectMappings_[object];
}
  
LonelyLaylaMapping LonelyLaylaMappings::lonelyBossMapping(
    LaylaBosses::LaylaBoss boss) {
  return lonelyBossMappings_[boss];
}
    
LonelyLaylaMapping LonelyLaylaMappings::lonelyNumberMapping(
            int value,
            int numDigits) {
  LonelyLaylaMapping result;
  TwoDByteArray bytes(numDigits * 2, 1);
  // Convert to BCD, then to tile index.
  // The tiles for digits 0-9 are found in all background banks,
  // starting with 0 at tile index 1.
  for (int i = numDigits - 1; i >= 0; i--) {
    bytes.data(i * 2, 0) = (value % 10) + 1;
    bytes.data(i * 2 + 1, 0) = 0;
    value /= 10;
  }
  result.setMapping(bytes);
  return result;
}
  
Tbyte LonelyLaylaMappings::areaTypeOfObject(
    LonelyLaylaObjects::LonelyLaylaObject object) {
  switch (object) {
  case LonelyLaylaObjects::manitokha:
    return SetAreaToken::bossFlag;
    break;
  default:
    break;
  }
    
  if (object < LonelyLaylaObjects::purim) {
    return SetAreaToken::bossFlag;
  }
  else if (object < LonelyLaylaObjects::soudchil) {
    return SetAreaToken::caveID;
  }
  else {
    return SetAreaToken::baseFlag;
  }
}

const Tbyte LonelyLaylaMappings::laylaMappingData_[
    laylaMappingDataLength_] = {
  0x02, 0x03,
  0x01, 0x00, 0x11, 0x00,
  0x19, 0x00, 0x2D, 0x00,
  0x0F, 0x00, 0x0F, 0x40,
};
  
const Tbyte* LonelyLaylaMappings::lonelyObjectMappingsData_
  [numLonelyObjectMappings] =
  {
    // none
    (const Tbyte*)("\x00\x00"),
    // unk1
    (const Tbyte*)("\x00\x00"),
    // unk2
    (const Tbyte*)("\x00\x00"),
    // fallingWeight
    (const Tbyte*)("\x00\x00"),
//    (const Tbyte*)("\x02\x02"
//                   "\xEE\x03\xFE\x03"
//                   "\xEF\x03\xFF\x03"),
    // keyCard
    (const Tbyte*)("\x02\x02"
                   "\xEE\x03\xFE\x03"
                   "\xEF\x03\xFF\x03"),
    // trappedIris
    (const Tbyte*)("\x02\x03"
                   "\x26\x00\x36\x00"
                   "\x3E\x00\x2D\x00"
                   "\x0F\x00\x0F\x40"),
    // undefined
    (const Tbyte*)("\x00\x00"),
    // purim
    (const Tbyte*)("\x02\x02"
                   "\x60\x03\x60\x43"
                   "\x61\x03\x61\x43"),
    // ben
    (const Tbyte*)("\x02\x02"
                   "\x5B\x03\x5B\x43"
                   "\x5C\x03\x5C\x43"),
    // spagani
    (const Tbyte*)("\x02\x02"
                   "\x53\x02\x53\x42"
                   "\x54\x02\x54\x42"),
    // flippedSpagani
    (const Tbyte*)("\x02\x02"
                   "\x54\x82\x54\xC2"
                   "\x53\x82\x53\xC2"),
    // friendy
    (const Tbyte*)("\x02\x03"
                   "\xA7\x03\xB7\x03"
                   "\xA8\x03\xB8\x03"
                   "\xA9\x03\xB9\x03"),
    // jethro
    (const Tbyte*)("\x02\x03"
                   "\x94\x03\x90\x03"
                   "\x81\x03\x91\x03"
                   "\x83\x03\x93\x03"),
    // apara
    (const Tbyte*)("\x02\x02"
                   "\x42\x02\x52\x02"
                   "\x50\x02\x50\x42"),
    // envy
    (const Tbyte*)("\x01\x04"
                   "\xA5\x02"
                   "\xA6\x02"
                   "\xA6\xC2"
                   "\xA5\xC2"),
    // jondy
    (const Tbyte*)("\x02\x02"
                   "\x69\x02\x79\x02"
                   "\x6A\x02\x7A\x42"),
    // mirako
    (const Tbyte*)("\x02\x03"
                   "\xA0\x02\xB0\x02"
                   "\xA1\x02\xB1\x02"
                   "\xA3\x02\xB3\x02"),
    // miichin
    (const Tbyte*)("\x02\x02"
                   "\x63\x02\x73\x02"
                   "\x64\x02\x74\x02"),
    // shiilance
    (const Tbyte*)("\x03\x02"
                   "\x7D\x02\x8A\x02\x9A\x02"
                   "\x86\x02\x8B\x02\x9B\x02"),
    // koan
    (const Tbyte*)("\x02\x02"
                   "\x8D\x02\x9D\x02"
                   "\x8D\x82\x9D\x82"),
    // biroganof
    (const Tbyte*)("\x03\x02"
                   "\x7E\x03\x8E\x03\x9E\x03"
                   "\x7E\x83\x8E\x83\x9E\x83"),
    // robots
    // soudchil
    (const Tbyte*)("\x02\x02"
                   "\xA0\x03\xB0\x03"
                   "\xA0\x83\xB0\x83"),
    // pogos
    (const Tbyte*)("\x02\x02"
                   "\x86\x03\x96\x03"
                   "\x88\x03\x98\x03"),
    // vandol
    (const Tbyte*)("\x02\x04"
                   "\x65\x01\x75\x01"
                   "\x66\x01\x76\x01"
                   "\x67\x01\x77\x01"
                   "\x69\x01\x79\x01"),
    // dolgadol
    (const Tbyte*)("\x02\x04"
                   "\x60\x01\x70\x01"
                   "\x61\x01\x71\x02"
                   "\x62\x02\x72\x02"
                   "\x63\x02\x73\x02"),
    // buldora
    (const Tbyte*)("\x02\x02"
                   "\x8B\x03\x9B\x03"
                   "\x8B\x83\x9B\x83"),
    // rogin
    (const Tbyte*)("\x02\x02"
                   "\x80\x03\x80\x43"
                   "\x81\x03\x91\x03"),
    // at3
    (const Tbyte*)("\x02\x02"
                   "\xA8\x03\xB8\x03"
                   "\xAA\x03\xBA\x03"),
    // torso
    (const Tbyte*)("\x02\x03"
                   "\xBF\x03\xD7\x03"
                   "\xC8\x03\xD8\x03"
                   "\xCD\x03\xDD\x03"),
    // usumiil
    (const Tbyte*)("\x02\x03"
                   "\x6A\x03\x6A\x03"
                   "\x6A\x03\x6A\x03"
                   "\x6A\x03\x6A\x03"
                   "\x6A\x03\x6A\x03"),
    // valketta
    (const Tbyte*)("\x02\x02"
                   "\xB2\x03\xB3\x03"
                   "\xA2\x03\xA2\x43"),
    // gramzi
    (const Tbyte*)("\x02\x03"
                   "\x8C\x02\x9C\x02"
                   "\x8D\x02\x9D\x02"
                   "\x8F\x03\x9F\x03"),
    // special
    // manitokha
    (const Tbyte*)("\x02\x04"
                   "\xC7\x02\xD7\x02"
                   "\xC8\x02\xD8\x02"
                   "\xC9\x02\xD9\x02"
                   "\xCA\x02\xDA\x02")
  };

const LonelyLaylaMapping LonelyLaylaMappings::lonelyObjectMappings_
  [numLonelyObjectMappings] =
  {
    LonelyLaylaMapping(lonelyObjectMappingsData_[0]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[1]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[2]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[3]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[4]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[5]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[6]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[7]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[8]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[9]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[10]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[11]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[12]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[13]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[14]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[15]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[16]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[17]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[18]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[19]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[20]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[21]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[22]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[23]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[24]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[25]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[26]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[27]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[28]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[29]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[30]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[31]),
    LonelyLaylaMapping(lonelyObjectMappingsData_[32])
  };
  
const Tbyte* LonelyLaylaMappings::lonelyBossMappingsData_
  [numLonelyBossMappings] =
  {
    // vamou
    (const Tbyte*)("\x04\x04"
                   "\xBC\x01\xCC\x01\x65\x01\x75\x01"
                   "\xBD\x01\xCD\x01\x66\x01\x76\x01"
                   "\xBE\x01\xCE\x01\x67\x01\x77\x01"
                   "\xBF\x01\xCF\x01\x68\x01\x78\x01"),
    // esperdra
    (const Tbyte*)("\x03\x03"
                   "\x8C\x01\x9C\x01\xAC\x01"
                   "\x8D\x01\x9D\x01\xAD\x01"
                   "\x00\x01\x9E\x01\xAE\x01"),
    // batear
    (const Tbyte*)("\x04\x04"
                   "\x81\x01\x91\x01\xA1\x01\xB1\x01"
                   "\x82\x01\x92\x01\xA2\x01\xB2\x01"
                   "\x83\x01\x93\x01\xA3\x01\xB3\x01"
                   "\x84\x01\x94\x01\xA4\x01\xB4\x01"),
    // taratab
    (const Tbyte*)("\x04\x02"
                   "\x88\x01\x98\x01\x98\x41\x88\x41"
                   "\x89\x01\x99\x01\x99\x41\x89\x41"),
    // dinobares
    (const Tbyte*)("\x04\x04"
                   "\x40\x03\x50\x03\x60\x03\x00\x03"
                   "\x41\x03\x51\x03\x61\x03\x71\x03"
                   "\x42\x03\x52\x03\x62\x03\x72\x03"
                   "\xDD\x03\xDC\x03\x6A\x03\x73\x03")
  };

const LonelyLaylaMapping LonelyLaylaMappings::lonelyBossMappings_
  [numLonelyObjectMappings] =
  {
    LonelyLaylaMapping(lonelyBossMappingsData_[0]),
    LonelyLaylaMapping(lonelyBossMappingsData_[1]),
    LonelyLaylaMapping(lonelyBossMappingsData_[2]),
    LonelyLaylaMapping(lonelyBossMappingsData_[3]),
    LonelyLaylaMapping(lonelyBossMappingsData_[4])
  };



};
