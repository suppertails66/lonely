#ifndef CLAPTONPARSER_H
#define CLAPTONPARSER_H


#include "parser/ParseTokenCollection.h"
#include "parser/ParseStateData.h"
#include "parser/ParseToken.h"
#include "parser/SoundProgramToken.h"
#include <string>

namespace Lonely {


class ClaptonParser {
public:
  ClaptonParser();
  
  void parseFile(const std::string& filename);
  
  void parseString(std::string& src);
  
  void writeRawData(const std::string& filepath);
  
  void writeToRom(const std::string& romfilename,
                  const std::string& outputfilename);
protected:
  ParseStateData parseStateData_;
  SoundProgramToken soundProgram_;
};


};


#endif
