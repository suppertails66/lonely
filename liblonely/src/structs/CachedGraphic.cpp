#include "structs/CachedGraphic.h"

namespace Lonely {


CachedGraphic::CachedGraphic() { };
  
CachedGraphic::CachedGraphic(int w, int h)
  : graphic_(w, h) { };

Graphic& CachedGraphic::graphic() {
  return graphic_;
}

const Graphic& CachedGraphic::graphic() const {
  return graphic_;
}

Graphic& CachedGraphic::graphicAtSize(int w, int h) {
  if ((graphic_.w() != w)
      || (graphic_.h() != h)) {
    graphic_ = Graphic(w, h);
    graphic_.clearTransparentBlack();
  }
  
  return graphic_;
}


};
