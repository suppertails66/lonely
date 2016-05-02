#ifndef STANDARDCLONETOOL_H
#define STANDARDCLONETOOL_H


#include "structs/TwoDArray.h"

namespace Lonely {


class StandardCloneTool {
public:
  enum Mode {
    waitingForSelectClick,
    waitingForSelectRelease,
    waitingForCloneClick
  };

  StandardCloneTool();
  
  virtual ~StandardCloneTool();
  
  Mode mode;
  
  TwoDArray<int> copyData;
  
  int editableBaseX;
  int editableBaseY;
  
  int editableDragX;
  int editableDragY;
protected:
  
};


};


#endif
