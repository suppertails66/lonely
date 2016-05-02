#include "structs/StandardCloneTool.h"

namespace Lonely {


StandardCloneTool::StandardCloneTool()
  : mode(waitingForSelectClick),
    editableBaseX(0),
    editableBaseY(0),
    editableDragX(0),
    editableDragY(0) { };
  
StandardCloneTool::~StandardCloneTool() {
  
}


};
