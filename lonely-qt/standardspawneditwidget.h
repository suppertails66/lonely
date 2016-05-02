#ifndef STANDARDSPAWNEDITWIDGET_H
#define STANDARDSPAWNEDITWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "objecteditorsubwidget.h"
#include "gamedata/LaylaObjectPatternToken.h"

namespace Ui {
class StandardSpawnEditWidget;
}

class StandardSpawnEditWidget : public ObjectEditorSubWidget
{
    Q_OBJECT
    
public:
    explicit StandardSpawnEditWidget(QWidget *parent = 0);
    ~StandardSpawnEditWidget();
    
    void refreshDisplay();

    void setSpawnToken(Lonely::LaylaObjectStandardSpawnToken& spawnToken__);
protected:
    Lonely::LaylaObjectStandardSpawnToken* spawnToken_;

    void reloadObjectList();

private slots:
    void on_continueInterpretingBox_clicked(bool checked);

    void on_objectListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::StandardSpawnEditWidget *ui;
};

#endif // STANDARDSPAWNEDITWIDGET_H
