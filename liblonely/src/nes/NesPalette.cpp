#include "nes/NesPalette.h"
#include "exception/OutOfRangeIndexException.h"

namespace Lonely {


NesPalette::NesPalette() { };
  
int NesPalette::numColors() const {
  return numColors_;
}

NesColor NesPalette::color(int index) const {
  if ((index < 0) || (index >= numColors_)) {
    throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                   "NesPalette::color(int)",
                                   index);
  }

  return colors_[index];
}

NesColor& NesPalette::colorRef(int index) {
  if ((index < 0) || (index >= numColors_)) {
    throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                   "NesPalette::color(int)",
                                   index);
  }

  return colors_[index];
}

void NesPalette::setColor(int index, NesColor color) {
  if ((index < 0) || (index >= numColors_)) {
    throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                   "NesPalette::setColor(int,NesColor)",
                                   index);
  }
  
  colors_[index] = color;
}


};
