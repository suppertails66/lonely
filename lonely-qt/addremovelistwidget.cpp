#include "addremovelistwidget.h"
#include "ui_addremovelistwidget.h"

AddRemoveListWidget::AddRemoveListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddRemoveListWidget)
{
    ui->setupUi(this);
}

AddRemoveListWidget::~AddRemoveListWidget()
{
    delete ui;
}

QListWidget& AddRemoveListWidget::list() {
    return *(ui->listWidget);
}

QPushButton& AddRemoveListWidget::addButton() {
    return *(ui->addButton);
}

QPushButton& AddRemoveListWidget::removeButton() {
    return *(ui->removeButton);
}

void AddRemoveListWidget::on_addButton_clicked()
{
    emit addButtonClicked();
}

void AddRemoveListWidget::on_removeButton_clicked()
{
    emit removeButtonClicked();
}
