#include "setareaedwidget.h"
#include "ui_setareaedwidget.h"
#include <cstdlib>

SetAreaEdWidget::SetAreaEdWidget(QWidget *parent) :
    QWidget(parent),
    token_(NULL),
    ui(new Ui::SetAreaEdWidget)
{
    ui->setupUi(this);

    ui->areaTypeBox->addItem(
                "Cave", Lonely::SetAreaToken::caveID);
    ui->areaTypeBox->addItem(
                "Base", Lonely::SetAreaToken::baseFlag);
    ui->areaTypeBox->addItem(
                "Boss", Lonely::SetAreaToken::bossFlag);
}

SetAreaEdWidget::~SetAreaEdWidget()
{
    delete ui;
}

void SetAreaEdWidget::setToken(Lonely::SetAreaToken& token__) {
    token_ = &token__;

    switch (token_->realAreaType()) {
    case Lonely::SetAreaToken::cave:
        ui->areaTypeBox->setCurrentIndex(caveIndex_);
        break;
    case Lonely::SetAreaToken::base:
        ui->areaTypeBox->setCurrentIndex(baseIndex_);
        break;
    case Lonely::SetAreaToken::boss:
        ui->areaTypeBox->setCurrentIndex(bossIndex_);
        break;
    default:
        break;
    }

    ui->backgroundMetatileBox->setValue(token_->backgroundMetatile());
}

void SetAreaEdWidget::on_backgroundMetatileBox_valueChanged(int arg1)
{
    if (token_ != NULL) {
        token_->setBackgroundMetatile(arg1);
    }
}

void SetAreaEdWidget::on_areaTypeBox_activated(int index)
{
    if (token_ != NULL) {
        switch (index) {
        case caveIndex_:
            token_->setRealAreaType(Lonely::SetAreaToken::cave);
            break;
        case baseIndex_:
            token_->setRealAreaType(Lonely::SetAreaToken::base);
            break;
        case bossIndex_:
            token_->setRealAreaType(Lonely::SetAreaToken::boss);
            break;
        default:
            break;
        }
    }
}
