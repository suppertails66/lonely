#include "addobjectpatterndialog.h"
#include "ui_addobjectpatterndialog.h"
#include "gamedata/LaylaObjectPatternToken.h"
#include "util/StringConversion.h"
#include <cstdlib>
#include <iostream>

using namespace Lonely;

AddObjectPatternDialog::AddObjectPatternDialog(
        Lonely::LonelyObjectView& objectView__,
        QWidget *parent) :
    QDialog(parent),
    objectView_(&objectView__),
    token_(NULL),
    ui(new Ui::AddObjectPatternDialog)
{
    ui->setupUi(this);

    changeToken(LaylaObjectPatternTypes::standardSpawn);

    for (int i = 0; i < LaylaObjectPatternTypes::numObjectPatternTypes; i++) {
        LaylaObjectPatternTypes::LaylaObjectPatternType patternType
                = static_cast<LaylaObjectPatternTypes::LaylaObjectPatternType>(i);

        switch (patternType) {
        case LaylaObjectPatternTypes::delay:
            ui->objectPatternTypeListWidget->addItem(
                        "Delay");
            break;
        case LaylaObjectPatternTypes::standardSpawn:
            ui->objectPatternTypeListWidget->addItem(
                        "Standard spawn");
            break;
        default:
            continue;
            break;
        }

        ui->objectPatternTypeListWidget->item(
                    ui->objectPatternTypeListWidget->count() - 1)->setData(
                    Qt::UserRole, patternType);
    }
}

AddObjectPatternDialog::~AddObjectPatternDialog()
{
    delete ui;

    if (result() == QDialog::Rejected) {
        delete token_;
        token_ == NULL;
    }
}

Lonely::LaylaObjectPatternToken* AddObjectPatternDialog::token() {
    return token_;
}

void AddObjectPatternDialog::on_objectPatternTypeListWidget_itemClicked(QListWidgetItem *item)
{
    LaylaObjectPatternTypes::LaylaObjectPatternType patternType
            = static_cast<LaylaObjectPatternTypes::LaylaObjectPatternType>(
                item->data(Qt::UserRole).toInt());

    changeToken(patternType);
}

void AddObjectPatternDialog::changeToken(
        Lonely::LaylaObjectPatternTypes::LaylaObjectPatternType patternType) {
    delete token_;
    token_ = NULL;

    switch (patternType) {
    case LaylaObjectPatternTypes::delay:
        token_ = new LaylaObjectDelayToken(9);
        break;
    case LaylaObjectPatternTypes::standardSpawn:
        token_ = new LaylaObjectStandardSpawnToken(10, false);
        break;
    default:
        break;
    }

    ui->objectEditStackedWidget->editToken(*token_);
}
