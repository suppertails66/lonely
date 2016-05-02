#include "jumptopositionedwidget.h"
#include "ui_jumptopositionedwidget.h"
#include <cstdlib>

JumpToPositionEdWidget::JumpToPositionEdWidget(QWidget *parent) :
    QWidget(parent),
    token_(NULL),
    ui(new Ui::JumpToPositionEdWidget)
{
    ui->setupUi(this);
}

JumpToPositionEdWidget::~JumpToPositionEdWidget()
{
    delete ui;
}

void JumpToPositionEdWidget::setToken(Lonely::JumpToPositionToken& token__) {
    token_ = &token__;

    ui->offsetBox->setValue(token_->offset());
}

void JumpToPositionEdWidget::on_offsetBox_valueChanged(int arg1)
{
    if (token_ != NULL) {
        token_->setOffset(arg1);
    }
}
