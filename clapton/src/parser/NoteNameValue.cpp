#include "parser/NoteNameValue.h"
#include <cctype>


namespace Lonely {


NoteNameValues::NoteNameValue
    NoteNameValues::baseValue(char noteLetter,
                          bool sharp,
                          bool flat) {
  NoteNameValues::NoteNameValue base = a;
  switch (tolower(noteLetter)) {
  case 'a':
    base = a;
    break;
  case 'b':
    base = b;
    break;
  case 'c':
    base = c;
    break;
  case 'd':
    base = d;
    break;
  case 'e':
    base = e;
    break;
  case 'f':
    base = f;
    break;
  case 'g':
    base = g;
    break;
  default:
    break;
  }
  
  if (sharp) {
    base = static_cast<NoteNameValues::NoteNameValue>(
            static_cast<int>(base) + 1);
  }
  
  if (flat) {
    base = static_cast<NoteNameValues::NoteNameValue>(
            static_cast<int>(base) - 1);
  }
  
  if (base < 0) {
    base = static_cast<NoteNameValues::NoteNameValue>(numValues - 1);
  }
  else if (base >= numValues) {
    base = static_cast<NoteNameValues::NoteNameValue>(0);
  }
  
  return base;
}


};
