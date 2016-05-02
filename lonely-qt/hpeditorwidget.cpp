#include "hpeditorwidget.h"
#include "ui_hpeditorwidget.h"
#include <cstdlib>

HpEditorWidget::HpEditorWidget(QWidget *parent) :
    QWidget(parent),
    hp_(NULL),
    ui(new Ui::HpEditorWidget)
{
    ui->setupUi(this);
}

HpEditorWidget::~HpEditorWidget()
{
    delete ui;
}

void HpEditorWidget::editHp(Lonely::Tbyte& hp__)  {
    hp_ = &hp__;

    ui->hpBox->setValue(*hp_);
}

void HpEditorWidget::clearHp() {
    hp_ = NULL;
}

bool HpEditorWidget::hpLoaded() const {
    return (hp_ != NULL);
}

void HpEditorWidget::on_hpBox_valueChanged(int arg1)
{
    if (hp_ != NULL) {
        *hp_ = arg1;
    }
}
