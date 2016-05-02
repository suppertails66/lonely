#include "structs/GridGraphicSceneLayer.h"
#include <iostream>

namespace Lonely {


GridGraphicSceneLayer::GridGraphicSceneLayer()
  : GraphicSceneLayer(),
    lineSpacing_(0),
    lineWidth_(0),
    enlargeOnZoom_(false),
    xOrigin_(0),
    yOrigin_(0),
    xLimit_(0),
    yLimit_(0) { };
  
GridGraphicSceneLayer::GridGraphicSceneLayer(
                      int lineSpacing__,
                      int lineWidth__,
                      Tcolor color__,
                      int xOrigin__,
                      int yOrigin__,
                      int xLimit__,
                      int yLimit__,
                      bool enlargeOnZoom__)
  : GraphicSceneLayer(),
    lineSpacing_(lineSpacing__),
    lineWidth_(lineWidth__),
    color_(color__),
    enlargeOnZoom_(enlargeOnZoom__),
    xOrigin_(xOrigin__),
    yOrigin_(yOrigin__),
    xLimit_(xLimit__),
    yLimit_(yLimit__) { };
  
void GridGraphicSceneLayer::render(Graphic& dst,
            Box srcbox,
            double scale) {
  if (!visible_) {
    return;
  }
  
//  int maxPos = basePos + (srcbox.w() * scale);
  int realSpacing = lineSpacing_ * scale;
  int realXLimit = xLimit_ * scale;
  int realYLimit = yLimit_ * scale;
  
  if (xLimit_ == 0) {
//    realXLimit = (srcbox.x() + srcbox.w()) * scale;
    realXLimit = dst.w();
  }
  
  if (yLimit_ == 0) {
//    realYLimit = (srcbox.y() + srcbox.h()) * scale;
    realYLimit = dst.h();
  }
  
  if (dst.w() < realXLimit) {
    realXLimit = dst.w();
  }
  
  if (dst.h() < realYLimit) {
    realYLimit = dst.h();
  }
  
  int realLineWidth = lineWidth_;
  if (enlargeOnZoom_) {
    realLineWidth *= scale;
  }
  
  int originOffsetX = xOrigin_ - srcbox.x();
  int originOffsetY = yOrigin_ - srcbox.y();
  
  int realXOrigin = 0;
  int realYOrigin = 0;
  
  if (originOffsetX > 0) {
    realXOrigin = originOffsetX * scale;
  }
  
  if (originOffsetY > 0) {
    realYOrigin = originOffsetY * scale;
  }
  
  // Compute where to start drawing lines
  int baseXPos = srcbox.x() * scale;
//  int baseXPos = srcbox.x();
  int drawXBase = -(baseXPos % realSpacing);
  
  int baseYPos = srcbox.y() * scale;
//  int baseYPos = srcbox.y();
  int drawYBase = -(baseYPos % realSpacing);
  
  // Draw vertical lines
  for (int i = drawXBase; i <= realXLimit; i += realSpacing) {
    dst.drawLine(i, realYOrigin,
                 i, realYLimit,
                 color_,
                 realLineWidth,
                 Graphic::noTransUpdate);
  }
  
  // Draw horizontal lines
  for (int i = drawYBase; i <= realYLimit; i += realSpacing) {
    dst.drawLine(realXOrigin, i,
                 realXLimit, i,
                 color_,
                 realLineWidth,
                 Graphic::noTransUpdate);
  }
}
  
int GridGraphicSceneLayer::lineSpacing() const {
  return lineSpacing_;
}

int GridGraphicSceneLayer::lineWidth() const {
  return lineWidth_;
}

Tcolor GridGraphicSceneLayer::color() const {
  return color_;
}

bool GridGraphicSceneLayer::enlargeOnZoom() const {
  return enlargeOnZoom_;
}

void GridGraphicSceneLayer::setLineSpacing(int lineSpacing__) {
  lineSpacing_ = lineSpacing__;
}

void GridGraphicSceneLayer::setLineWidth(int lineWidth__) {
  lineWidth_ = lineWidth__;
}

void GridGraphicSceneLayer::setColor(Tcolor color__) {
  color_ = color__;
}

void GridGraphicSceneLayer::setEnlargeOnZoom(bool enlargeOnZoom__) {
  enlargeOnZoom_ = enlargeOnZoom__;
}
  
void GridGraphicSceneLayer::setXLimit(int xLimit__) {
  xLimit_ = xLimit__;
}

void GridGraphicSceneLayer::setYLimit(int yLimit__) {
  yLimit_ = yLimit__;
}
  
void GridGraphicSceneLayer::setXOrigin(int xOrigin__) {
  xOrigin_ = xOrigin__;
}

void GridGraphicSceneLayer::setYOrigin(int yOrigin__) {
  yOrigin_ = yOrigin__;
}


};

