#include "metatileelevatoreditwidget.h"
#include "ui_metatileelevatoreditwidget.h"

MetatileElevatorEditWidget::MetatileElevatorEditWidget(QWidget *parent) :
    MetatileEditWidgetBase(parent),
    ui(new Ui::MetatileElevatorEditWidget)
{
    ui->setupUi(this);
}

MetatileElevatorEditWidget::~MetatileElevatorEditWidget()
{
    delete ui;
}

void MetatileElevatorEditWidget::refreshMetatileDisplay() {
    if (metatile_->behaviorFlag3) {
        ui->warpButton->setChecked(true);
    }
    else {
        ui->localButton->setChecked(true);
    }
}

void MetatileElevatorEditWidget::on_localButton_clicked()
{
    metatile_->behaviorFlag3 = false;
}

void MetatileElevatorEditWidget::on_warpButton_clicked()
{
    metatile_->behaviorFlag3 = true;
}
