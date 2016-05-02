#ifndef ADDOBJECTPATTERNDIALOG_H
#define ADDOBJECTPATTERNDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "editor/LonelyObjectView.h"
#include "gamedata/LaylaObjectPatternToken.h"

namespace Ui {
class AddObjectPatternDialog;
}

class AddObjectPatternDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddObjectPatternDialog(
            Lonely::LonelyObjectView& objectView__,
            QWidget *parent = 0);
    ~AddObjectPatternDialog();

    Lonely::LaylaObjectPatternToken* token();
    
protected:
    void changeToken(Lonely::LaylaObjectPatternTypes::LaylaObjectPatternType
                        patternType);

    Lonely::LonelyObjectView* objectView_;

    Lonely::LaylaObjectPatternToken* token_;

private slots:
    void on_objectPatternTypeListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::AddObjectPatternDialog *ui;
};

#endif // ADDOBJECTPATTERNDIALOG_H
