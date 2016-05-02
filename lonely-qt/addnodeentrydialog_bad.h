#ifndef ADDNODEENTRYDIALOG_H
#define ADDNODEENTRYDIALOG_H

#include <QDialog>

namespace Ui {
class AddNodeEntryDialog;
}

class AddNodeEntryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddNodeEntryDialog(QWidget *parent = 0);
    ~AddNodeEntryDialog();
    
private:
    Ui::AddNodeEntryDialog *ui;
};

#endif // ADDNODEENTRYDIALOG_H
