#include "spawnbossedwidget.h"
#include "ui_spawnbossedwidget.h"
#include "lonelyglobals.h"
#include "gamedata/LaylaBoss.h"
#include <cstdlib>

using namespace Lonely;

SpawnBossEdWidget::SpawnBossEdWidget(QWidget *parent) :
    QWidget(parent),
    token_(NULL),
    ui(new Ui::SpawnBossEdWidget)
{
    ui->setupUi(this);

    refreshChoices();
}

SpawnBossEdWidget::~SpawnBossEdWidget()
{
    delete ui;
}

void SpawnBossEdWidget::setToken(Lonely::SpawnBossToken& token__) {
    token_ = &token__;

    refreshChoices();

    for (int i = 0; i < ui->bossSelectBox->count(); i++) {
        if (static_cast<LaylaBosses::LaylaBoss>(
                    ui->bossSelectBox->itemData(i, Qt::UserRole).toInt())
                == token_->bossNum()) {
            ui->bossSelectBox->setCurrentIndex(i);
            break;
        }
    }
}

void SpawnBossEdWidget::refreshChoices() {
    ui->bossSelectBox->clear();

    // CHEAT CHEAT CHEAT CHEAT CHEAT
    int objectSetNum = globals_.editor().levelEditor().currentObjectSetNum();

    for (int i = 0; i < LaylaBosses::numBosses; i++) {
        LaylaBosses::LaylaBoss boss
                = static_cast<LaylaBosses::LaylaBoss>(i);

        if (LaylaBosses::bossIsInObjectSet(boss, objectSetNum)) {
            ui->bossSelectBox->addItem(
                        LaylaBosses::nameOfBoss(boss).c_str(),
                        i);
        }
    }
}

void SpawnBossEdWidget::on_bossSelectBox_activated(int index)
{
    LaylaBosses::LaylaBoss boss =
            static_cast<LaylaBosses::LaylaBoss>(
                ui->bossSelectBox->itemData(index, Qt::UserRole).toInt());

    if (token_ != NULL) {
        token_->setBossNum(boss);
    }
}
