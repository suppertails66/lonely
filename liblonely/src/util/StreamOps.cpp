#include "util/StreamOps.h"

namespace Lonely {


std::streampos StreamOps::getStreamSize(std::istream& stream) {
  std::streampos startPos = stream.tellg();
  stream.seekg(0, std::ios_base::end);
  std::streampos size = stream.tellg();
  stream.seekg(startPos);
  
  return size;
}


}; 
