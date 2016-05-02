#ifndef PALETTEEDITORWIDGET_H
#define PALETTEEDITORWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "editorwidget.h"
#include "editor/LonelyEditor.h"
#include "editor/LonelyPaletteView.h"
#include "cachedqimage.h"

namespace Ui {
class PaletteEditorWidget;
}

class PaletteEditorWidget : public EditorWidget
{
    Q_OBJECT
    
public:
    explicit PaletteEditorWidget(QWidget *parent = 0);
    ~PaletteEditorWidget();

    virtual void refreshDisplay();

    virtual void reload();

protected:
    Lonely::LonelyEditor& editor_;
    Lonely::LonelyPaletteView& paletteView_;

    CachedQImage colorPickerImage_;
    CachedQImage palettePickerImage_;

    void refreshViews();

private slots:
    void pickerSimpleEvent();
    void pickerComplexEvent(QMouseEvent* event);
    
    void on_palettePickerBox_activated(int index);

private:
    Ui::PaletteEditorWidget *ui;
};

#endif // PALETTEEDITORWIDGET_H
