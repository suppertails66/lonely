#include "lonelymainwindow.h"
#include "ui_lonelymainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QFileDialog>
#include "romselectdialog.h"
#include "aboutdialog.h"
#include "util/StringConversion.h"
#include "lonelyglobals.h"
#include <iostream>

using namespace Lonely;

LonelyMainWindow::LonelyMainWindow(Lonely::IniFile& configFile__,
                                   QWidget *parent) :
    QMainWindow(parent),
    configFile_(configFile__),
    editor_(configFile_.valueOfKey("Global", "SourceRomPath"),
            configFile_.valueOfKey("Global", "OffsetFilePath"),
            StringConversion::fromString<bool>(
                configFile_.valueOfKey("Global", "SaveLastExportLocation"))),
    ui(new Ui::LonelyMainWindow)
{
    globals_.setEditor(editor_);

    ui->setupUi(this);

    initTools();
}

LonelyMainWindow::~LonelyMainWindow()
{
    delete ui;
}

void LonelyMainWindow::initTools() {
//    delete ui->toolSelectWidget->layout();
//    QVBoxLayout* layout = new QVBoxLayout(ui->toolSelectWidget);
//    ui->toolSelectWidget->setLayout(
//                layout);

    for (int i = 0; i < LonelyWidgets::numWidgets; i++) {
        LonelyWidgets::LonelyWidget widgetID
                = static_cast<LonelyWidgets::LonelyWidget>(i);

        // Add to stacked widget

        EditorWidget* widget = LonelyWidgets::instantiateWidget(widgetID,
                                                           configFile_,
                                                           editor_);
        widget->setParent(ui->toolStackedWidget);
        ui->toolStackedWidget->addWidget(widget);

        // Add to sidebar

/*        NumberedQPushButton* button
                = new NumberedQPushButton(i, ui->toolSelectWidget);
        button->setCheckable(true);
        button->setMaximumWidth(24);
        button->setMaximumHeight(24);
        button->setText(StringConversion::toString(i).c_str());
        button->setToolTip(LonelyWidgets::widgetToolTip(widgetID).c_str());
        QObject::connect(button,
                         SIGNAL(clicked(int, bool)),
                         this,
                         SLOT(toolButtonClicked(int, bool)));
        toolButtons_[i] = button;

        ui->toolSelectWidget->layout()->addWidget(
                    button);

        // Add to toolbar

        NumberedQAction* action = new NumberedQAction(
                                      i,
                                      QIcon(""),
                                      tr(LonelyWidgets::widgetToolTip(widgetID).c_str()),
                                      this);
        action->setCheckable(true);
        action->setShortcut(
                    QKeySequence(tr(LonelyWidgets::widgetShortcut(widgetID).c_str())));
        QObject::connect(action,
                         SIGNAL(triggered(int)),
                         this,
                         SLOT(toolActionTriggered(int)));
        toolActions_[i] = action;

        ui->menuTools->addAction(action); */

        widget->refreshDisplay();
    }

    // Push buttons to top of sidebar
//    layout->addStretch(0);

    // Select first tool
    changeTool(static_cast<LonelyWidgets::LonelyWidget>(0));
}

void LonelyMainWindow::changeTool(LonelyWidgets::LonelyWidget widgetID) {
    // Raise all buttons
/*    for (int i = 0; i < LonelyWidgets::numWidgets; i++) {
        toolButtons_[i]->setChecked(false);
        toolActions_[i]->setChecked(false);
    }

    // Lower the clicked button
    toolButtons_[widgetID]->setChecked(true);
    toolActions_[widgetID]->setChecked(true); */

    // Select the chosen widget in the stacked widget
    ui->toolStackedWidget->setCurrentIndex(widgetID);

    // Refresh the newly displayed widget
    dynamic_cast<EditorWidget*>(
                ui->toolStackedWidget->currentWidget())->refreshDisplay();
}

void LonelyMainWindow::toolButtonClicked(int num, bool checked) {
    changeTool(static_cast<LonelyWidgets::LonelyWidget>(num));
}

void LonelyMainWindow::toolActionTriggered(int num) {
    changeTool(static_cast<LonelyWidgets::LonelyWidget>(num));
}

void LonelyMainWindow::on_actionExport_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Export ROM"),
                                                tr(""),
                                                tr("NES ROMs (*.nes)"));

    if (path == "") {
        return;
    }

    editor_.exportToFile(path.toStdString());
}

void LonelyMainWindow::on_actionSave_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save project"),
                                                tr(""),
                                                tr("Project files (*.lon)"));

    if (path == "") {
        return;
    }

    editor_.saveToFile(path.toStdString());
}

void LonelyMainWindow::on_actionOpen_project_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open project"),
                                                tr(""),
                                                tr("Project files (*.lon)"));

    if (path == "") {
        return;
    }

    editor_.loadFromFile(path.toStdString());

    dynamic_cast<EditorWidget*>(
                ui->toolStackedWidget->currentWidget())->reload();
}

void LonelyMainWindow::on_actionOptions_triggered()
{
    RomSelectDialog dialog(configFile_, this);
    dialog.exec();
}

void LonelyMainWindow::on_actionSave_project_triggered()
{
    if (editor_.lastSavedFile().size() > 0) {
        editor_.saveToFile(editor_.lastSavedFile());
    }
    else {
        on_actionSave_as_triggered();
    }
}

void LonelyMainWindow::on_actionExport_ROM_triggered()
{
    if (editor_.rememberExportLocation()
            && (editor_.lastExportLocation().size() > 0)) {
        editor_.exportToFile(editor_.lastExportLocation());
    }
    else {
        on_actionExport_as_triggered();
    }
}

void LonelyMainWindow::on_actionAbout_triggered()
{
    AboutDialog(this).exec();
}

void LonelyMainWindow::on_actionExit_triggered()
{
    qApp->closeAllWindows();
}

void LonelyMainWindow::on_actionSave_map_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save level map"),
                                                tr(""),
                                                tr("PNG files (*.png)"));

    if (path == "") {
        return;
    }

    editor_.levelEditor().levelView().exportLevelMap(path.toStdString());
}
