#include "leveleditorwidget.h"
#include "ui_leveleditorwidget.h"
#include "editorwidget.h"
#include <cstdlib>

using namespace Lonely;

LevelEditorWidget::LevelEditorWidget(
        Lonely::IniFile& configFile__,
        Lonely::LonelyLevelEditor& levelEditor__,
        QWidget *parent) :
    EditorWidget(parent),
    ui(new Ui::LevelEditorWidget),
    configFile_(configFile__),
    levelEditor_(levelEditor__)
{
    ui->setupUi(this);

    for (int i = 0; i < levelEditor_.numLevels(); i++) {
        ui->levelSelectBox->addItem(
                    levelEditor_.levelName(i).c_str(),
                    i);
    }

    loadLevel(levelEditor_.currentLevelNum());


}

LevelEditorWidget::~LevelEditorWidget()
{
    delete ui;
}

void LevelEditorWidget::refreshDisplay() {
//    ui->levelEditorTab->refreshDisplay();
}

void LevelEditorWidget::reload() {
    reloadCurrentWidget();
}

void LevelEditorWidget::loadLevel(int num) {
    levelEditor_.loadLevel(num);
//    ui->levelEditorTab->loadLevel(num);
    reloadCurrentWidget();

//    refreshDisplay();
}

void LevelEditorWidget::reloadCurrentWidget() {
    dynamic_cast<EditorWidget*>(ui->tabWidget->currentWidget())
            ->reload();
}

void LevelEditorWidget::on_levelSelectBox_activated(int index)
{
    loadLevel(ui->levelSelectBox->itemData(index).toInt());

//    reloadCurrentWidget();
}

void LevelEditorWidget::on_tabWidget_currentChanged(int index)
{
    if (index == -1) {
        return;
    }

    reloadCurrentWidget();
}
