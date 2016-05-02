#include "delayeditwidget.h"
#include "ui_delayeditwidget.h"
#include <cstdlib>

DelayEditWidget::DelayEditWidget(QWidget *parent) :
    ObjectEditorSubWidget(parent),
    delayToken_(NULL),
    ui(new Ui::DelayEditWidget)
{
    ui->setupUi(this);
}

DelayEditWidget::~DelayEditWidget()
{
    delete ui;
}

void DelayEditWidget::refreshDisplay() {
    if (delayToken_ != NULL) {
        ui->delayBox->setValue(delayToken_->delayAmount());
    }
}

void DelayEditWidget::setDelayToken(Lonely::LaylaObjectDelayToken& delayToken__) {
    delayToken_ = &delayToken__;
}

void DelayEditWidget::on_delayBox_valueChanged(int arg1)
{
    delayToken_->setDelayAmount(arg1);
}
