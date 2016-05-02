#include "paletteeditorwidget.h"
#include "ui_paletteeditorwidget.h"
#include "lonelyglobals.h"
#include "structs/IndexedPickerScene.h"
#include "util/StringConversion.h"

using namespace Lonely;

PaletteEditorWidget::PaletteEditorWidget(QWidget *parent) :
    EditorWidget(parent),
    editor_(globals_.editor()),
    paletteView_(globals_.editor().levelEditor().paletteView()),
    ui(new Ui::PaletteEditorWidget)
{
    ui->setupUi(this);

    for (int i = 0; i < LonelyPaletteView::numEditablePalettes; i++) {
        ui->palettePickerBox->addItem(
                    LonelyPaletteView::nameOfEditablePalette(
                        static_cast<LonelyPaletteView::EditablePalette>(i))
                        .c_str(),
                    i);
    }

    ui->colorPickerLabel->setCallbacks(
                paletteView_.colorPickerCallbackObj(),
                IndexedPickerScene::enterEventCallback,
                IndexedPickerScene::leaveEventCallback,
                IndexedPickerScene::mouseMoveEventCallback,
                IndexedPickerScene::mousePressEventCallback,
                IndexedPickerScene::mouseReleaseEventCallback,
                IndexedPickerScene::mouseDoubleClickEventCallback);
    ui->palettePickerLabel->setCallbacks(
                paletteView_.palettePickerCallbackObj(),
                IndexedPickerScene::enterEventCallback,
                IndexedPickerScene::leaveEventCallback,
                IndexedPickerScene::mouseMoveEventCallback,
                IndexedPickerScene::mousePressEventCallback,
                IndexedPickerScene::mouseReleaseEventCallback,
                IndexedPickerScene::mouseDoubleClickEventCallback);

    QObject::connect(ui->colorPickerLabel,
                     SIGNAL(entered()),
                     this,
                     SLOT(pickerSimpleEvent()));
    QObject::connect(ui->colorPickerLabel,
                     SIGNAL(left()),
                     this,
                     SLOT(pickerSimpleEvent()));
    QObject::connect(ui->colorPickerLabel,
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));
    QObject::connect(ui->colorPickerLabel,
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));
    QObject::connect(ui->colorPickerLabel,
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));
    QObject::connect(ui->colorPickerLabel,
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));

    QObject::connect(ui->palettePickerLabel,
                     SIGNAL(entered()),
                     this,
                     SLOT(pickerSimpleEvent()));
    QObject::connect(ui->palettePickerLabel,
                     SIGNAL(left()),
                     this,
                     SLOT(pickerSimpleEvent()));
    QObject::connect(ui->palettePickerLabel,
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));
    QObject::connect(ui->palettePickerLabel,
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));
    QObject::connect(ui->palettePickerLabel,
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));
    QObject::connect(ui->palettePickerLabel,
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(pickerComplexEvent(QMouseEvent*)));
}

PaletteEditorWidget::~PaletteEditorWidget()
{
    delete ui;
}

void PaletteEditorWidget::refreshDisplay() {
    refreshViews();
}

void PaletteEditorWidget::refreshViews() {
    paletteView_.drawViews();

    colorPickerImage_.directInit(paletteView_.colorPickerView());
    ui->colorPickerLabel->setPixmap(
                QPixmap().fromImage(colorPickerImage_.image()));

    palettePickerImage_.directInit(paletteView_.palettePickerView());
    ui->palettePickerLabel->setPixmap(
                QPixmap().fromImage(palettePickerImage_.image()));


}

void PaletteEditorWidget::reload() {
    refreshDisplay();
}

void PaletteEditorWidget::pickerSimpleEvent() {
    refreshDisplay();
}

void PaletteEditorWidget::pickerComplexEvent(QMouseEvent* event) {
    refreshDisplay();
}

void PaletteEditorWidget::on_palettePickerBox_activated(int index)
{
    paletteView_.changePalette(static_cast<LonelyPaletteView::EditablePalette>(index));
    refreshDisplay();
}
