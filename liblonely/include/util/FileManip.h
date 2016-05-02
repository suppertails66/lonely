#ifndef FILEMANIP_H
#define FILEMANIP_H


#include <string>
#include <fstream>

namespace Lonely {


class FileManip {
public:
  static unsigned int fileExists(const std::string& filename);
  
  static unsigned int getFileSize(std::ifstream& ifs);
protected:
  
};


};


#endif
