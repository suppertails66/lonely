#ifndef LONELYMETATILEPICKERSCENEFUNCTOR_H
#define LONELYMETATILEPICKERSCENEFUNCTOR_H


#include "structs/IndexedPickerSceneFunctor.h"

namespace Lonely {


class LonelyPatternView;
class LonelyMetatileView;

class PatternViewMetatilePicked
  : public IndexedPickerSceneItemPicked {
public:
  PatternViewMetatilePicked(LonelyPatternView& view__);
  
  virtual void operator()(int index);
protected:
  LonelyPatternView* view_;
  
};

class MetatileViewMetatilePicked
  : public IndexedPickerSceneItemPicked {
public:
  MetatileViewMetatilePicked(LonelyMetatileView& view__);
  
  virtual void operator()(int index);
protected:
  LonelyMetatileView* view_;
  
};

class MetatileViewTilePicked
  : public IndexedPickerSceneItemPicked {
public:
  MetatileViewTilePicked(LonelyMetatileView& view__);
  
  virtual void operator()(int index);
protected:
  LonelyMetatileView* view_;
  
};

class MetatileViewSubtilePicked
  : public IndexedPickerSceneItemPicked {
public:
  MetatileViewSubtilePicked(LonelyMetatileView& view__);
  
  virtual void operator()(int index);
protected:
  LonelyMetatileView* view_;
  
};


};


#endif
