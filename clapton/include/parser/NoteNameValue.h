#ifndef NOTENAMEVALUE_H
#define NOTENAMEVALUE_H


#include <string>

namespace Lonely {


class NoteNameValues {
public:
  const static int numValues = 12;
  
  const static int restValue = 0x7F;
  
  enum NoteNameValue {
    c = 0,
    cSharp,
    d,
    dSharp,
    e,
    f,
    fSharp,
    g,
    gSharp,
    a,
    aSharp,
    b
  };
  
  static NoteNameValue baseValue(char noteLetter,
                                 bool sharp,
                                 bool flat);
protected:
};


};


#endif
