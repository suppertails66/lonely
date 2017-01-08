#include "romselectdialog.h"
#include "ui_romselectdialog.h"
#include <QFileDialog>
#include "util/StringConversion.h"
#include <cstdlib>
#include <iostream>

using namespace Lonely;

RomSelectDialog::RomSelectDialog(QWidget *parent) :
    QDialog(parent),
    configFile_(NULL),
    ui(new Ui::RomSelectDialog)
{
    ui->setupUi(this);

    QObject::connect(this,
                     SIGNAL(accepted()),
                     this,
                     SLOT(onAccepted()));
}

RomSelectDialog::RomSelectDialog(
        IniFile& configFile,
        QWidget *parent) :
    QDialog(parent),
    configFile_(&configFile),
    ui(new Ui::RomSelectDialog)
{
    ui->setupUi(this);

    ui->romPathEdit->setText(configFile.valueOfKey("Global", "SourceRomPath").c_str());
    setSelectedOffsetFile(
                (configFile.valueOfKey("Global", "OffsetFilePath")));
    setRememberLastExportLocation(
                StringConversion::fromString<bool>(
                    (configFile.valueOfKey("Global", "SaveLastExportLocation"))));

    QObject::connect(this,
                     SIGNAL(accepted()),
                     this,
                     SLOT(onAccepted()));
}

RomSelectDialog::~RomSelectDialog()
{
    delete ui;
}

void RomSelectDialog::on_romPathBrowseButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open ROM"),
                                                tr(""),
                                                tr(""));

    if (path == "") {
        return;
    }

    ui->romPathEdit->setText(path);
}

std::string RomSelectDialog::selectedRomPath() const {
    return ui->romPathEdit->text().toStdString();
}

std::string RomSelectDialog::selectedOffsetFile() const {
    return ui->offsetFileLineEdit->text().toStdString();
}

bool RomSelectDialog::rememberLastExportLocation() const {
    return ui->rememberLastExportLocationBox->isChecked();
}

void RomSelectDialog::setSelectedOffsetFile(const std::string& path__) {
    ui->offsetFileLineEdit->setText(path__.c_str());
}

void RomSelectDialog::setRememberLastExportLocation(bool rememberLastExportLocation__) {
    ui->rememberLastExportLocationBox->setChecked(rememberLastExportLocation__);
}

void RomSelectDialog::on_rememberLastExportLocationBox_clicked(bool checked)
{

}

void RomSelectDialog::on_offsetFileBrowseButton_clicked(bool checked)
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open offset file"),
                                                tr(""),
                                                tr("Text files (*.txt)"));

    if (path == "") {
        return;
    }

    ui->offsetFileLineEdit->setText(path);
}

void RomSelectDialog::onAccepted() {
    if (configFile_ != NULL) {
        configFile_->setValue(
                    "Global", "SourceRomPath", selectedRomPath());
        configFile_->setValue(
                    "Global", "OffsetFilePath", selectedOffsetFile());
        configFile_->setValue(
                    "Global", "SaveLastExportLocation",
                        StringConversion::toString(rememberLastExportLocation()));
    }
}
