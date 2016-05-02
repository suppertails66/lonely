#include "layouteditorwidget.h"
#include "ui_layouteditorwidget.h"
#include "lonelyglobals.h"
#include "qtformatconversion.h"
#include "cachedqimage.h"
#include "blankedwidget.h"
#include "spawnpointedwidget.h"
#include "spawnbossedwidget.h"
#include "setscrollingedwidget.h"
#include "setareaedwidget.h"
#include "setmapedwidget.h"
#include "setelevatorsedwidget.h"
#include "jumptopositionedwidget.h"
#include "addnodeentrydialog.h"
#include "util/StringConversion.h"
#include "editor/LonelyLevelViewManager.h"
#include "editor/LonelyControlViewManager.h"
#include <string>
#include <iostream>

using namespace Lonely;

LayoutEditorWidget::LayoutEditorWidget(
        QWidget *parent) :
    EditorWidget(parent),
    ui(new Ui::LayoutEditorWidget),
//    toolsToolBar_(new QToolBar("Tools", this)),
    levelEditor_(globals_.editor().levelEditor()),
    levelView_(levelEditor_.levelView())
{
    ui->setupUi(this);

/*    toolsToolBar_->addAction(ui->modifyToolAction);
    toolsToolBar_->addAction(ui->copyToolAction);
    toolsToolBar_->addAction(ui->moveToolAction);
//    ui->toolBarWidget->layout()->addWidget(toolsToolBar_);
    QBoxLayout* toolBarLayout = new QBoxLayout(QBoxLayout::LeftToRight, ui->toolBarWidget);
    toolBarLayout->addWidget(toolsToolBar_);
    toolBarLayout->addStretch(0);
    ui->toolBarWidget->setLayout(toolBarLayout);
//    ui->toolBarWidget->layout()->addStretch(0); */

    ui->levelViewWidget->levelViewLabel().installEventFilter(this);
    ui->levelViewWidget->controlViewLabel().installEventFilter(this);

    levelView_.setControlCallbacks((void*)this,
                                   nodeSelectedCallback,
                                   nodeDeselectedCallback,
                                   spawnsChangedCallback);

    ui->levelViewWidget->levelViewLabel().setCallbacks(
                (void*)(&levelView_),
                LonelyLevelViewManager::enterEvent,
                LonelyLevelViewManager::leaveEvent,
                LonelyLevelViewManager::mouseMoveEvent,
                LonelyLevelViewManager::mousePressEvent,
                LonelyLevelViewManager::mouseReleaseEvent,
                LonelyLevelViewManager::mouseDoubleClickEvent);

    ui->levelViewWidget->controlViewLabel().setCallbacks(
                (void*)(&levelView_),
                LonelyControlViewManager::enterEvent,
                LonelyControlViewManager::leaveEvent,
                LonelyControlViewManager::mouseMoveEvent,
                LonelyControlViewManager::mousePressEvent,
                LonelyControlViewManager::mouseReleaseEvent,
                LonelyControlViewManager::mouseDoubleClickEvent);

    QObject::connect(&(ui->levelViewWidget->levelViewLabel()),
                     SIGNAL(entered(QEvent*)),
                     this,
                     SLOT(onLevelEnter(QEvent*)));
    QObject::connect(&(ui->levelViewWidget->levelViewLabel()),
                     SIGNAL(left(QEvent*)),
                     this,
                     SLOT(onLevelLeave(QEvent*)));
    QObject::connect(&(ui->levelViewWidget->levelViewLabel()),
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(onLevelPress(QMouseEvent*)));
    QObject::connect(&(ui->levelViewWidget->levelViewLabel()),
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(onLevelMove(QMouseEvent*)));
    QObject::connect(&(ui->levelViewWidget->levelViewLabel()),
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(onLevelRelease(QMouseEvent*)));
    QObject::connect(&(ui->levelViewWidget->levelViewLabel()),
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(onLevelDoubleClick(QMouseEvent*)));

    QObject::connect(&(ui->levelViewWidget->controlViewLabel()),
                     SIGNAL(entered(QEvent*)),
                     this,
                     SLOT(onControlEnter(QEvent*)));
    QObject::connect(&(ui->levelViewWidget->controlViewLabel()),
                     SIGNAL(left(QEvent*)),
                     this,
                     SLOT(onControlLeave(QEvent*)));
    QObject::connect(&(ui->levelViewWidget->controlViewLabel()),
                     SIGNAL(mousePressed(QMouseEvent*)),
                     this,
                     SLOT(onControlPress(QMouseEvent*)));
    QObject::connect(&(ui->levelViewWidget->controlViewLabel()),
                     SIGNAL(mouseMoved(QMouseEvent*)),
                     this,
                     SLOT(onControlMove(QMouseEvent*)));
    QObject::connect(&(ui->levelViewWidget->controlViewLabel()),
                     SIGNAL(mouseReleased(QMouseEvent*)),
                     this,
                     SLOT(onControlRelease(QMouseEvent*)));
    QObject::connect(&(ui->levelViewWidget->controlViewLabel()),
                     SIGNAL(mouseDoubleClicked(QMouseEvent*)),
                     this,
                     SLOT(onControlDoubleClick(QMouseEvent*)));

    QObject::connect(&(ui->levelViewWidget->horizontalScrollBar()),
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(onHorizontalScroll(int)));

    QObject::connect(&(ui->levelViewWidget->verticalScrollBar()),
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(onVerticalScroll(int)));

    QObject::connect(&(ui->controlNodeList->list()),
                     SIGNAL(itemClicked(QListWidgetItem*)),
                     this,
                     SLOT(onNodeBoxSelectionChanged(
                              QListWidgetItem*)));
    QObject::connect((ui->controlNodeList),
                     SIGNAL(addButtonClicked()),
                     this,
                     SLOT(onNodeBoxAddButtonClicked()));
    QObject::connect((ui->controlNodeList),
                     SIGNAL(removeButtonClicked()),
                     this,
                     SLOT(onNodeBoxRemoveButtonClicked()));

    QObject::connect(&(ui->spawnList->list()),
                     SIGNAL(itemClicked(QListWidgetItem*)),
                     this,
                     SLOT(onSpawnListSelectionChanged(
                              QListWidgetItem*)));
    QObject::connect((ui->spawnList),
                     SIGNAL(addButtonClicked()),
                     this,
                     SLOT(onSpawnListAddButtonClicked()));
    QObject::connect((ui->spawnList),
                     SIGNAL(removeButtonClicked()),
                     this,
                     SLOT(onSpawnListRemoveButtonClicked()));

/*    ui->zoomBox->addItem("25%", (double)0.25);
    ui->zoomBox->addItem("50%", (double)0.50);
    ui->zoomBox->addItem("100%", (double)1.00);
    ui->zoomBox->addItem("200%", (double)2.00);
    ui->zoomBox->addItem("300%", (double)3.00);
    ui->zoomBox->addItem("400%", (double)4.00);
    ui->zoomBox->setCurrentIndex(2); */

/*    for (int i = 0;
         i < LaylaPatternTypes::numLaylaPatternTypes;
         i++) {
        LaylaPatternTypes::LaylaPatternType patternType
                = static_cast<LaylaPatternTypes::LaylaPatternType>(i);

        switch (patternType) {
        case LaylaPatternTypes::spawnBoss:
            ui->nodeEditorStackedWidget->addWidget(
                        new SpawnBossEdWidget());
            break;
        case LaylaPatternTypes::setScrolling:
            ui->nodeEditorStackedWidget->addWidget(
                        new SetScrollingEdWidget());
            break;
        case LaylaPatternTypes::setArea:
            ui->nodeEditorStackedWidget->addWidget(
                        new SetAreaEdWidget());
            break;
        case LaylaPatternTypes::setMap:
            ui->nodeEditorStackedWidget->addWidget(
                        new SetMapEdWidget());
            break;
        case LaylaPatternTypes::setElevators:
            ui->nodeEditorStackedWidget->addWidget(
                        new SetElevatorsEdWidget());
            break;
        case LaylaPatternTypes::jumpToPosition:
            ui->nodeEditorStackedWidget->addWidget(
                        new JumpToPositionEdWidget());
            break;

        case LaylaPatternTypes::spawnPoint:
        case LaylaPatternTypes::standardPattern:
        default:
            ui->nodeEditorStackedWidget->addWidget(
                        new BlankEdWidget());
            break;
        }
    } */

/*    ui->patternListWidget->addItem("1");
    ui->patternListWidget->addItem("2");
    ui->patternListWidget->addItem("3");
    QImage image(128, 128, QImage::Format_ARGB32);
    ui->patternListWidget->item(0)->setIcon(
                QIcon(QPixmap().fromImage(image)));
    ui->patternListWidget->item(1)->setIcon(
                QIcon(QPixmap().fromImage(image)));
    ui->patternListWidget->item(2)->setIcon(
                QIcon(QPixmap().fromImage(image)));
    for (int i = 0; i < 128; i++) {
        ui->patternListWidget->addItem("z");
        ui->patternListWidget->item(i + 3)->setIcon(
                    QIcon(QPixmap().fromImage(image)));
    }
    ui->patternListWidget->setIconSize(QSize(48, 112));
    ui->patternListWidget->setGridSize(QSize(48, 128));
    ui->patternListWidget->setViewMode(QListWidget::IconMode);
    ui->patternListWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
    ui->patternListWidget->setMovement(QListWidget::Static);
    ui->patternListWidget->setFlow(QListWidget::LeftToRight);
    ui->patternListWidget->setWrapping(true); */
    ui->patternListWidget->setLevelEditor(levelEditor_);

    deselectNode();

    refreshToolButtons();

    refreshBoxes();

    refreshPatternList();
}

LayoutEditorWidget::~LayoutEditorWidget()
{
    delete ui;

//    delete toolsToolBar_;
}

void LayoutEditorWidget::loadLevel(int levelNum) {
    levelView_.setCamW(ui->levelViewWidget->levelViewLabel().width());
    levelView_.setCamH(ui->levelViewWidget->levelViewLabel().height());
    refreshPatternList();
    refreshSelectedPattern();
    refreshSpawnList();

//    refreshScrollBars();

//    refreshDisplay();
}

void LayoutEditorWidget::refreshDisplay() {
    refreshLevelDisplay();
    refreshScrollBars();
    refreshSelectedPattern();
    refreshBoxes();
    refreshToolButtons();
    refreshNodeEditorWidget();
}

void LayoutEditorWidget::reload() {
    levelView_.reloadRefresh();
    loadLevel(levelEditor_.currentLevelNum());
    refreshDisplay();
}


void LayoutEditorWidget::onHorizontalScroll(int val) {
    levelView_.seekCamX(val);
    refreshLevelDisplay();
}

void LayoutEditorWidget::onVerticalScroll(int val) {
    levelView_.seekCamY(val);
    refreshLevelDisplay();
}

void LayoutEditorWidget::resizeEvent(QResizeEvent* event) {
    refreshLevelDisplay();
    refreshScrollBars();
}

void LayoutEditorWidget::refreshLevelDisplay() {
    // Resize the camera to the area of the display labels
    levelView_.setCamW(ui->levelViewWidget->levelViewLabel().width());
    levelView_.setCamH(ui->levelViewWidget->levelViewLabel().height());
//    controlView_.setCamW(ui->levelViewWidget->controlViewLabel().width());
//    controlView_.setCamH(ui->levelViewWidget->controlViewLabel().height());

    // Draw the level
    levelView_.drawViews();
//    levelView_.drawLevelView();
//    levelView_.drawControlView();

    // Update QImage cache
    levelViewImage_.directInit(levelView_.levelView());
    controlViewImage_.directInit(levelView_.controlView());

    // Convert cache to pixmap and attach to display label
    ui->levelViewWidget->levelViewLabel().setPixmap(
                QPixmap().fromImage(levelViewImage_.image()));
    ui->levelViewWidget->controlViewLabel().setPixmap(
                QPixmap().fromImage(controlViewImage_.image()));
}

void LayoutEditorWidget::refreshScrollBars() {
    int w = levelView_.levelNativeWidth();
    int h = levelView_.levelNativeHeight();

//    std::cout << levelView_.camH() << std::endl;

    ui->levelViewWidget->horizontalScrollBar().setRange(
                0, w - levelView_.realCamW());
    ui->levelViewWidget->horizontalScrollBar().setPageStep(
                levelView_.camW() * 2 / 3 / levelView_.zoom());
    ui->levelViewWidget->horizontalScrollBar().setSingleStep(
                LaylaMetatile::width);

    ui->levelViewWidget->verticalScrollBar().setRange(
                0, h - levelView_.realCamH());
    ui->levelViewWidget->verticalScrollBar().setPageStep(
                levelView_.camH() * 2 / 3 / levelView_.zoom());
    ui->levelViewWidget->verticalScrollBar().setSingleStep(
                LaylaMetatile::height);

    ui->levelViewWidget->horizontalScrollBar().setValue(
                levelView_.camX());
    ui->levelViewWidget->verticalScrollBar().setValue(
                levelView_.camY());

    if (levelView_.realCamW() >= w) {
        ui->levelViewWidget->horizontalScrollBar().setVisible(false);
    }
    else {
        ui->levelViewWidget->horizontalScrollBar().setVisible(true);
    }

    if (levelView_.realCamH() >= h) {
        ui->levelViewWidget->verticalScrollBar().setVisible(false);
    }
    else {
        ui->levelViewWidget->verticalScrollBar().setVisible(true);
    }
}

void LayoutEditorWidget::on_zoomBox_activated(int index)
{
    levelView_.setZoom(ui->zoomBox->itemData(index).toDouble());
    refreshDisplay();
}

void LayoutEditorWidget::on_gridBox_clicked(bool checked)
{
    levelView_.setGridEnabled(checked);
    refreshDisplay();
}

void LayoutEditorWidget::on_visualButton_clicked()
{
    levelView_.setViewType(LonelyLevelView::visual);
    refreshDisplay();
}

void LayoutEditorWidget::on_behaviorButton_clicked()
{
    levelView_.setViewType(LonelyLevelView::behavior);
    refreshDisplay();
}

void LayoutEditorWidget::on_solidityButton_clicked()
{
    levelView_.setViewType(LonelyLevelView::solidity);
    refreshDisplay();
}

void LayoutEditorWidget::on_patternMarkersBox_clicked(bool checked)
{
    levelView_.setPatternMarkersEnabled(checked);
    refreshDisplay();
}

void LayoutEditorWidget::on_loopsBox_clicked(bool checked)
{
    levelView_.setLoopsEnabled(checked);
    refreshDisplay();
}

void LayoutEditorWidget::refreshPatternList() {
    ui->patternListWidget->refreshPatternList();
}

void LayoutEditorWidget::refreshSelectedPattern() {
    if (levelView_.hasSelectedPattern()) {
        ui->patternListWidget->setCurrentRow(levelView_.selectedPatternNum());
/*        ui->patternListWidget->selectionModel()->setCurrentIndex(
            ui->patternListWidget->model()->index(0,
                                                  levelView_.selectedPatternNum()),
                    QItemSelectionModel::Select);*/
    }
    else {
//        ui->patternListWidget->setCurrentRow(-1, QItemSelectionModel::Clear);
        ui->patternListWidget->clearSelection();
//        ui->patternListWidget->selectionModel()->clearSelection();
    }
}

void LayoutEditorWidget::refreshBoxes() {
    ui->gridBox->setChecked(levelView_.gridEnabled());
    ui->patternMarkersBox->setChecked(levelView_.patternMarkersEnabled());
    ui->loopsBox->setChecked(levelView_.loopsEnabled());
    ui->spawnsBox->setChecked(levelView_.spawnsEnabled());

    if (levelView_.overwriteEnabled()) {
        ui->overwriteButton->setChecked(true);
    }
    else {
        ui->insertButton->setChecked(true);
    }

    if (levelView_.copyNodesEnabled()) {
        ui->copyNodesButton->setChecked(levelView_.copyNodesEnabled());
    }

    switch(levelView_.viewType()) {
    case LonelyLevelView::visual:
        ui->visualButton->setChecked(true);
        break;
    case LonelyLevelView::behavior:
        ui->behaviorButton->setChecked(true);
        break;
    case LonelyLevelView::solidity:
        ui->solidityButton->setChecked(true);
        break;
    default:
        break;
    }

    if (levelView_.toolOverwriteAvailable()) {
        ui->insertOverwriteWidget->setEnabled(true);
    }
    else {
        ui->insertOverwriteWidget->setEnabled(false);
    }

    if (levelView_.toolCopyNodesAvailable()) {
        ui->copyNodesButton->setEnabled(true);
    }
    else {
        ui->copyNodesButton->setEnabled(false);
    }
}

void LayoutEditorWidget::refreshNodeEditorWidget() {
    if (!(levelView_.nodeSelected())) {
        ui->nodeEditorStackedWidget->setCurrentIndex(
                    LaylaPatternTypes::none);
        ui->controlNodeList->addButton().setEnabled(false);
        ui->controlNodeList->removeButton().setEnabled(false);
    }
    else {
        ui->controlNodeList->addButton().setEnabled(true);

        if (ui->controlNodeList->list().currentRow() < 0) {
            ui->controlNodeList->removeButton().setEnabled(false);
            ui->nodeUpButton->setEnabled(false);
            ui->nodeDownButton->setEnabled(false);
        }
        else {
            ui->controlNodeList->removeButton().setEnabled(true);
            ui->nodeUpButton->setEnabled(true);
            ui->nodeDownButton->setEnabled(true);
        }
/*        ui->nodeEditorStackedWidget->setCurrentIndex(
                    levelView_.patternToken(
                        ui->controlNodeList->list().currentItem()
                        ->data(Qt::UserRole).toInt()).type()); */
    }
}

void LayoutEditorWidget::refreshSpawnList() {
    ui->spawnList->list().clear();

    Lonely::TokenIndexCollection spawnList = levelView_.listOfSpawns();

    for (int i = 0; i < spawnList.size(); i++) {
        ui->spawnList->list().addItem(
                    new QListWidgetItem());
        ui->spawnList->list().item(i)->setText(
                    (std::string("Spawn ")
                     + StringConversion::toString(i)
                     + " ("
                     + StringConversion::toString(spawnList[i])
                     + ")").c_str());
        ui->spawnList->list().item(i)->setData(
                    Qt::UserRole, i);
    }
}

void LayoutEditorWidget::refreshToolButtons() {
    // Raise all buttons
    ui->modifyToolButton->setChecked(false);
    ui->copyToolButton->setChecked(false);
    ui->moveToolButton->setChecked(false);

    switch (levelView_.currentTool()) {
    case LonelyLevelViewManager::patternModify:
        ui->modifyToolButton->setChecked(true);
        break;
    case LonelyLevelViewManager::patternCopy:
        ui->copyToolButton->setChecked(true);
        break;
    case LonelyLevelViewManager::patternMove:
        ui->moveToolButton->setChecked(true);
        break;
    default:
        break;
    }
}

void LayoutEditorWidget::refreshNodeList() {
    // Refresh item names
    for (int i = 0; i < ui->controlNodeList->list().count(); i++) {
        ui->controlNodeList->list().item(i)->setText(
                    levelView_.patternToken(
                        ui->controlNodeList->list().item(i)->data(Qt::UserRole).toInt())
                    .descriptionString().c_str());
    }

    if (ui->controlNodeList->list().selectedItems().size() != 1) {
        return;
    }

    // Reload widget
    LaylaPatternToken& patternToken
            = levelView_.patternToken(
                ui->controlNodeList->list().selectedItems()[0]->data(Qt::UserRole).toInt());
//    LaylaPatternTypes::LaylaPatternType patternType
//            = patternToken.type();
    ui->nodeEditorStackedWidget->setToken(&patternToken);
}

void LayoutEditorWidget::on_spawnsBox_clicked(bool checked)
{
    levelView_.setSpawnsEnabled(checked);
    refreshDisplay();
}

void LayoutEditorWidget::onLevelEnter(QEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onLevelLeave(QEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onLevelPress(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onLevelMove(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onLevelRelease(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onLevelDoubleClick(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onControlEnter(QEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onControlLeave(QEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onControlPress(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onControlMove(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onControlRelease(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onControlDoubleClick(QMouseEvent* event) {
    refreshDisplay();
}

void LayoutEditorWidget::onNodeBoxSelectionChanged(QListWidgetItem* item) {
    refreshNodeList();

    refreshDisplay();
}

void LayoutEditorWidget::onNodeBoxAddButtonClicked() {
    if (!(levelView_.nodeSelected())) {
        return;
    }

    AddNodeEntryDialog dialog(this);
    int result = dialog.exec();

    if ((result == QDialog::Accepted)) {
        if ((dialog.token() != NULL)) {
            levelView_.appendTokenToSelectedNode(dialog.token());
        }
    }
//    else {
//    }

    delete (dialog.token());

    refreshDisplay();
}

void LayoutEditorWidget::onNodeBoxRemoveButtonClicked() {
    if ((levelView_.nodeSelected())
            && (ui->controlNodeList->list().count() > 0)
            && (ui->controlNodeList->list().selectedItems().count() > 0)) {
        levelView_.removeTokenFromSelectedNode(
                    ui->controlNodeList->list().currentItem()
                        ->data(Qt::UserRole).toInt());
    }

    refreshDisplay();
}

void LayoutEditorWidget::onSpawnListSelectionChanged(QListWidgetItem* item) {
    levelView_.seekSpawn(item->data(Qt::UserRole).toInt());

    refreshDisplay();
}

void LayoutEditorWidget::onSpawnListAddButtonClicked() {
    levelView_.addSpawnAtSelectedNode();

    refreshDisplay();
}

void LayoutEditorWidget::onSpawnListRemoveButtonClicked() {
    levelView_.deleteSelectedNode();

    refreshDisplay();
}

void LayoutEditorWidget::on_patternListWidget_itemClicked(QListWidgetItem *item)
{
    levelView_.setSourcePattern(item->data(Qt::UserRole).toInt());
    changeTool(LonelyLevelViewManager::patternModify);
}

void LayoutEditorWidget::on_overwriteButton_clicked()
{
    levelView_.setOverwriteEnabled(true);
    refreshDisplay();
}

void LayoutEditorWidget::on_insertButton_clicked()
{
    levelView_.setOverwriteEnabled(false);
    refreshDisplay();
}

void LayoutEditorWidget::on_copyNodesButton_clicked(bool checked)
{
    levelView_.setCopyNodesEnabled(checked);
    refreshDisplay();
}

void LayoutEditorWidget::nodeSelectedCallback(
                                void* layoutEditorWidget,
                                 TokenIndexCollection& tokens) {
    ((LayoutEditorWidget*)(layoutEditorWidget))->selectNode(tokens);
}

void LayoutEditorWidget::nodeDeselectedCallback(
                                void* layoutEditorWidget) {
    ((LayoutEditorWidget*)(layoutEditorWidget))->deselectNode();
}

void LayoutEditorWidget::spawnsChangedCallback(
        void* layoutEditorWidget) {
    ((LayoutEditorWidget*)(layoutEditorWidget))->refreshSpawnList();
}

void LayoutEditorWidget::changeTool(Lonely::LonelyLevelViewManager::Tool tool__) {
    levelView_.changeTool(tool__);

    refreshDisplay();
}

void LayoutEditorWidget::selectNode(
        Lonely::TokenIndexCollection& tokens) {
    // Deselect current node
    deselectNode();
//    ui->controlNodeList->list().clear();

    ui->controlNodeLabel->setText(
                (std::string("Node position: ")
                 + StringConversion::toString(
                     levelView_.selectedNodeIndex())).c_str());

    // Add item for each token
    for (int i = 0; i < tokens.size(); i++) {
        ui->controlNodeList->list().addItem(
                    new QListWidgetItem(
                        StringConversion::toString(i).c_str(),
                        &(ui->controlNodeList->list())));
        ui->controlNodeList->list().item(i)->setText(
                    levelView_.patternToken(tokens[i])
                        .descriptionString().c_str());
        ui->controlNodeList->list().item(i)->setData(
                    Qt::UserRole, tokens[i]);


//        ui->controlNodeList->list().item(i)->setText(
//                    StringConversion::toString(i).c_str());

//        ui->controlNodeList->list().item(i)->setData(
//                    0, tokens[i]);
    }

    ui->spawnList->addButton().setEnabled(true);

    if (ui->controlNodeList->list().count() > 0) {
        ui->spawnList->removeButton().setEnabled(true);
    }
    else {
        ui->spawnList->removeButton().setEnabled(false);
    }
}

void LayoutEditorWidget::deselectNode() {
    ui->controlNodeList->list().clear();
    ui->nodeEditorStackedWidget->setCurrentIndex(LaylaPatternTypes::none);
    ui->controlNodeLabel->setText("No node selected");
    ui->spawnList->addButton().setEnabled(false);
    ui->spawnList->removeButton().setEnabled(false);
}

void LayoutEditorWidget::incrementZoomLevel() {
    if (ui->zoomBox->currentIndex() >= (ui->zoomBox->count() - 1)) {
        return;
    }

    ui->zoomBox->setCurrentIndex(
                ui->zoomBox->currentIndex() + 1);

    levelView_.setZoom(ui->zoomBox->itemData(
                           ui->zoomBox->currentIndex()).toDouble());
    refreshDisplay();
}

void LayoutEditorWidget::decrementZoomLevel() {
    if (ui->zoomBox->currentIndex() <= 0) {
        return;
    }

    ui->zoomBox->setCurrentIndex(
                ui->zoomBox->currentIndex() - 1);

    levelView_.setZoom(ui->zoomBox->itemData(
                           ui->zoomBox->currentIndex()).toDouble());
    refreshDisplay();
}

void LayoutEditorWidget::on_modifyToolButton_clicked()
{
    changeTool(LonelyLevelViewManager::patternModify);
}

void LayoutEditorWidget::on_copyToolButton_clicked()
{
    changeTool(LonelyLevelViewManager::patternCopy);
}

void LayoutEditorWidget::on_moveToolButton_clicked()
{
    changeTool(LonelyLevelViewManager::patternMove);
}

void LayoutEditorWidget::on_nodeUpButton_clicked()
{
    if (ui->controlNodeList->list().selectedItems().size() != 1) {
        return;
    }

    bool result = levelView_.moveNodeItemUp(
                ui->controlNodeList->list().selectedItems()[0]->data(Qt::UserRole).toInt());

    if (result) {
        ui->controlNodeList->list().setCurrentRow(
                    ui->controlNodeList->list().currentRow() - 1);
    }

    refreshNodeList();
    refreshDisplay();
}

void LayoutEditorWidget::on_nodeDownButton_clicked()
{
    if (ui->controlNodeList->list().selectedItems().size() != 1) {
        return;
    }

    bool result = levelView_.moveNodeItemDown(
                ui->controlNodeList->list().selectedItems()[0]->data(Qt::UserRole).toInt());

    if (result) {
        ui->controlNodeList->list().setCurrentRow(
                    ui->controlNodeList->list().currentRow() + 1);
    }

    refreshNodeList();
    refreshDisplay();
}

bool LayoutEditorWidget::eventFilter(QObject* obj, QEvent* event) {
    // Intercept wheel events for the level view labels and send them
    // to the scroll bars instead
    if ((obj == &(ui->levelViewWidget->levelViewLabel()))
            || (obj == &(ui->levelViewWidget->controlViewLabel()))) {
        if (event->type() == QEvent::Wheel) {
            QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

            if ((wheelEvent->modifiers() & Qt::ControlModifier)
                    && (!(wheelEvent->modifiers() & Qt::ShiftModifier))) {
//                Qt::KeyboardModifiers mods = wheelEvent->modifiers();
//                mods ^= Qt::ShiftModifier;
//                wheelEvent->setModifiers(mods);
                if (wheelEvent->delta() > 0) {
                    incrementZoomLevel();
                }
                else if (wheelEvent->delta() < 0) {
                    decrementZoomLevel();
                }

//                QCoreApplication::sendEvent(ui->zoomBox,
//                                            wheelEvent);
                return true;
            }
            if (wheelEvent->modifiers() & Qt::ShiftModifier) {
                // QScrollBar's default implementation of the WheelEvent handler
                // uses the Shift modifier to indicate "faster" scrolling. I
                // prefer to use it to distinguish between horizontal and vertical
                // scrolling, so we have to clear the Shift modifier before passing
                // the event on.
                Qt::KeyboardModifiers mods = wheelEvent->modifiers();
                mods ^= Qt::ShiftModifier;
                wheelEvent->setModifiers(mods);

                QCoreApplication::sendEvent(&(ui->levelViewWidget->horizontalScrollBar()),
                                            wheelEvent);
                return true;
            }
            else {
                QCoreApplication::sendEvent(&(ui->levelViewWidget->verticalScrollBar()),
                                            wheelEvent);
                return true;
            }
        }
        else {
            return false;
        }
    }
    else {
        return QObject::eventFilter(obj, event);
    }
}
