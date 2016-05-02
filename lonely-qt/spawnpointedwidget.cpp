#include "spawnpointedwidget.h"
#include "ui_spawnpointedwidget.h"

SpawnPointEdWidget::SpawnPointEdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpawnPointEdWidget)
{
    ui->setupUi(this);
}

SpawnPointEdWidget::~SpawnPointEdWidget()
{
    delete ui;
}
