#ifndef ROMSELECTDIALOG_H
#define ROMSELECTDIALOG_H

#include <QDialog>
#include <string>
#include "util/IniFile.h"

namespace Ui {
class RomSelectDialog;
}

class RomSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RomSelectDialog(QWidget *parent = 0);
    RomSelectDialog(
            Lonely::IniFile& configFile,
            QWidget *parent = 0);
    ~RomSelectDialog();

    std::string selectedRomPath() const;

    std::string selectedOffsetFile() const;

    bool rememberLastExportLocation() const;

    void setSelectedOffsetFile(const std::string& path__);
    void setRememberLastExportLocation(bool rememberLastExportLocation__);

protected:
    Lonely::IniFile* configFile_;
    
private slots:
    void on_romPathBrowseButton_clicked();

    void on_rememberLastExportLocationBox_clicked(bool checked);

    void on_offsetFileBrowseButton_clicked(bool checked);

    void onAccepted();

private:
    Ui::RomSelectDialog *ui;
};

#endif // ROMSELECTDIALOG_H
