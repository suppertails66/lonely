#include "editor/LonelyMetatilePickerSceneFunctor.h"
#include "editor/LonelyPatternView.h"
#include "editor/LonelyMetatileView.h"

namespace Lonely {


PatternViewMetatilePicked
  ::PatternViewMetatilePicked(LonelyPatternView& view__)
  : IndexedPickerSceneItemPicked(),
    view_(&view__) { };
  
void PatternViewMetatilePicked::operator()(int index) {
  view_->metatilePickerItemPicked(index);
}

MetatileViewMetatilePicked
  ::MetatileViewMetatilePicked(LonelyMetatileView& view__)
  : IndexedPickerSceneItemPicked(),
    view_(&view__) { };
  
void MetatileViewMetatilePicked::operator()(int index) {
  view_->metatilePickerItemPicked(index);
}

MetatileViewTilePicked
  ::MetatileViewTilePicked(LonelyMetatileView& view__)
  : IndexedPickerSceneItemPicked(),
    view_(&view__) { };
  
void MetatileViewTilePicked::operator()(int index) {
  view_->tilePickerItemPicked(index);
}

MetatileViewSubtilePicked
  ::MetatileViewSubtilePicked(LonelyMetatileView& view__)
  : IndexedPickerSceneItemPicked(),
    view_(&view__) { };
  
void MetatileViewSubtilePicked::operator()(int index) {
  view_->subtilePickerItemPicked(index);
}


};
