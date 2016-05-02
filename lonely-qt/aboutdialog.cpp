#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "lonelyglobals.h"
#include "util/StringConversion.h"

using namespace Lonely;

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->versionNumLabel->setText(StringConversion::toString(
                                     globals_.editor().version()).c_str());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
