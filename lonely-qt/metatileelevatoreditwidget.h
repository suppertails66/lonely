#ifndef METATILEELEVATOREDITWIDGET_H
#define METATILEELEVATOREDITWIDGET_H

#include <QWidget>
#include "metatileeditwidgetbase.h"

namespace Ui {
class MetatileElevatorEditWidget;
}

class MetatileElevatorEditWidget : public MetatileEditWidgetBase
{
    Q_OBJECT
    
public:
    explicit MetatileElevatorEditWidget(QWidget *parent = 0);
    ~MetatileElevatorEditWidget();

    virtual void refreshMetatileDisplay();
private slots:
    void on_localButton_clicked();

    void on_warpButton_clicked();

private:
    Ui::MetatileElevatorEditWidget *ui;
};

#endif // METATILEELEVATOREDITWIDGET_H
