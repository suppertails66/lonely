#ifndef OBJECTEDITORWIDGET_H
#define OBJECTEDITORWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "editorwidget.h"
#include "editor/LonelyLevelEditor.h"
#include "editor/LonelyObjectView.h"

namespace Ui {
class ObjectEditorWidget;
}

class ObjectEditorWidget : public EditorWidget
{
    Q_OBJECT

public:
    explicit ObjectEditorWidget(QWidget *parent = 0);
    virtual ~ObjectEditorWidget();

    virtual void refreshDisplay();

    virtual void reload();

protected:
    void refreshObjectPatternList();
    void reloadObjectPatternList();

    void refreshBoxes();

    void refreshObjectEditStackedWidget();

    Lonely::LonelyLevelEditor& levelEditor_;
    Lonely::LonelyObjectView& objectView_;
    
private slots:
    void on_objectPatternListWidget_itemClicked(QListWidgetItem *item);

    void on_objectSetBox_activated(int index);

    void on_objectRemoveButton_clicked();

    void on_objectAddButton_clicked();

    void on_objectUpButton_clicked();

    void on_objectDownButton_clicked();

private:
    Ui::ObjectEditorWidget *ui;
};

#endif // OBJECTEDITORWIDGET_H
