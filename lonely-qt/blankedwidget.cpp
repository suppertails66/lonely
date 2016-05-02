#include "blankedwidget.h"
#include "ui_blankedwidget.h"

BlankEdWidget::BlankEdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlankEdWidget)
{
    ui->setupUi(this);
}

BlankEdWidget::~BlankEdWidget()
{
    delete ui;
}
