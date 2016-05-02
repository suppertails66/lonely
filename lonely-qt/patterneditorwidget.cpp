#include "patterneditorwidget.h"
#include "ui_patterneditorwidget.h"
#include "lonelyglobals.h"
#include "structs/StandardEditingScene.h"
#include "structs/IndexedPickerScene.h"

using namespace Lonely;

PatternEditorWidget::PatternEditorWidget(QWidget *parent) :
    EditorWidget(parent),
    ui(new Ui::PatternEditorWidget),
    levelEditor_(globals_.editor().levelEditor()),
    patternView_(globals_.editor().levelEditor().patternView())
{
    ui->setupUi(this);

    ui->patternListWidget->setLevelEditor(levelEditor_);

    ui->patternViewLabel->setCallbacks(
                patternView_.patternViewCallbackObj(),
                StandardEditingScene::enterEventCallback,
                StandardEditingScene::leaveEventCallback,
                StandardEditingScene::mouseMoveEventCallback,
                StandardEditingScene::mousePressEventCallback,
                StandardEditingScene::mouseReleaseEventCallback,
                StandardEditingScene::mouseDoubleClickEventCallback);

    ui->metatilePickerLabel->setCallbacks(
                patternView_.metatilePickerCallbackObj(),
                IndexedPickerScene::enterEventCallback,
                IndexedPickerScene::leaveEventCallback,
                IndexedPickerScene::mouseMoveEventCallback,
                IndexedPickerScene::mousePressEventCallback,
                IndexedPickerScene::mouseReleaseEventCallback,
                IndexedPickerScene::mouseDoubleClickEventCallback);

    QObject::connect(ui->patternViewLabel,
                     SIGNAL(entered()),
                     this,
                     SLOT(simplePatternViewMouseEvent()));
    QObject::connect(ui->patternViewLabel,
                     SIGNAL(left()),
                     this,
                     SLOT(simplePatternViewMouseEvent()));
    QObject::connect(ui->patternViewLabel,
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->patternViewLabel,
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->patternViewLabel,
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->patternViewLabel,
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));

    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(entered()),
                     this,
                     SLOT(simplePatternViewMouseEvent()));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(left()),
                     this,
                     SLOT(simplePatternViewMouseEvent()));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(complexPatternViewMouseEvent(QMouseEvent*)));

//    refreshPatternList();
//    refreshDisplay();
    reload();
}

PatternEditorWidget::~PatternEditorWidget()
{
    delete ui;
}

void PatternEditorWidget::refreshDisplay() {
    refreshPatternView();
    refreshBoxes();
    refreshObjectPatternListSelection();
}

void PatternEditorWidget::reload() {
    patternView_.reloadRefresh();
    refreshPatternList();
    refreshObjectPatternList();
    refreshDisplay();
}

void PatternEditorWidget::refreshPatternList() {
    ui->patternListWidget->refreshPatternList();
}

void PatternEditorWidget::refreshPatternView() {
//    patternViewCache_.clear();
    // Resize the camera to the area of the display labels
//    patternView_.setPatternViewSize(ui->patternViewWidget->levelViewLabel().width(),
//                                    ui->patternViewWidget->levelViewLabel().height());

    // Draw the level
    patternView_.drawViews();

    // Update QImage cache
    patternViewCache_.directInit(patternView_.patternView());
    metatilePickerCache_.directInit(patternView_.metatilePickerView());
    objectsCache_.directInit(patternView_.objectsView());

    // Convert cache to pixmap and attach to display label
    ui->patternViewLabel->setPixmap(
                QPixmap().fromImage(patternViewCache_.image()));
    ui->metatilePickerLabel->setPixmap(
                QPixmap().fromImage(metatilePickerCache_.image()));
    ui->objectsViewLabel->setPixmap(
                QPixmap().fromImage(objectsCache_.image()));

    // Qt is too dumb to properly account for the width of the vertical scroll bar when
    // determining the width of the scroll area contents, so we have to add a small constant
    // to make sure there's enough room for it. Otherwise, we'll end up with an ugly
    // horizontal scroll bar in the metatile picker.
    ui->metatilePickerScrollArea->setMinimumWidth(
                metatilePickerCache_.image().width() + 32);
}

void PatternEditorWidget::refreshBoxes() {
    ui->gridBox->setChecked(patternView_.gridEnabled());
    ui->widthBox->setValue(patternView_.activePatternWidth());

    switch (patternView_.viewType()) {
    case MetatileViewTypes::visual:
        ui->visualButton->setChecked(true);
        break;
    case MetatileViewTypes::behavior:
        ui->behaviorButton->setChecked(true);
        break;
    case MetatileViewTypes::solidity:
        ui->solidityButton->setChecked(true);
        break;
    default:
        break;
    }

    ui->zoomTilesBox->setChecked(patternView_.metatilePickerZoomEnabled());

    ui->tilesetBox->selectFromAreaType(patternView_.activeAreaType());

    ui->pencilButton->setChecked(false);
    ui->cloneButton->setChecked(false);
    switch (patternView_.activeTool()) {
    case StandardEditingScene::pencil:
        ui->pencilButton->setChecked(true);
        break;
    case StandardEditingScene::clone:
        ui->cloneButton->setChecked(true);
        break;
    default:
        break;
    }
}

void PatternEditorWidget::refreshObjectPatternList() {
    ui->objectPatternListWidget->repopulate(patternView_.objectPatterns(),
                                            patternView_.currentObjectSetNum(),
                                            patternView_.currentSpritePatternTable(),
                                            patternView_.currentSpritePalettes());

/*    ui->objectPatternListWidget->repopulate(patternView_.objectPatterns(),
                                            patternView_.currentLevelNum(),
                                            patternView_.currentObjectSetNum(),
                                            patternView_.graphics(),
                                            patternView_.palettes()); */
}

void PatternEditorWidget::refreshObjectPatternListSelection() {
    ui->objectPatternListWidget->setCurrentRow(
                patternView_.currentObjectPatternIndex());
}

void PatternEditorWidget::on_patternListWidget_itemClicked(QListWidgetItem *item)
{
    patternView_.changePattern(item->data(Qt::UserRole).toInt());
    refreshObjectPatternList();
    refreshDisplay();
}

void PatternEditorWidget::on_zoomBox_activated(int index)
{
    patternView_.setZoom(ui->zoomBox->itemData(index, Qt::UserRole).toDouble());
    refreshDisplay();
}

void PatternEditorWidget::on_gridBox_clicked(bool checked)
{
    patternView_.setGridEnabled(checked);
    refreshDisplay();
}

void PatternEditorWidget::on_widthBox_valueChanged(int arg1)
{
    patternView_.changeActivePatternWidth(ui->widthBox->value());
    refreshDisplay();
}

void PatternEditorWidget::on_widthBox_editingFinished()
{
//    patternView_.changeActivePatternWidth(ui->widthBox->value());
//    refreshDisplay();
}

void PatternEditorWidget::simplePatternViewMouseEvent() {
    refreshDisplay();
}

void PatternEditorWidget::complexPatternViewMouseEvent(QMouseEvent*) {
    refreshDisplay();
}

void PatternEditorWidget::on_visualButton_clicked(bool checked)
{
    patternView_.changeViewType(MetatileViewTypes::visual);
    refreshDisplay();
}

void PatternEditorWidget::on_behaviorButton_clicked(bool checked)
{
    patternView_.changeViewType(MetatileViewTypes::behavior);
    refreshDisplay();
}

void PatternEditorWidget::on_solidityButton_clicked(bool checked)
{
    patternView_.changeViewType(MetatileViewTypes::solidity);
    refreshDisplay();
}

void PatternEditorWidget::on_zoomTilesBox_clicked(bool checked)
{
    patternView_.setMetatilePickerZoomEnabled(checked);
    refreshDisplay();
}

void PatternEditorWidget::on_tilesetBox_activated(int index)
{
    patternView_.changeMetatiles(
                ui->tilesetBox->itemData(index).toInt());
    refreshObjectPatternList();
    refreshDisplay();
}

void PatternEditorWidget::on_objectPatternListWidget_itemClicked(QListWidgetItem *item)
{
//    std::cout << item->data(Qt::UserRole).toInt() << std::endl;
//    std::cout << patternView_.currentObjectPatternIndex() << std::endl;
    patternView_.setCurrentObjectPatternIndex(item->data(Qt::UserRole).toInt());
//    std::cout << patternView_.currentObjectPatternIndex() << std::endl << std::endl;
    refreshDisplay();
}

void PatternEditorWidget::on_patternAddButton_clicked()
{
    int newIndex = patternView_.insertNewPattern();
    refreshPatternList();

    ui->patternListWidget->setCurrentRow(newIndex);
    patternView_.changePattern(newIndex);

    refreshDisplay();
}

void PatternEditorWidget::on_patternRemoveButton_clicked()
{
    int newIndex = patternView_.removePattern();
    refreshPatternList();

    ui->patternListWidget->setCurrentRow(newIndex);
    patternView_.changePattern(newIndex);

    refreshDisplay();
}

void PatternEditorWidget::on_pencilButton_clicked(bool checked)
{
    patternView_.changeActiveTool(StandardEditingScene::pencil);
    refreshDisplay();
}

void PatternEditorWidget::on_cloneButton_clicked(bool checked)
{
    patternView_.changeActiveTool(StandardEditingScene::clone);
    refreshDisplay();
}
