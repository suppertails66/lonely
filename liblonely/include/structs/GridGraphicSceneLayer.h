#ifndef GRIDGRAPHICSCENELAYER_H
#define GRIDGRAPHICSCENELAYER_H


#include "structs/GraphicSceneLayer.h"
#include "structs/Tcolor.h"

namespace Lonely {


class GridGraphicSceneLayer : public GraphicSceneLayer {
public:
  GridGraphicSceneLayer();
  
  GridGraphicSceneLayer(int lineSpacing__,
                        int lineWidth__ = 1,
                        Tcolor color__ = Tcolor(0, 0, 0,
                                              Tcolor::fullAlphaOpacity),
                        int xOrigin__ = 0,
                        int yOrigin__ = 0,
                        int xLimit__ = 0,
                        int yLimit__ = 0,
                        bool enlargeOnZoom__ = false);
                        
  virtual void render(Graphic& dst,
                      Box srcbox,
                      double scale);
  
  int lineSpacing() const;
  
  int lineWidth() const;
  
  Tcolor color() const;
  
  bool enlargeOnZoom() const;
  
  void setLineSpacing(int lineSpacing__);
  
  void setLineWidth(int lineWidth__);
  
  void setColor(Tcolor color__);
  
  void setEnlargeOnZoom(bool enlargeOnZoom__);
  
  void setXOrigin(int xOrigin__);
  void setYOrigin(int yOrigin__);
  
  void setXLimit(int xLimit__);
  void setYLimit(int yLimit__);
  
protected:
  
  int lineSpacing_;
  
  int lineWidth_;
  
  Tcolor color_;
  
  bool enlargeOnZoom_;
  
  int xOrigin_;
  int yOrigin_;
  int xLimit_;
  int yLimit_;
  
};


};


#endif

