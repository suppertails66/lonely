#include "addnodeentrydialog.h"
#include "ui_addnodeentrydialog.h"
#include "gamedata/LaylaPatternToken.h"
#include <iostream>

using namespace Lonely;

AddNodeEntryDialog::AddNodeEntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNodeEntryDialog)
{
    ui->setupUi(this);

    for (int i = 0;
         i < LaylaPatternTypes::numLaylaPatternTypes;
         i++) {
        LaylaPatternTypes::LaylaPatternType patternType
                = static_cast<LaylaPatternTypes::LaylaPatternType>(i);

        switch (patternType) {
        case LaylaPatternTypes::spawnBoss:
            ui->nodeTypeList->addItem(
                        new QListWidgetItem());
            ui->nodeTypeList->item(
                        ui->nodeTypeList->count() - 1)->setText("Spawn boss");
            break;
        case LaylaPatternTypes::setScrolling:
            ui->nodeTypeList->addItem(
                        new QListWidgetItem());
            ui->nodeTypeList->item(
                        ui->nodeTypeList->count() - 1)->setText("Set scrolling");
            break;
        case LaylaPatternTypes::setArea:
            ui->nodeTypeList->addItem(
                        new QListWidgetItem());
            ui->nodeTypeList->item(
                        ui->nodeTypeList->count() - 1)->setText("Set area");
            break;
        case LaylaPatternTypes::setMap:
            ui->nodeTypeList->addItem(
                        new QListWidgetItem());
            ui->nodeTypeList->item(
                        ui->nodeTypeList->count() - 1)->setText("Set map");
            break;
        case LaylaPatternTypes::setElevators:
            ui->nodeTypeList->addItem(
                        new QListWidgetItem());
            ui->nodeTypeList->item(
                        ui->nodeTypeList->count() - 1)->setText("Set elevators");
            break;
        case LaylaPatternTypes::jumpToPosition:
            ui->nodeTypeList->addItem(
                        new QListWidgetItem());
            ui->nodeTypeList->item(
                        ui->nodeTypeList->count() - 1)->setText("Jump to position");
            break;

        case LaylaPatternTypes::spawnPoint:
        case LaylaPatternTypes::standardPattern:
        default:
            break;
        }

        // Add item data if item was added
        switch (patternType) {
        case LaylaPatternTypes::spawnBoss:
        case LaylaPatternTypes::setScrolling:
        case LaylaPatternTypes::setArea:
        case LaylaPatternTypes::setMap:
        case LaylaPatternTypes::setElevators:
        case LaylaPatternTypes::jumpToPosition:
            ui->nodeTypeList->item(
                        ui->nodeTypeList->count() - 1)->setData(
                        Qt::UserRole, patternType);
            break;

        case LaylaPatternTypes::spawnPoint:
        case LaylaPatternTypes::standardPattern:
        default:
            break;
        }
    }
}

AddNodeEntryDialog::~AddNodeEntryDialog()
{
    delete ui;
}
