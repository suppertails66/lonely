#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(const std::string& message,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);

    ui->label->setText(message.c_str());
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}
