#include "standardspawneditwidget.h"
#include "ui_standardspawneditwidget.h"
#include "editor/LonelyObjectView.h"
#include "gamedata/LaylaObjectSets.h"
#include "util/StringConversion.h"
#include "lonelyglobals.h"
#include "qtformatconversion.h"
#include <cstdlib>

using namespace Lonely;

StandardSpawnEditWidget::StandardSpawnEditWidget(QWidget *parent) :
    ObjectEditorSubWidget(parent),
    spawnToken_(NULL),
    ui(new Ui::StandardSpawnEditWidget)
{
    ui->setupUi(this);

    ui->objectListWidget->setIconSize(QSize(32, 32));
}

StandardSpawnEditWidget::~StandardSpawnEditWidget()
{
    delete ui;
}

void StandardSpawnEditWidget::refreshDisplay() {
    if (spawnToken_ != NULL) {
        ui->continueInterpretingBox->setChecked(
                    spawnToken_->continueInterpreting());

        reloadObjectList();
    }
}

void StandardSpawnEditWidget::reloadObjectList() {
    ui->objectListWidget->clear();

    // utterly cheat by grabbing all the data from globals instead
    // of properly passing it in
    LonelyObjectView objectView
            = globals_.editor().levelEditor().objectView();

    for (int i = 0; i < LaylaObjectSets::objectsPerSet; i++) {
        LonelyLaylaObjects::LonelyLaylaObject lonelyID
                = LaylaObjectSets::lonelyID(objectView.currentObjectSetNum(),
                                            i);

        ui->objectListWidget->addItem(
                    (StringConversion::toString(i)
                        + ": "
                        + LonelyLaylaObjects::nameOfObject(lonelyID)).c_str());
        ui->objectListWidget->item(i)->setData(
                    Qt::UserRole, i);

        Graphic g;
        objectView.drawObjectIcon(g,
                                  i);
        ui->objectListWidget->item(i)->setIcon(
                    QIcon(QtFormatConversion::graphicToPixmap(g)));
    }

    if (spawnToken_ != NULL) {
        ui->objectListWidget->setCurrentRow(
                    spawnToken_->objectType());
    }
}

void StandardSpawnEditWidget::setSpawnToken(
        Lonely::LaylaObjectStandardSpawnToken& spawnToken__) {
    spawnToken_ = &spawnToken__;
}

void StandardSpawnEditWidget::on_continueInterpretingBox_clicked(bool checked)
{
    if (spawnToken_ != NULL) {
        spawnToken_->setContinueInterpreting(checked);
    }
}

void StandardSpawnEditWidget::on_objectListWidget_itemClicked(QListWidgetItem *item)
{
    if (spawnToken_ != NULL) {
        spawnToken_->setObjectType(item->data(Qt::UserRole).toInt());
    }
}
