#include "editor/LonelyEditor.h"
#include "util/FileManip.h"
#include <fstream>

namespace Lonely {


LonelyEditor::LonelyEditor(const Tstring& romFileName,
             const Tstring& offsetFileName,
             bool rememberExportLocation__)
  : data_(romFileName,
          offsetFileName),
    levelEditor_(data_.levels(),
                 data_.graphics(),
                 data_.palettes(),
                 data_.staticMetatiles(),
                 data_.objectSets()) {
  setRememberExportLocation(rememberExportLocation__);
}
               
int LonelyEditor::version() {
  return version_;
}
             
void LonelyEditor::exportToFile(const Tstring& romFileName) {
  data_.exportToFile(romFileName);
  
  if (rememberExportLocation()) {
    setLastExportLocation(romFileName);
  }
}
  
int LonelyEditor::save(Tstring& data) const {
  SaveHelper saver(data,
                   DataChunkIDs::LonelyEditor,
                   version_);
  
  editorData_.save(data);
  
  data_.save(data);
  
  return saver.finalize();
}

int LonelyEditor::load(const Tbyte* data) {
  int byteCount = 0;
  LoadHelper loader(data,
                    byteCount);

  byteCount += editorData_.load(data + byteCount);

  byteCount += data_.load(data + byteCount);
  
  return byteCount;
}
  
LonelyLevelEditor& LonelyEditor::levelEditor() {
  return levelEditor_;
}
  
void LonelyEditor::saveToFile(const Tstring& filename) {
  Tstring savedata;
  save(savedata);
  
  std::ofstream ofs(filename,
                    std::ios_base::binary
                      | std::ios_base::trunc);
  
  ofs.write(savedata.c_str(),
            savedata.size());
  
  lastSavedFile_ = filename;
}
  
void LonelyEditor::loadFromFile(const Tstring& filename) {
  if (!FileManip::fileExists(filename)) {
    return;
  }

  std::ifstream ifs(filename,
                    std::ios_base::binary);
  
  int filesize = FileManip::getFileSize(ifs);
  
  Tbyte* loaddata = new Tbyte[filesize];
  
  ifs.read((char*)loaddata, filesize);
  load(loaddata);
  
  delete loaddata;
  
  lastLoadedFile_ = filename;
  lastSavedFile_ = filename;
}
  
bool LonelyEditor::rememberExportLocation() const {
  return editorData_.rememberExportLocation();
}

void LonelyEditor
  ::setRememberExportLocation(bool rememberExportLocation__) {
  editorData_.setRememberExportLocation(rememberExportLocation__);
}
  
const Tstring& LonelyEditor::lastExportLocation() const {
  return editorData_.lastExportLocation();
}

void LonelyEditor
  ::setLastExportLocation(const Tstring& lastExportLocation__) {
  editorData_.setLastExportLocation(lastExportLocation__);
}
  
Tstring LonelyEditor
  ::lastSavedFile() const {
  return lastSavedFile_;
}

Tstring LonelyEditor
  ::lastLoadedFile() const {
  return lastLoadedFile_;
}


};
