#ifndef RESISTANCEEDITORWIDGET_H
#define RESISTANCEEDITORWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "editorwidget.h"
#include "editor/LonelyResistanceView.h"

namespace Ui {
class ResistanceEditorWidget;
}

class ResistanceEditorWidget : public EditorWidget
{
    Q_OBJECT
    
public:
    explicit ResistanceEditorWidget(QWidget *parent = 0);
    ~ResistanceEditorWidget();

    virtual void refreshDisplay();

    virtual void reload();

protected:
    Lonely::LonelyResistanceView& resistanceView_;

    void repopulateLists();

    void refreshVulnerabilityEditor();
    
private slots:
    void on_objectListWidget_itemClicked(QListWidgetItem *item);

    void on_bossListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::ResistanceEditorWidget *ui;
};

#endif // RESISTANCEEDITORWIDGET_H
