#ifndef CACHEDGRAPHIC_H
#define CACHEDGRAPHIC_H


#include "structs/Graphic.h"

namespace Lonely {


class CachedGraphic {
public:
  CachedGraphic();
  
  CachedGraphic(int w, int h);
  
  Graphic& graphic();
  
  const Graphic& graphic() const;
  
  Graphic& graphicAtSize(int w, int h);
protected:
  Graphic graphic_;
};


};


#endif
