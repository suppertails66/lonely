#include "metatiledamagingeditwidget.h"
#include "ui_metatiledamagingeditwidget.h"

MetatileDamagingEditWidget::MetatileDamagingEditWidget(QWidget *parent) :
    MetatileEditWidgetBase(parent),
    ui(new Ui::MetatileDamagingEditWidget)
{
    ui->setupUi(this);
}

MetatileDamagingEditWidget::~MetatileDamagingEditWidget()
{
    delete ui;
}

void MetatileDamagingEditWidget::refreshMetatileDisplay() {
    if ((metatile_->behaviorFlag2)
            && (metatile_->behaviorFlag3)) {
        ui->damagingBox->setChecked(true);
    }
    else {
        ui->damagingBox->setChecked(false);
    }
}

void MetatileDamagingEditWidget::on_damagingBox_clicked(bool checked)
{
    if (checked) {
        metatile_->behaviorFlag2 = true;
        metatile_->behaviorFlag3 = true;
    }
    else {
        metatile_->behaviorFlag2 = false;
        metatile_->behaviorFlag3 = false;
    }
}
