#include "structs/GraphicSceneLayer.h"

namespace Lonely {


GraphicSceneLayer::GraphicSceneLayer()
  : visible_(true) { };

GraphicSceneLayer::~GraphicSceneLayer() { };

bool GraphicSceneLayer::visible() const {
  return visible_;
}
  
void GraphicSceneLayer::setVisible(bool visible__) {
  visible_ = visible__;
}


};
