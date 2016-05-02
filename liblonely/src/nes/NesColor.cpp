#include "nes/NesColor.h"

namespace Lonely {


NesColor::NesColor()
  : nativeValue_(standardBlack_) { };

NesColor::NesColor(Tbyte nativeValue__)
  : nativeValue_(clipNativeValue(nativeValue__)) { };
  
const int NesColor::numNativeColors() {
  return numNativeColors_;
}
  
Tbyte NesColor::maxNativeValue() {
  return numNativeColors_;
}

Tbyte NesColor::nativeValue() const {
  return nativeValue_;
}

void NesColor::setNativeValue(Tbyte nativeValue__) {
  nativeValue_ = clipNativeValue(nativeValue__);
}

Tcolor NesColor::realColor() const {
  return nativeToRealColor_[nativeValue_];
}

Tcolor NesColor::realColorTCRF() const {
  return nativeToRealColorTCRF_[nativeValue_];
}

Tcolor NesColor::realColorBisquit() const {
  return nativeToRealColorBisquit_[nativeValue_];
}
  
Tbyte NesColor::clipNativeValue(Tbyte value) {
  return (value & nativeColorMask_);
}
  
const Tcolor NesColor::nativeToRealColor_[numNativeColors_] =
  // FCEUX default palette
  {
    Tcolor(116, 116, 116), 
    Tcolor(36, 24, 140), 
    Tcolor(0, 0, 168), 
    Tcolor(68, 0, 156), 
    Tcolor(140, 0, 116), 
    Tcolor(168, 0, 16), 
    Tcolor(164, 0, 0), 
    Tcolor(124, 8, 0), 
    Tcolor(64, 44, 0), 
    Tcolor(0, 68, 0), 
    Tcolor(0, 80, 0), 
    Tcolor(0, 60, 20), 
    Tcolor(24, 60, 92), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(188, 188, 188), 
    Tcolor(0, 112, 236), 
    Tcolor(32, 56, 236), 
    Tcolor(128, 0, 240), 
    Tcolor(188, 0, 188), 
    Tcolor(228, 0, 88), 
    Tcolor(216, 40, 0), 
    Tcolor(200, 76, 12), 
    Tcolor(136, 112, 0), 
    Tcolor(0, 148, 0), 
    Tcolor(0, 168, 0), 
    Tcolor(0, 144, 56), 
    Tcolor(0, 128, 136), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(252, 252, 252), 
    Tcolor(60, 188, 252), 
    Tcolor(92, 148, 252), 
    Tcolor(204, 136, 252), 
    Tcolor(244, 120, 252), 
    Tcolor(252, 116, 180), 
    Tcolor(252, 116, 96), 
    Tcolor(252, 152, 56), 
    Tcolor(240, 188, 60), 
    Tcolor(128, 208, 16), 
    Tcolor(76, 220, 72), 
    Tcolor(88, 248, 152), 
    Tcolor(0, 232, 216), 
    Tcolor(120, 120, 120), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(252, 252, 252), 
    Tcolor(168, 228, 252), 
    Tcolor(196, 212, 252), 
    Tcolor(212, 200, 252), 
    Tcolor(252, 196, 252), 
    Tcolor(252, 196, 216), 
    Tcolor(252, 188, 176), 
    Tcolor(252, 216, 168), 
    Tcolor(252, 228, 160), 
    Tcolor(224, 252, 160), 
    Tcolor(168, 240, 188), 
    Tcolor(176, 252, 204), 
    Tcolor(156, 252, 240), 
    Tcolor(196, 196, 196), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0)
  };
  
const Tcolor NesColor::nativeToRealColorTCRF_[numNativeColors_] =
  // "TCRF" palette
  {
    Tcolor(82, 82, 82), 
    Tcolor(1, 26, 81), 
    Tcolor(15, 15, 101), 
    Tcolor(35, 6, 99), 
    Tcolor(54, 3, 75), 
    Tcolor(64, 4, 38), 
    Tcolor(63, 9, 4), 
    Tcolor(50, 19, 0), 
    Tcolor(31, 32, 0), 
    Tcolor(11, 42, 0), 
    Tcolor(0, 47, 0), 
    Tcolor(0, 46, 10), 
    Tcolor(0, 38, 45), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(160, 160, 160), 
    Tcolor(30, 74, 157), 
    Tcolor(56, 55, 188), 
    Tcolor(88, 40, 184), 
    Tcolor(117, 33, 148), 
    Tcolor(132, 35, 92), 
    Tcolor(130, 46, 36), 
    Tcolor(111, 63, 0), 
    Tcolor(81, 82, 0), 
    Tcolor(49, 99, 0), 
    Tcolor(26, 107, 5), 
    Tcolor(14, 105, 46), 
    Tcolor(16, 92, 104), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(254, 255, 255), 
    Tcolor(105, 158, 252), 
    Tcolor(137, 135, 255), 
    Tcolor(174, 118, 255), 
    Tcolor(206, 109, 241), 
    Tcolor(224, 112, 178), 
    Tcolor(222, 124, 112), 
    Tcolor(200, 145, 62), 
    Tcolor(166, 167, 37), 
    Tcolor(129, 186, 40), 
    Tcolor(99, 196, 70), 
    Tcolor(84, 193, 125), 
    Tcolor(86, 179, 192), 
    Tcolor(60, 60, 60), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(254, 255, 255), 
    Tcolor(190, 214, 253), 
    Tcolor(204, 204, 255), 
    Tcolor(221, 196, 255), 
    Tcolor(234, 192, 249), 
    Tcolor(242, 193, 223), 
    Tcolor(241, 199, 194), 
    Tcolor(232, 208, 170), 
    Tcolor(217, 218, 157), 
    Tcolor(201, 226, 158), 
    Tcolor(188, 230, 174), 
    Tcolor(180, 229, 199), 
    Tcolor(181, 223, 228), 
    Tcolor(169, 169, 169), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0)
  };

const Tcolor NesColor::nativeToRealColorBisquit_[numNativeColors_] =
  // Color values generated by Bisqwit's NES color generator
  {
    Tcolor(82, 82, 82), 
    Tcolor(1, 26, 81), 
    Tcolor(15, 15, 101), 
    Tcolor(35, 6, 99), 
    Tcolor(54, 3, 75), 
    Tcolor(64, 4, 38), 
    Tcolor(63, 9, 4), 
    Tcolor(50, 19, 0), 
    Tcolor(31, 32, 0), 
    Tcolor(11, 42, 0), 
    Tcolor(0, 47, 0), 
    Tcolor(0, 46, 10), 
    Tcolor(0, 38, 45), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(160, 160, 160), 
    Tcolor(30, 74, 157), 
    Tcolor(56, 55, 188), 
    Tcolor(88, 40, 184), 
    Tcolor(117, 33, 148), 
    Tcolor(132, 35, 92), 
    Tcolor(130, 46, 36), 
    Tcolor(111, 63, 0), 
    Tcolor(81, 82, 0), 
    Tcolor(49, 99, 0), 
    Tcolor(26, 107, 5), 
    Tcolor(14, 105, 46), 
    Tcolor(16, 92, 104), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(254, 255, 255), 
    Tcolor(105, 158, 252), 
    Tcolor(137, 135, 255), 
    Tcolor(174, 118, 255), 
    Tcolor(206, 109, 241), 
    Tcolor(224, 112, 178), 
    Tcolor(222, 124, 112), 
    Tcolor(200, 145, 62), 
    Tcolor(166, 167, 37), 
    Tcolor(129, 186, 40), 
    Tcolor(99, 196, 70), 
    Tcolor(84, 193, 125), 
    Tcolor(86, 179, 192), 
    Tcolor(60, 60, 60), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0), 
    Tcolor(254, 255, 255), 
    Tcolor(190, 214, 253), 
    Tcolor(204, 204, 255), 
    Tcolor(221, 196, 255), 
    Tcolor(234, 192, 249), 
    Tcolor(242, 193, 223), 
    Tcolor(241, 199, 194), 
    Tcolor(232, 208, 170), 
    Tcolor(217, 218, 157), 
    Tcolor(201, 226, 158), 
    Tcolor(188, 230, 174), 
    Tcolor(180, 229, 199), 
    Tcolor(181, 223, 228), 
    Tcolor(169, 169, 169), 
    Tcolor(0, 0, 0), 
    Tcolor(0, 0, 0)
  };
  
  // Table values taken from nesdev who took them from blargg who
  // made them up or something
/*  {
    Tcolor(84, 84, 84, Tcolor::fullAlphaOpacity),
    Tcolor(0, 30, 116, Tcolor::fullAlphaOpacity),
    Tcolor(8, 16, 144, Tcolor::fullAlphaOpacity),
    Tcolor(48, 0, 136, Tcolor::fullAlphaOpacity),
    Tcolor(68, 0, 100, Tcolor::fullAlphaOpacity),
    Tcolor(92, 0, 48, Tcolor::fullAlphaOpacity),
    Tcolor(0, 48, 84, Tcolor::fullAlphaOpacity),
    Tcolor(84, 4, 0, Tcolor::fullAlphaOpacity),
    Tcolor(60, 24, 0, Tcolor::fullAlphaOpacity),
    Tcolor(32, 42, 0, Tcolor::fullAlphaOpacity),
    Tcolor(8, 58, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 64, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 60, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 50, 60, Tcolor::fullAlphaOpacity),
    Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity),
    
    Tcolor(152, 150, 152, Tcolor::fullAlphaOpacity),
    Tcolor(8, 76, 116, Tcolor::fullAlphaOpacity),
    Tcolor(48, 50, 144, Tcolor::fullAlphaOpacity),
    Tcolor(92, 30, 136, Tcolor::fullAlphaOpacity),
    Tcolor(136, 20, 100, Tcolor::fullAlphaOpacity),
    Tcolor(160, 20, 48, Tcolor::fullAlphaOpacity),
    Tcolor(152, 34, 84, Tcolor::fullAlphaOpacity),
    Tcolor(120, 60, 0, Tcolor::fullAlphaOpacity),
    Tcolor(84, 90, 0, Tcolor::fullAlphaOpacity),
    Tcolor(40, 114, 0, Tcolor::fullAlphaOpacity),
    Tcolor(8, 124, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 118, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 102, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity),
    Tcolor(0, 0, 0, Tcolor::fullAlphaOpacity),
  }; */


};
