#include "util/ArrayOps.h"
#include <cstring>
#include <exception>

namespace Lonely {


void ArrayOps::reverseArray(char* src, int len) {
  char temp;
  
  char* f = src;
  char* l = src + len - 1;
  
  while (f < l) {
    temp = *f;
    *(f++) = *l;
    *(l--) = temp;
  }
}

void ArrayOps::reverseArray(unsigned char* src, int len) {
  reverseArray((char*)src, len);
}
  
int ArrayOps::saveArray(std::string& dst,
                     const char* src,
                     int len) {
  char* buffer = new char[len];
  try {
    std::memcpy(buffer,
                src,
                len);
    dst += std::string((char*)(buffer), len);
  }
  catch (std::exception& e) {
    delete buffer;
    throw;
  }
  
  delete buffer;
  return len;
}
  
int ArrayOps::saveArray(std::string& dst,
                     const unsigned char* src,
                     int len) {
  return saveArray(dst,
                   (const char*)src,
                   len);
}
  
int ArrayOps::loadArray(char* dst,
                     const char* src,
                     int len) {
  std::memcpy(dst,
              src,
              len);
  return len;
}
  
int ArrayOps::loadArray(unsigned char* dst,
                     const unsigned char* src,
                     int len) {
  return loadArray((char*)dst,
                   (const char*)src,
                   len);
}


};
