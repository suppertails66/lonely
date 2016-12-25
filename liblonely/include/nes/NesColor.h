#ifndef NESCOLOR_H
#define NESCOLOR_H


#include "structs/Tbyte.h"
#include "structs/Tcolor.h"

namespace Lonely {


/**
 * An NES hue/value format color.
 */
class NesColor {
public:
  const static int numNativeColors();

  NesColor();
  
  NesColor(Tbyte nativeValue__);
  
  bool operator==(const NesColor& c) const;
  
  static Tbyte maxNativeValue();
  
  Tbyte nativeValue() const;
  void setNativeValue(Tbyte nativeValue__);
  
  Tcolor realColor() const;
  Tcolor realColorTCRF() const;
  Tcolor realColorBisquit() const;
  
protected:
  const static int standardBlack_ = 0x0F;

  const static int numNativeColors_ = 64;
  const static int nativeColorMask_ = 0x3F;
  const static Tcolor nativeToRealColor_[numNativeColors_];
  const static Tcolor nativeToRealColorTCRF_[numNativeColors_];
  const static Tcolor nativeToRealColorBisquit_[numNativeColors_];
  
  static Tbyte clipNativeValue(Tbyte value);

  Tbyte nativeValue_;
  
  
};

bool operator<(const NesColor& first, const NesColor& second);

class NesColorHash {
public:

  int operator()(const NesColor& c) const;  
  
};


};


#endif
