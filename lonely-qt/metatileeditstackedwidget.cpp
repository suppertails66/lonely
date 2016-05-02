#include "metatileeditstackedwidget.h"

using namespace Lonely;

MetatileEditStackedWidget::MetatileEditStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    elevatorWidget_(new MetatileElevatorEditWidget()),
    damagingWidget_(new MetatileDamagingEditWidget())
{
    addWidget(elevatorWidget_);
    addWidget(damagingWidget_);
}

/*MetatileEditStackedWidget::~MetatileEditStackedWidget() {

} */

void MetatileEditStackedWidget::editMetatile(Lonely::LaylaMetatile& metatile__) {
    if (metatile__.behavior == LaylaMetatileBehaviors::elevator) {
        setCurrentWidget(elevatorWidget_);
        elevatorWidget_->editMetatile(metatile__);
    }
    else {
        setCurrentWidget(damagingWidget_);
        damagingWidget_->editMetatile(metatile__);
    }
}
