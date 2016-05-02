#ifndef METATILEEDITORWIDGET_H
#define METATILEEDITORWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "editorwidget.h"
#include "cachedqimage.h"
#include "editor/LonelyMetatileView.h"

namespace Ui {
class MetatileEditorWidget;
}

class MetatileEditorWidget : public EditorWidget
{
    Q_OBJECT
    
public:
    explicit MetatileEditorWidget(QWidget *parent = 0);
    ~MetatileEditorWidget();

    virtual void refreshDisplay();

    virtual void reload();
protected:
    void refreshViewLabels();

    void refreshBoxes();

    void refreshActiveMetatile();

    Lonely::LonelyMetatileView& metatileView_;

    CachedQImage metatilePickerCache_;
    CachedQImage tilePickerCache_;
    CachedQImage subtileViewCache_;

private:
    Ui::MetatileEditorWidget *ui;

private slots:
    void simpleViewMouseEvent();
    void complexViewMouseEvent(QMouseEvent*);
    void on_areaBox_activated(int index);
    void on_gridBox_clicked(bool checked);
    void on_metatileZoomBox_clicked(bool checked);
    void on_visualButton_clicked(bool checked);
    void on_behaviorButton_clicked(bool checked);
    void on_solidityButton_clicked(bool checked);
    void on_paletteBox_activated(int index);
    void on_behaviorBox_activated(int index);
    void on_topSolidBox_clicked(bool checked);
    void on_bottomSolidBox_clicked(bool checked);
    void on_metatileRemoveButton_clicked();
    void on_metatileAddButton_clicked();
};

#endif // METATILEEDITORWIDGET_H
