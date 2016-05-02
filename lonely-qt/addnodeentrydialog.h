#ifndef ADDNODEENTRYDIALOG_H
#define ADDNODEENTRYDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "gamedata/LaylaPatternToken.h"

namespace Ui {
class AddNodeEntryDialog;
}

class AddNodeEntryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddNodeEntryDialog(QWidget *parent = 0);
    ~AddNodeEntryDialog();

    Lonely::LaylaPatternToken* token();
protected:
    Lonely::LaylaPatternToken* token_;

    void changeType(Lonely::LaylaPatternTypes::LaylaPatternType patternType);

    
private slots:
    void on_nodeTypeList_itemClicked(QListWidgetItem *item);

private:
    Ui::AddNodeEntryDialog *ui;
};

#endif // ADDNODEENTRYDIALOG_H
