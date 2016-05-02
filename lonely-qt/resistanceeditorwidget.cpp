#include "resistanceeditorwidget.h"
#include "ui_resistanceeditorwidget.h"
#include "qtformatconversion.h"
#include "lonelyglobals.h"
#include "structs/Graphic.h"

using namespace Lonely;

ResistanceEditorWidget::ResistanceEditorWidget(QWidget *parent) :
    EditorWidget(parent),
    resistanceView_(globals_.editor().levelEditor().resistanceView()),
    ui(new Ui::ResistanceEditorWidget)
{
    ui->setupUi(this);

    ui->objectListWidget->setIconSize(QSize(32, 32));
    ui->bossListWidget->setIconSize(QSize(32, 32));

    reload();
}

ResistanceEditorWidget::~ResistanceEditorWidget()
{
    delete ui;
}

void ResistanceEditorWidget::refreshDisplay() {
    refreshVulnerabilityEditor();
}

void ResistanceEditorWidget::reload() {
//    std::cout << "here" << std::endl;
    repopulateLists();
    refreshDisplay();
}

void ResistanceEditorWidget::repopulateLists() {
    ui->objectListWidget->clear();
    ui->bossListWidget->clear();

    for (int i = 0; i < resistanceView_.numObjectResistances(); i++) {
        ui->objectListWidget->addItem(
                    resistanceView_.nameOfObject(i).c_str());
        ui->objectListWidget->item(ui->objectListWidget->count() - 1)
                ->setData(Qt::UserRole, i);
        Graphic icon;
        QImage iconImage;
        resistanceView_.drawObjectIcon(icon, i);
        QtFormatConversion::drawGraphicToImage(iconImage, icon);
        ui->objectListWidget->item(ui->objectListWidget->count() - 1)
                ->setIcon(
                    QPixmap().fromImage(iconImage));
    }

    for (int i = 0; i < resistanceView_.numBossResistances(); i++) {
        ui->bossListWidget->addItem(
                    resistanceView_.nameOfBoss(i).c_str());
        ui->bossListWidget->item(ui->bossListWidget->count() - 1)
                ->setData(Qt::UserRole, i);
        Graphic icon;
        QImage iconImage;
        resistanceView_.drawBossIcon(icon, i);
        QtFormatConversion::drawGraphicToImage(iconImage, icon);
        ui->bossListWidget->item(ui->bossListWidget->count() - 1)
                ->setIcon(
                    QPixmap().fromImage(iconImage));
    }
}

void ResistanceEditorWidget::refreshVulnerabilityEditor() {
    ui->vulnerabilityEditorWidget->setEnabled(
                ui->vulnerabilityEditorWidget->settingsLoaded());
    ui->hpEditorWidget->setEnabled(
                ui->hpEditorWidget->hpLoaded());
//    ui->vulnerabilityEditorWidget->editSettings(
//               )
}

void ResistanceEditorWidget::on_objectListWidget_itemClicked(QListWidgetItem *item)
{
    ui->bossListWidget->clearSelection();
    ui->vulnerabilityEditorWidget->editSettings(
                resistanceView_.objectResistances(item->data(Qt::UserRole).toInt()));
    ui->hpEditorWidget->editHp(
                resistanceView_.objectHp(item->data(Qt::UserRole).toInt()));
    refreshDisplay();
}

void ResistanceEditorWidget::on_bossListWidget_itemClicked(QListWidgetItem *item)
{
    ui->objectListWidget->clearSelection();
    ui->vulnerabilityEditorWidget->editSettings(
                resistanceView_.bossResistances(item->data(Qt::UserRole).toInt()));
    ui->hpEditorWidget->clearHp();
    refreshDisplay();
}
