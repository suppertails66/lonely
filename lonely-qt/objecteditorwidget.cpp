#include "objecteditorwidget.h"
#include "ui_objecteditorwidget.h"
#include <QScrollBar>
#include "addobjectpatterndialog.h"
#include "lonelyglobals.h"
#include "util/StringConversion.h"
#include <cstdlib>

using namespace Lonely;

ObjectEditorWidget::ObjectEditorWidget(QWidget *parent) :
    EditorWidget(parent),
    levelEditor_(globals_.editor().levelEditor()),
    objectView_(globals_.editor().levelEditor().objectView()),
    ui(new Ui::ObjectEditorWidget)
{
    ui->setupUi(this);

    for (int i = 0; i < objectView_.numObjectSets(); i++) {
        ui->objectSetBox->addItem(StringConversion::toString(i).c_str(),
                                  i);
    }

    reload();
}

ObjectEditorWidget::~ObjectEditorWidget()
{
    delete ui;
}

void ObjectEditorWidget::refreshDisplay() {
    refreshBoxes();
}

void ObjectEditorWidget::reload() {
    reloadObjectPatternList();
    refreshObjectEditStackedWidget();
    refreshDisplay();
}

void ObjectEditorWidget::refreshObjectPatternList() {
//    int oldScroll = ui->objectPatternListWidget->verticalScrollBar()->value();

    ui->objectPatternListWidget->refreshItems(objectView_.objectPatterns(),
                                                objectView_.currentLevelNum(),
                                                objectView_.currentObjectSetNum(),
                                                objectView_.graphics(),
                                                objectView_.palettes());

//    ui->objectPatternListWidget->scroll(0, oldScroll);
//    ui->objectPatternListWidget->verticalScrollBar()->setValue(oldScroll);
}

void ObjectEditorWidget::reloadObjectPatternList() {
    ui->objectPatternListWidget->repopulate(objectView_.objectPatterns(),
                                                objectView_.currentLevelNum(),
                                                objectView_.currentObjectSetNum(),
                                                objectView_.graphics(),
                                                objectView_.palettes());
}

void ObjectEditorWidget::refreshBoxes() {
    ui->objectSetBox->setCurrentIndex(
                objectView_.currentObjectSetNum());
}

void ObjectEditorWidget::refreshObjectEditStackedWidget() {
    ui->objectEditStackedWidget->refreshActiveWidget();
}

void ObjectEditorWidget::on_objectPatternListWidget_itemClicked(QListWidgetItem *item)
{
    int index = item->data(Qt::UserRole).toInt();
    ui->objectEditStackedWidget->editToken(
                objectView_.patternToken(index));

    refreshObjectPatternList();
    refreshDisplay();
//    ui->objectPatternListWidget->scrollTo(
//                ui->objectPatternListWidget->currentIndex());

//    ui->objectPatternListWidget->setCurrentRow(index, QItemSelectionModel::Current);
}

void ObjectEditorWidget::on_objectSetBox_activated(int index)
{
    objectView_.changeObjectSet(index);
    reload();
}

void ObjectEditorWidget::on_objectRemoveButton_clicked()
{
    if (ui->objectPatternListWidget->currentRow() < 0) {
        return;
    }

    int result = objectView_.removeObjectPattern(
                ui->objectPatternListWidget->currentRow());

    // Load the item at the new index before reloading the pattern list;
    // otherwise, a deleted pointer will be dereferenced when the stacked
    // widget, which still holds the removed item, gets refreshed
    on_objectPatternListWidget_itemClicked(
                ui->objectPatternListWidget->item(result));

    reload();

    ui->objectPatternListWidget->setCurrentRow(
                result);
}

void ObjectEditorWidget::on_objectAddButton_clicked()
{
    int targetRow = ui->objectPatternListWidget->currentRow();

    // Insert at beginning if nothing selected
    if ((targetRow < 0)) {
        targetRow = 0;
    }
    // Insert at end if last item in list is selected
    else if ((targetRow == ui->objectPatternListWidget->count() - 1)) {
        targetRow = ui->objectPatternListWidget->count();
    }

    AddObjectPatternDialog dialog(objectView_, this);
    int result = dialog.exec();

    if ((result == QDialog::Rejected)
            || (dialog.token() == NULL)) {
        return;
    }

    int newIndex = objectView_.addObjectPattern(
                targetRow,
                *(dialog.token()));

//    on_objectPatternListWidget_itemClicked(
//                ui->objectPatternListWidget->item(newIndex));

    reload();

    ui->objectPatternListWidget->setCurrentRow(
                newIndex);

    on_objectPatternListWidget_itemClicked(
                ui->objectPatternListWidget->item(newIndex));
}

void ObjectEditorWidget::on_objectUpButton_clicked()
{
    int result
            = objectView_.moveObjectPatternUp(
                ui->objectPatternListWidget->currentRow());

    reload();

    ui->objectPatternListWidget->setCurrentRow(
                result);
}

void ObjectEditorWidget::on_objectDownButton_clicked()
{
    int result
            = objectView_.moveObjectPatternDown(
                ui->objectPatternListWidget->currentRow());

    reload();

    if (result >= ui->objectPatternListWidget->count()) {
        ui->objectPatternListWidget->setCurrentRow(
                    ui->objectPatternListWidget->count() - 1);
    }
    else {
        ui->objectPatternListWidget->setCurrentRow(
                    result);
    }
}
