#include "metatileeditorwidget.h"
#include "ui_metatileeditorwidget.h"
#include "lonelyglobals.h"
#include "gamedata/LaylaPatternToken.h"
#include "editor/LonelyMetatilePickerScene.h"

using namespace Lonely;

MetatileEditorWidget::MetatileEditorWidget(QWidget *parent) :
    EditorWidget(parent),
    metatileView_(globals_.editor().levelEditor().metatileView()),
    ui(new Ui::MetatileEditorWidget)
{
    ui->setupUi(this);

    ui->areaBox->changeAreaText(SetAreaToken::caveID,
                                "Cave (global)");
    ui->areaBox->changeAreaText(SetAreaToken::bossFlag,
                                "Boss (global)");

    ui->metatilePickerLabel->setCallbacks(
                metatileView_.metatilePickerCallbackObj(),
                IndexedPickerScene::enterEventCallback,
                IndexedPickerScene::leaveEventCallback,
                IndexedPickerScene::mouseMoveEventCallback,
                IndexedPickerScene::mousePressEventCallback,
                IndexedPickerScene::mouseReleaseEventCallback,
                IndexedPickerScene::mouseDoubleClickEventCallback);

    ui->tilePickerLabel->setCallbacks(
                metatileView_.tilePickerCallbackObj(),
                IndexedPickerScene::enterEventCallback,
                IndexedPickerScene::leaveEventCallback,
                IndexedPickerScene::mouseMoveEventCallback,
                IndexedPickerScene::mousePressEventCallback,
                IndexedPickerScene::mouseReleaseEventCallback,
                IndexedPickerScene::mouseDoubleClickEventCallback);

    ui->subtileViewLabel->setCallbacks(
                metatileView_.subtilePickerCallbackObj(),
                IndexedPickerScene::enterEventCallback,
                IndexedPickerScene::leaveEventCallback,
                IndexedPickerScene::mouseMoveEventCallback,
                IndexedPickerScene::mousePressEventCallback,
                IndexedPickerScene::mouseReleaseEventCallback,
                IndexedPickerScene::mouseDoubleClickEventCallback);

    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(entered()),
                     this,
                     SLOT(simpleViewMouseEvent()));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(left()),
                     this,
                     SLOT(simpleViewMouseEvent()));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->metatilePickerLabel,
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));

    QObject::connect(ui->tilePickerLabel,
                     SIGNAL(entered()),
                     this,
                     SLOT(simpleViewMouseEvent()));
    QObject::connect(ui->tilePickerLabel,
                     SIGNAL(left()),
                     this,
                     SLOT(simpleViewMouseEvent()));
    QObject::connect(ui->tilePickerLabel,
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->tilePickerLabel,
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->tilePickerLabel,
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->tilePickerLabel,
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));

    QObject::connect(ui->subtileViewLabel,
                     SIGNAL(entered()),
                     this,
                     SLOT(simpleViewMouseEvent()));
    QObject::connect(ui->subtileViewLabel,
                     SIGNAL(left()),
                     this,
                     SLOT(simpleViewMouseEvent()));
    QObject::connect(ui->subtileViewLabel,
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->subtileViewLabel,
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->subtileViewLabel,
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));
    QObject::connect(ui->subtileViewLabel,
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(complexViewMouseEvent(QMouseEvent*)));

    refreshDisplay();
}

MetatileEditorWidget::~MetatileEditorWidget()
{
    delete ui;
}

void MetatileEditorWidget::refreshDisplay() {
    refreshViewLabels();
    refreshBoxes();
    refreshActiveMetatile();
}

void MetatileEditorWidget::reload() {
    //

    refreshDisplay();
}

void MetatileEditorWidget::refreshViewLabels() {
    metatileView_.drawViews();

    metatilePickerCache_.directInit(metatileView_.metatilePickerView());
    tilePickerCache_.directInit(metatileView_.tilePickerView());
    subtileViewCache_.directInit(metatileView_.subtilePickerView());
//    tilePickerCache_.drawGraphic(metatileView_.tilePickerView());

    // Convert cache to pixmap and attach to display label
    ui->metatilePickerLabel->setPixmap(
                QPixmap().fromImage(metatilePickerCache_.image()));
    ui->tilePickerLabel->setPixmap(
                QPixmap().fromImage(tilePickerCache_.image()));
    ui->subtileViewLabel->setPixmap(
                QPixmap().fromImage(subtileViewCache_.image()));

    // Qt is too dumb to properly account for the width of the vertical scroll bar when
    // determining the width of the scroll area contents, so we have to add a small constant
    // to make sure there's enough room for it. Otherwise, we'll end up with an ugly
    // horizontal scroll bar in the metatile picker.
    ui->metatilePickerScrollArea->setMinimumWidth(
                metatilePickerCache_.image().width() + 32);
    ui->metatilePickerScrollArea->setMaximumWidth(
                metatilePickerCache_.image().width() + 32);
}

void MetatileEditorWidget::refreshBoxes() {
    ui->metatileZoomBox->setChecked(metatileView_.metatilePickerZoom());
    ui->gridBox->setChecked(metatileView_.gridEnabled());

    switch (metatileView_.viewType()) {
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

    if (metatileView_.numMetatiles() >= metatileView_.maxNumMetatiles()) {
        ui->metatileAddButton->setEnabled(false);
    }
    else {
        ui->metatileAddButton->setEnabled(true);
    }

    if (metatileView_.numMetatiles() <= metatileView_.minNumMetatiles()) {
        ui->metatileRemoveButton->setEnabled(false);
    }
    else {
        ui->metatileRemoveButton->setEnabled(true);
    }
}

void MetatileEditorWidget::refreshActiveMetatile() {
    LaylaMetatile& metatile
            = metatileView_.activeMetatile();
    ui->paletteBox->setCurrentIndex(metatile.palette);
    ui->behaviorBox->setCurrentIndex(metatile.behavior);
    ui->topSolidBox->setChecked(metatile.topSolid);
    ui->bottomSolidBox->setChecked(metatile.bottomSolid);

    ui->metatileEditStackedWidget->editMetatile(metatile);
}

void MetatileEditorWidget::simpleViewMouseEvent() {
    refreshDisplay();
}

void MetatileEditorWidget::complexViewMouseEvent(QMouseEvent*) {
    refreshDisplay();
}

void MetatileEditorWidget::on_areaBox_activated(int index)
{
    metatileView_.changeArea(
                ui->areaBox->itemData(index, Qt::UserRole).toInt());
    refreshDisplay();
}

void MetatileEditorWidget::on_gridBox_clicked(bool checked)
{
    metatileView_.setGridEnabled(checked);
    refreshDisplay();
}

void MetatileEditorWidget::on_metatileZoomBox_clicked(bool checked)
{
    metatileView_.setMetatilePickerZoom(checked);
    refreshDisplay();
}

void MetatileEditorWidget::on_visualButton_clicked(bool checked)
{
    metatileView_.setViewType(MetatileViewTypes::visual);
    refreshDisplay();
}

void MetatileEditorWidget::on_behaviorButton_clicked(bool checked)
{
    metatileView_.setViewType(MetatileViewTypes::behavior);
    refreshDisplay();
}

void MetatileEditorWidget::on_solidityButton_clicked(bool checked)
{
    metatileView_.setViewType(MetatileViewTypes::solidity);
    refreshDisplay();
}

void MetatileEditorWidget::on_paletteBox_activated(int index)
{
    metatileView_.changeActiveMetatilePalette(
                ui->paletteBox->itemData(index, Qt::UserRole).toInt());
    refreshDisplay();
}

void MetatileEditorWidget::on_behaviorBox_activated(int index)
{
    metatileView_.changeActiveMetatileBehavior(
                static_cast<LaylaMetatileBehaviors::LaylaMetatileBehavior>(
                    ui->behaviorBox->itemData(index, Qt::UserRole).toInt()));
    refreshDisplay();
}

void MetatileEditorWidget::on_topSolidBox_clicked(bool checked)
{
    metatileView_.changeActiveMetatileTopSolid(checked);
    refreshDisplay();
}

void MetatileEditorWidget::on_bottomSolidBox_clicked(bool checked)
{
    metatileView_.changeActiveMetatileBottomSolid(checked);
    refreshDisplay();
}

void MetatileEditorWidget::on_metatileRemoveButton_clicked()
{
    metatileView_.removeActiveMetatile();
    refreshDisplay();
}

void MetatileEditorWidget::on_metatileAddButton_clicked()
{
    metatileView_.addMetatile();
    refreshDisplay();
}
