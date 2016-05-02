#include "metatileeditwidgetbase.h"
#include <cstdlib>

MetatileEditWidgetBase::MetatileEditWidgetBase(QWidget *parent) :
    QWidget(parent),
    metatile_(NULL)
{
}

void MetatileEditWidgetBase::editMetatile(Lonely::LaylaMetatile& metatile__) {
    metatile_ = &metatile__;

    refreshMetatileDisplay();
}
