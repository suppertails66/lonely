#ifndef METATILEDAMAGINGEDITWIDGET_H
#define METATILEDAMAGINGEDITWIDGET_H

#include <QWidget>
#include "metatileeditwidgetbase.h"

namespace Ui {
class MetatileDamagingEditWidget;
}

class MetatileDamagingEditWidget : public MetatileEditWidgetBase
{
    Q_OBJECT
    
public:
    explicit MetatileDamagingEditWidget(QWidget *parent = 0);
    ~MetatileDamagingEditWidget();

    virtual void refreshMetatileDisplay();
    
private slots:
    void on_damagingBox_clicked(bool checked);

private:
    Ui::MetatileDamagingEditWidget *ui;
};

#endif // METATILEDAMAGINGEDITWIDGET_H
