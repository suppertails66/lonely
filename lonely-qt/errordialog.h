#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ErrorDialog(const std::string& message,
                         QWidget *parent = 0);
    ~ErrorDialog();
    
private:
    Ui::ErrorDialog *ui;
};

#endif // ERRORDIALOG_H
