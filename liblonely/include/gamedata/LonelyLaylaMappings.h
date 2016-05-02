#ifndef LONELYLAYLAMAPPINGS_H
#define LONELYLAYLAMAPPINGS_H


#include "structs/Tbyte.h"
#include "gamedata/LonelyLaylaObject.h"
#include "gamedata/LonelyLaylaMapping.h"
#include "gamedata/LaylaBoss.h"

namespace Lonely {


class LonelyLaylaMappings {
public:
  static LonelyLaylaMapping lonelyLaylaMapping();
  
  static LonelyLaylaMapping lonelyObjectMapping(
    LonelyLaylaObjects::LonelyLaylaObject object);
  
  static LonelyLaylaMapping lonelyBossMapping(
    LaylaBosses::LaylaBoss boss);
    
  static LonelyLaylaMapping lonelyNumberMapping(
              int value,
              int numDigits);
  
  static Tbyte areaTypeOfObject(
      LonelyLaylaObjects::LonelyLaylaObject object);
protected:
  const static int laylaMappingDataLength_ = 14;
  const static Tbyte laylaMappingData_[];

  const static int numLonelyObjectMappings
    = (int)(LonelyLaylaObjects::manitokha) + 1;
  const static Tbyte* lonelyObjectMappingsData_[];
  const static LonelyLaylaMapping lonelyObjectMappings_[];
  
  const static int numLonelyBossMappings
    = (int)(LaylaBosses::dinobares) + 1;
  const static Tbyte* lonelyBossMappingsData_[];
  const static LonelyLaylaMapping lonelyBossMappings_[];
  
};


};


#endif
