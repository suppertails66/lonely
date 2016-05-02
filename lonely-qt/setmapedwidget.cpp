#include "setmapedwidget.h"
#include "ui_setmapedwidget.h"
#include <cstdlib>

SetMapEdWidget::SetMapEdWidget(QWidget *parent) :
    QWidget(parent),
    token_(NULL),
    ui(new Ui::SetMapEdWidget)
{
    ui->setupUi(this);
}

SetMapEdWidget::~SetMapEdWidget()
{
    delete ui;
}

void SetMapEdWidget::setToken(Lonely::SetMapToken& token__) {
    token_ = &token__;

    ui->mapNumBox->setValue(token_->mapNum());
    ui->param2Box->setValue(token_->param2());
}

void SetMapEdWidget::on_mapNumBox_valueChanged(int arg1)
{
    if (token_ != NULL) {
        token_->setMapNum(arg1);
    }
}

void SetMapEdWidget::on_param2Box_valueChanged(int arg1)
{
    if (token_ != NULL) {
        token_->setParam2(arg1);
    }
}
