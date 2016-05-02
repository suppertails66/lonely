#ifndef PATTERNEDITORWIDGET_H
#define PATTERNEDITORWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include "editorwidget.h"
#include "cachedqimage.h"
#include "editor/LonelyLevelEditor.h"
#include "editor/LonelyPatternView.h"

namespace Ui {
class PatternEditorWidget;
}

class PatternEditorWidget : public EditorWidget
{
    Q_OBJECT
    
public:
    explicit PatternEditorWidget(QWidget *parent = 0);
    ~PatternEditorWidget();

    virtual void refreshDisplay();

    virtual void reload();
    
private:
    Ui::PatternEditorWidget *ui;

protected:
    Lonely::LonelyLevelEditor& levelEditor_;
    Lonely::LonelyPatternView& patternView_;

    CachedQImage patternViewCache_;
    CachedQImage metatilePickerCache_;
    CachedQImage objectsCache_;

    void refreshPatternList();

    void refreshPatternView();

    void refreshBoxes();

    void refreshObjectPatternList();

    void refreshObjectPatternListSelection();
private slots:
    void on_patternListWidget_itemClicked(QListWidgetItem *item);
    void on_zoomBox_activated(int index);
    void on_gridBox_clicked(bool checked);
    void on_widthBox_valueChanged(int arg1);
    void on_widthBox_editingFinished();

    void simplePatternViewMouseEvent();
    void complexPatternViewMouseEvent(QMouseEvent*);
    void on_visualButton_clicked(bool checked);
    void on_behaviorButton_clicked(bool checked);
    void on_solidityButton_clicked(bool checked);
    void on_zoomTilesBox_clicked(bool checked);
    void on_tilesetBox_activated(int index);
    void on_objectPatternListWidget_itemClicked(QListWidgetItem *item);
    void on_patternAddButton_clicked();
    void on_patternRemoveButton_clicked();
    void on_pencilButton_clicked(bool checked);
    void on_cloneButton_clicked(bool checked);
};

#endif // PATTERNEDITORWIDGET_H
