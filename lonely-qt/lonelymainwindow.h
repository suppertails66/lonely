#ifndef LONELYMAINWINDOW_H
#define LONELYMAINWINDOW_H

#include <QMainWindow>
#include "editor/LonelyEditor.h"
#include "util/IniFile.h"
#include "editor/LonelyEditor.h"
#include "lonelywidgets.h"
#include "numberedqpushbutton.h"
#include "numberedqaction.h"

namespace Ui {
class LonelyMainWindow;
}

class LonelyMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LonelyMainWindow(Lonely::IniFile& configFile__,
                              QWidget *parent = 0);
    ~LonelyMainWindow();
    
private:
    Lonely::IniFile& configFile_;

    Lonely::LonelyEditor editor_;

    NumberedQPushButton* toolButtons_[LonelyWidgets::numWidgets];
    NumberedQAction* toolActions_[LonelyWidgets::numWidgets];

    Ui::LonelyMainWindow *ui;

    void initTools();

    void changeTool(LonelyWidgets::LonelyWidget widgetID);

    void exportToFile(const std::string& filename);

private slots:
    void toolButtonClicked(int num, bool checked);

    void toolActionTriggered(int num);
    void on_actionExport_as_triggered();
    void on_actionSave_as_triggered();
    void on_actionOpen_project_triggered();
    void on_actionOptions_triggered();
    void on_actionSave_project_triggered();
    void on_actionExport_ROM_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionSave_map_triggered();
};

#endif // LONELYMAINWINDOW_H
