#ifndef LAYLAOFFSETFILE_H
#define LAYLAOFFSETFILE_H


#include "util/IniFile.h"
#include "structs/Tstring.h"
#include "util/ByteConversion.h"

namespace Lonely {


class LaylaOffsetFile {
public:
  LaylaOffsetFile();
  
  LaylaOffsetFile(const Tstring& filename);
  
  void loadFromFile(const Tstring& filename);
  
  IniFile& ini();
  
  const IniFile& ini() const;
protected:
  const static Tstring areaBaseLabel;
  
  IniFile ini_;

};


};


#endif
