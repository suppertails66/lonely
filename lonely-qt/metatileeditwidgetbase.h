#ifndef METATILEEDITWIDGETBASE_H
#define METATILEEDITWIDGETBASE_H

#include <QWidget>
#include "gamedata/LaylaMetatile.h"

class MetatileEditWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit MetatileEditWidgetBase(QWidget *parent = 0);
    
    void editMetatile(Lonely::LaylaMetatile& metatile__);

    virtual void refreshMetatileDisplay() =0;
protected:
    Lonely::LaylaMetatile* metatile_;

signals:
    
public slots:
    
};

#endif // METATILEEDITWIDGETBASE_H
