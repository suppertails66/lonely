#ifndef ARRAYOPS_H
#define ARRAYOPS_H


#include <string>

namespace Lonely {


class ArrayOps {
public:
  
  static void reverseArray(char* src, int len);
  
  static void reverseArray(unsigned char* src, int len);
  
  static int saveArray(std::string& dst,
                       const char* src,
                       int len);
  
  static int saveArray(std::string& dst,
                       const unsigned char* src,
                       int len);
  
  static int loadArray(char* dst,
                       const char* src,
                       int len);
  
  static int loadArray(unsigned char* dst,
                       const unsigned char* src,
                       int len);
  
protected:
  
};


};


#endif 
