#ifndef LONELYCONTROLVIEWMANAGER_H
#define LONELYCONTROLVIEWMANAGER_H


#include "structs/DisplayAreaManager.h"
#include "gamedata/LaylaArea.h"

namespace Lonely {


class LonelyLevelView;

class LonelyControlViewManager {
public:
  enum ToolMode {
    none = 0,
    waitingForNodeClick,
    waitingForNodeRelease
  };

  friend class LonelyLevelView;

  LonelyControlViewManager(LonelyLevelView& levelView__);
  
  static void enterEvent(void* levelViewV);
  static void leaveEvent(void* levelViewV);
  static void mouseMoveEvent(void* levelViewV, InputEventData event);
  static void mousePressEvent(void* levelViewV, InputEventData event);
  static void mouseReleaseEvent(void* levelViewV, InputEventData event);
  static void mouseDoubleClickEvent(void* levelViewV, InputEventData event);
  
  void setCallbacks(void* callbackObj__,
                    void (*nodeSelectedCallback__)
                      (void*,TokenIndexCollection&),
                    void (*nodeDeselectedCallback__)(void*));
  
  void selectNode(int index);
  void deselectNode();
protected:
  LonelyLevelView* levelView_;
  
  ToolMode toolMode_;
  bool nodeSelected_;
  bool cloneClick_;
  int selectedNodePatternIndex_;
  int draggedNodePatternIndex_;
  
  bool isTouchingNode(InputEventData event,
                      int targetDrawX);
  
  bool isTouchingNodeInX(InputEventData event,
                        int targetDrawX);
  
  bool isTouchingNodeInY(InputEventData event,
                        int targetDrawY);
  
  int getTargetIndex(InputEventData event);
  
  int getTargetDrawX(InputEventData event);
  
  void handleStandardClick(InputEventData event);
  
  void handleStandardMove(InputEventData event);
  
  void handleStandardRelease(InputEventData event);
  
  void* callbackObj_;
  void (*nodeSelectedCallback_)(void*,TokenIndexCollection&);
  void (*nodeDeselectedCallback_)(void*);
  
};


};


#endif
