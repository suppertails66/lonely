#include "editor/LonelyEditorData.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "editor/LonelyEditor.h"
#include <iostream>

namespace Lonely {


LonelyEditorData::LonelyEditorData()
  : rememberExportLocation_(true) { };
  
int LonelyEditorData::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LonelyEditorData,
                   0);
  
  int total = 0;
  if (rememberExportLocation_) {
    SaveHelper exportLocationSaver(data,
                     DataChunkIDs::LastExportString,
                     0);
    
    SaveHelper::saveString(data, lastExportLocation_);
    
    total += exportLocationSaver.finalize();
  }
  
  return total + saver.finalize();
}
  
int LonelyEditorData::load(const Tbyte* data) {
  // Make sure we don't retain the last export location
  // from a previous project
  lastExportLocation_ = "";

  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);

  while (byteCount < loader.fullSize()) {
    LoadHelper subloader(data + byteCount,
                         byteCount);
    
    switch (subloader.id()) {
    case DataChunkIDs::LastExportString:
      byteCount += LoadHelper::loadString(data + byteCount,
                                          lastExportLocation_);
      break;
    default:
      byteCount += subloader.chunkSize();
      break;
    }
  }
//  byteCount += data_.load(data + byteCount);
  
  return byteCount;
}
  
bool LonelyEditorData::rememberExportLocation() const {
  return rememberExportLocation_;
}

void LonelyEditorData
  ::setRememberExportLocation(bool rememberExportLocation__) {
  rememberExportLocation_ = rememberExportLocation__;
}
  
const Tstring& LonelyEditorData::lastExportLocation() const {
  return lastExportLocation_;
}

void LonelyEditorData
  ::setLastExportLocation(const Tstring& lastExportLocation__) {
  lastExportLocation_ = lastExportLocation__;
}


};
