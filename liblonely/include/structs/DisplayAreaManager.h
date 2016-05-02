#ifndef DISPLAYAREAMANAGER_H
#define DISPLAYAREAMANAGER_H


#include "structs/InputEventData.h"

namespace Lonely {


class DisplayAreaManager {
public:
  DisplayAreaManager();
  
  virtual ~DisplayAreaManager();
  
  virtual void enterEvent() =0;
  virtual void leaveEvent() =0;
  virtual void mouseMoveEvent(InputEventData event) =0;
  virtual void mousePressEvent(InputEventData event) =0;
  virtual void mouseReleaseEvent(InputEventData event) =0;
  virtual void mouseDoubleClickEvent(InputEventData event) =0;
protected:
  
};


};


#endif
