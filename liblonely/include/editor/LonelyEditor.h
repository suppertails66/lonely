#ifndef LONELYEDITOR_H
#define LONELYEDITOR_H


#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "gamedata/LaylaData.h"
#include "gamedata/LaylaOffsetFile.h"
#include "editor/LonelyLevelEditor.h"
#include "editor/LonelyEditorData.h"

namespace Lonely {


class LonelyEditor {
public:
  LonelyEditor(const Tstring& romFileName,
               const Tstring& offsetFileName,
               bool rememberExportLocation__ = true);
               
  static int version();
  
  void exportToFile(const Tstring& romFileName);
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  LonelyLevelEditor& levelEditor();
  
  void saveToFile(const Tstring& filename);
  
  void loadFromFile(const Tstring& filename);
  
  bool rememberExportLocation() const;
  void setRememberExportLocation(bool rememberExportLocation__);
  
  const Tstring& lastExportLocation() const;
  void setLastExportLocation(const Tstring& lastExportLocation__);
  
  Tstring lastSavedFile() const;
  Tstring lastLoadedFile() const;
protected:
  const static int version_ = 0;

  LaylaData data_;
  
  LonelyLevelEditor levelEditor_;
  
  LonelyEditorData editorData_;
  
  Tstring lastSavedFile_;
  Tstring lastLoadedFile_;
  
};


};


#endif
