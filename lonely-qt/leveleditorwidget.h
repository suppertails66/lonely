#ifndef LEVELEDITORWIDGET_H
#define LEVELEDITORWIDGET_H

#include <QWidget>
#include "editorwidget.h"
#include "editor/LonelyLevelEditor.h"
#include "util/IniFile.h"

namespace Ui {
class LevelEditorWidget;
}

class LevelEditorWidget : public EditorWidget
{
    Q_OBJECT
    
public:
    explicit LevelEditorWidget(
            Lonely::IniFile& configFile__,
            Lonely::LonelyLevelEditor& levelEditor__,
            QWidget *parent = 0);
    ~LevelEditorWidget();

    virtual void refreshDisplay();

    virtual void reload();
    
private:
    Ui::LevelEditorWidget *ui;

protected:
    Lonely::IniFile& configFile_;
    Lonely::LonelyLevelEditor& levelEditor_;

    void loadLevel(int num);

    void reloadCurrentWidget();
private slots:
    void on_levelSelectBox_activated(int index);
    void on_tabWidget_currentChanged(int index);
};

#endif // LEVELEDITORWIDGET_H
