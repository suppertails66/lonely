#include "gamedata/LaylaOffsetFile.h"
#include "util/StringConversion.h"
#include <iostream>

namespace Lonely {


const Tstring areaBaseLabel("Area");
  
LaylaOffsetFile::LaylaOffsetFile() { };

LaylaOffsetFile::LaylaOffsetFile(const Tstring& filename) {
  loadFromFile(filename);
}

void LaylaOffsetFile::loadFromFile(const Tstring& filename) {
//  IniFile ini(filename);
  ini_ = IniFile(filename);
  
//  Tstring str = ini.valueOfKey("Area1", "Offset");
  
//  int value = StringConversion::stringToInt(str);
}
  
IniFile& LaylaOffsetFile::ini() {
  return ini_;
}
  
const IniFile& LaylaOffsetFile::ini() const {
  return ini_;
}


};
