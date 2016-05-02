#ifndef LONELYLEVELVIEWMANAGER_H
#define LONELYLEVELVIEWMANAGER_H


#include "structs/Tbyte.h"
#include "structs/TpointerVector.h"
#include "structs/DisplayAreaManager.h"
#include "gamedata/LaylaPatternToken.h"
#include "gamedata/LaylaArea.h"
#include <vector>

namespace Lonely {


class LonelyLevelView;

class LonelyLevelViewManager {
public:
  friend class LonelyLevelView;

  enum Tool {
    none = 0,
    patternModify,
    patternCopy,
    patternMove
  };
  
  enum PatternInsertOverwriteMode {
    pio_waitingForClick
  };
  
  enum PatternCopyMode {
    cpy_waitingForSelectClick,
    cpy_waitingForSelectRelease,
    cpy_waitingForPasteClick
  };
  
  enum PatternMoveMode {
    mov_waitingForSelectClick,
    mov_waitingForSelectRelease,
    mov_waitingForMoveClick,
    mov_waitingForMoveRelease
  };

  LonelyLevelViewManager(LonelyLevelView& levelView__);
  
  static void enterEvent(void* levelViewV);
  static void leaveEvent(void* levelViewV);
  static void mouseMoveEvent(void* levelViewV, InputEventData event);
  static void mousePressEvent(void* levelViewV, InputEventData event);
  static void mouseReleaseEvent(void* levelViewV, InputEventData event);
  static void mouseDoubleClickEvent(void* levelViewV, InputEventData event);
  
  void resetTools();
  
  void changeTool(Tool tool__);
protected:
  LonelyLevelView* levelView_;
  
  bool toolsActive_;
  bool mouseIsIn_;
  
  Tool activeTool_;
  
  // Pattern modifyClick
  bool pio_overwrite_;
  Tbyte pio_sourcePatternNum_;
  int pio_targetPatternIndex_;
  
  void handlePatternModifyClick(InputEventData event);
  
  // Pattern copy
  PatternCopyMode cpy_mode_;
  bool cpy_copyNodes_;
  LaylaPatternTokenVector cpy_copyPatterns_;
  int cpy_patternBaseIndex_;
  int cpy_patternBaseIndexReal_;
  int cpy_patternEndIndex_;
  int cpy_patternOrigIndex_;
  
  void handlePatternCopyClick(InputEventData event);
  void handlePatternCopyMove(InputEventData event);
  void handlePatternCopyRelease(InputEventData event);
  void doPatternCopyCopyClick(InputEventData event);
  
  // Pattern move
  PatternMoveMode mov_mode_;
  int mov_patternBaseIndex_;
  int mov_patternBaseIndexReal_;
  int mov_patternEndIndex_;
  int mov_patternDragIndex_;
  
  void handlePatternMoveClick(InputEventData event);
  void handlePatternMoveDoubleClick(InputEventData event);
  void handlePatternMoveMove(InputEventData event);
  void handlePatternMoveRelease(InputEventData event);
  void doPatternMoveSelectClick(InputEventData event);
  void doPatternMoveMoveClick(InputEventData event);
  void moveSelection(int target);
  
};


};


#endif
