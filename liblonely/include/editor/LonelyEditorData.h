#ifndef LONELYEDITORDATA_H
#define LONELYEDITORDATA_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"

namespace Lonely {


class LonelyEditorData {
public:
  LonelyEditorData();
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  bool rememberExportLocation() const;
  void setRememberExportLocation(bool rememberExportLocation__);
  
  const Tstring& lastExportLocation() const;
  void setLastExportLocation(const Tstring& lastExportLocation__);
protected:
  bool rememberExportLocation_;

  Tstring lastExportLocation_;
};


};


#endif
