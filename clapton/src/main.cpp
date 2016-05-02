#include "parser/ClaptonParser.h"
#include "util/StringConversion.h"
#include <string>
#include <iostream>

using namespace Lonely;

int main(int argc, char* argv[]) {
//  std::cout << Lonely::StringConversion::toString(4) << std::endl;
  
//  Lonely::GenericException except("nothing");

  if (argc < 3) {
    std::cout << "Clapton music compiler for Layla (NES)" << std::endl;
    std::cout << "Usage: clapton <songfile> <output> [rom] [options]"
      << std::endl;
    return 0;
  }
  
  std::string infile(argv[1]);

  ClaptonParser parser;
  parser.parseFile(infile);
  
  // Output raw data
  if (argc == 3) {
    std::string outpath(argv[2]);
    parser.writeRawData(outpath);
  }
  // Write to ROM
  else if (argc == 4) {
    std::string inrom(argv[2]);
    std::string outfile(argv[3]);
    
    parser.writeToRom(inrom, outfile);
  }
  
  return 0;
}
