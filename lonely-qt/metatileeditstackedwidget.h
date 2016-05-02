#ifndef METATILEEDITSTACKEDWIDGET_H
#define METATILEEDITSTACKEDWIDGET_H

#include <QStackedWidget>
#include "metatileelevatoreditwidget.h"
#include "metatiledamagingeditwidget.h"
#include "gamedata/LaylaMetatile.h"

class MetatileEditStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MetatileEditStackedWidget(QWidget *parent = 0);
//    virtual ~MetatileEditStackedWidget();
    
    void editMetatile(Lonely::LaylaMetatile& metatile__);

protected:
    MetatileElevatorEditWidget* elevatorWidget_;
    MetatileDamagingEditWidget* damagingWidget_;

signals:
    
public slots:
    
};

#endif // METATILEEDITSTACKEDWIDGET_H
