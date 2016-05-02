#include "setelevatorsedwidget.h"
#include "ui_setelevatorsedwidget.h"
#include <cstdlib>

SetElevatorsEdWidget::SetElevatorsEdWidget(QWidget *parent) :
    QWidget(parent),
    token_(NULL),
    ui(new Ui::SetElevatorsEdWidget)
{
    ui->setupUi(this);
}

SetElevatorsEdWidget::~SetElevatorsEdWidget()
{
    delete ui;
}

void SetElevatorsEdWidget::setToken(Lonely::SetElevatorsToken& token__) {
    token_ = &token__;

    ui->upDestinationBox->setValue(token_->upDestination());
    ui->downDestinationBox->setValue(token_->downDestination());
}

void SetElevatorsEdWidget::on_upDestinationBox_valueChanged(int arg1)
{
    if (token_ != NULL) {
        token_->setUpDestination(arg1);
    }
}

void SetElevatorsEdWidget::on_downDestinationBox_valueChanged(int arg1)
{
    if (token_ != NULL) {
        token_->setDownDestination(arg1);
    }
}
