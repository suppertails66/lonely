#include "metatilebehaviorbox.h"
#include "gamedata/LaylaMetatileBehavior.h"

using namespace Lonely;

MetatileBehaviorBox::MetatileBehaviorBox(QWidget *parent) :
    QComboBox(parent)
{
    for (int i = 0; i < LaylaMetatileBehaviors::numMetatileBehaviors; i++) {
        LaylaMetatileBehaviors::LaylaMetatileBehavior behavior
                = static_cast<
                    LaylaMetatileBehaviors::LaylaMetatileBehavior>(i);

        addItem(LaylaMetatileBehaviors::nameOfBehavior(behavior).c_str(),
                i);
    }
}
