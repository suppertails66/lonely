#ifndef LAYOUTEDITORWIDGET_H
#define LAYOUTEDITORWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QToolBar>
#include "editorwidget.h"
#include "editor/LonelyLevelEditor.h"
#include "editor/LonelyLevelView.h"
#include "gamedata/LaylaArea.h"
#include "cachedqimage.h"
#include <cstdlib>

namespace Ui {
class LayoutEditorWidget;
}

class LayoutEditorWidget : public EditorWidget
{
    Q_OBJECT
    
public:
    explicit LayoutEditorWidget(
            QWidget *parent = 0);
    ~LayoutEditorWidget();

    void loadLevel(int levelNum);

    virtual void refreshDisplay();

    virtual void reload();
    
private:
    Ui::LayoutEditorWidget *ui;

private slots:
    void onHorizontalScroll(int val);
    void onVerticalScroll(int val);

    void onLevelEnter(QEvent* event);
    void onLevelLeave(QEvent* event);
    void onLevelPress(QMouseEvent* event);
    void onLevelMove(QMouseEvent* event);
    void onLevelRelease(QMouseEvent* event);
    void onLevelDoubleClick(QMouseEvent* event);

    void onControlEnter(QEvent* event);
    void onControlLeave(QEvent* event);
    void onControlPress(QMouseEvent* event);
    void onControlMove(QMouseEvent* event);
    void onControlRelease(QMouseEvent* event);
    void onControlDoubleClick(QMouseEvent* event);

    void onNodeBoxSelectionChanged(QListWidgetItem* item);
    void onNodeBoxAddButtonClicked();
    void onNodeBoxRemoveButtonClicked();

    void onSpawnListSelectionChanged(QListWidgetItem* item);
    void onSpawnListAddButtonClicked();
    void onSpawnListRemoveButtonClicked();

    void on_zoomBox_activated(int index);

    void on_gridBox_clicked(bool checked);

    void on_visualButton_clicked();

    void on_behaviorButton_clicked();

    void on_solidityButton_clicked();

    void on_patternMarkersBox_clicked(bool checked);

    void on_loopsBox_clicked(bool checked);

    void on_spawnsBox_clicked(bool checked);

    void on_patternListWidget_itemClicked(QListWidgetItem *item);

    void on_overwriteButton_clicked();

    void on_insertButton_clicked();

    void on_copyNodesButton_clicked(bool checked);

    void on_modifyToolButton_clicked();

    void on_copyToolButton_clicked();

    void on_moveToolButton_clicked();

    void on_nodeUpButton_clicked();

    void on_nodeDownButton_clicked();

protected:
//    QToolBar* toolsToolBar_;

    Lonely::LonelyLevelEditor& levelEditor_;
    Lonely::LonelyLevelView& levelView_;

    CachedQImage levelViewImage_;
    CachedQImage controlViewImage_;

    static void nodeSelectedCallback(
            void* layoutEditorWidget,
            Lonely::TokenIndexCollection& tokens);

    static void nodeDeselectedCallback(
            void* layoutEditorWidget);

    static void spawnsChangedCallback(
            void* layoutEditorWidget);

    void changeTool(Lonely::LonelyLevelViewManager::Tool tool__);

    void selectNode(Lonely::TokenIndexCollection& tokens);
    void deselectNode();

    void incrementZoomLevel();
    void decrementZoomLevel();

    void resizeEvent(QResizeEvent* event);

    bool eventFilter(QObject* obj, QEvent* event);

    void refreshLevelDisplay();

    void refreshScrollBars();

    void refreshPatternList();

    void refreshSelectedPattern();

    void refreshBoxes();

    void refreshNodeEditorWidget();

    void refreshSpawnList();

    void refreshToolButtons();

    void refreshNodeList();
};

#endif // LAYOUTEDITORWIDGET_H
