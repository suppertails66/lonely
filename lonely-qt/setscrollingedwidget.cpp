#include "setscrollingedwidget.h"
#include "ui_setscrollingedwidget.h"

SetScrollingEdWidget::SetScrollingEdWidget(QWidget *parent) :
    QWidget(parent),
    token_(NULL),
    ui(new Ui::SetScrollingEdWidget)
{
    ui->setupUi(this);
}

SetScrollingEdWidget::~SetScrollingEdWidget()
{
    delete ui;
}

void SetScrollingEdWidget::setToken(Lonely::SetScrollingToken& token__) {
    token_ = &token__;

    ui->killsToNextFoodBox->setValue(token_->killsToNextFood());

    if (token_->scrollingIsDisabled()) {
        ui->scrollingDisabledButton->setChecked(true);
    }
    else {
        ui->scrollingEnabledButton->setChecked(true);
    }
}

void SetScrollingEdWidget::on_killsToNextFoodBox_valueChanged(int arg1)
{
    if (token_ != NULL) {
        token_->setKillsToNextFood(arg1);
    }
}

void SetScrollingEdWidget::on_scrollingEnabledButton_clicked()
{
    if (token_ != NULL) {
        token_->setScrollingDisabled(
                    Lonely::SetScrollingToken::scrollingEnabledValue);
    }
}

void SetScrollingEdWidget::on_scrollingDisabledButton_clicked()
{
    if (token_ != NULL) {
        token_->setScrollingDisabled(
                    Lonely::SetScrollingToken::scrollingDisabledValue);
    }
}
