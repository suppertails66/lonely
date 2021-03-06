#include "structs/Tcolor.h"

namespace Lonely {


Tcolor::Tcolor()
 : a_(0),
   r_(0),
   g_(0),
   b_(0) { };

Tcolor::Tcolor(Tbyte r__,
      Tbyte g__,
      Tbyte b__,
      Tbyte a__)
  : a_(a__),
    r_(r__),
    g_(g__),
    b_(b__) { };

Tcolor::Tcolor(const Tbyte* src)
  : a_(src[3]),
    r_(src[2]),
    g_(src[1]),
    b_(src[0]) { };
        
Tbyte Tcolor::a() const {
  return a_;
}

Tbyte Tcolor::r() const {
  return r_;
}

Tbyte Tcolor::g() const {
  return g_;
}

Tbyte Tcolor::b() const {
  return b_;
}

void Tcolor::setA(Tbyte a__) {
  a_ = a__;
}

void Tcolor::setR(Tbyte r__) {
  r_ = r__;
}

void Tcolor::setG(Tbyte g__) {
  g_ = g__;
}

void Tcolor::setB(Tbyte b__) {
  b_ = b__;
}


};
