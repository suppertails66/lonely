#ifndef OBJECTEDITSTACKEDWIDGET_H
#define OBJECTEDITSTACKEDWIDGET_H

#include <QStackedWidget>
#include "gamedata/LaylaObjectPatternToken.h"
#include "editor/LonelyObjectView.h"

class ObjectEditStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ObjectEditStackedWidget(QWidget *parent = 0);
    
    void editToken(Lonely::LaylaObjectPatternToken& token);

    void refreshActiveWidget();
signals:
    
public slots:

protected:
    Lonely::LonelyObjectView& objectView_;
    
};

#endif // OBJECTEDITSTACKEDWIDGET_H
