#include "customscrollwidget.h"
#include "ui_customscrollwidget.h"

CustomScrollWidget::CustomScrollWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomScrollWidget)
{
    ui->setupUi(this);
}

CustomScrollWidget::~CustomScrollWidget()
{
    delete ui;
}
