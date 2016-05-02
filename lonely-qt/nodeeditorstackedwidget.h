#ifndef NODEEDITORSTACKEDWIDGET_H
#define NODEEDITORSTACKEDWIDGET_H

#include <QStackedWidget>
#include "gamedata/LaylaPatternToken.h"

class NodeEditorStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit NodeEditorStackedWidget(QWidget *parent = 0);
    
    void setToken(Lonely::LaylaPatternToken* token__);

    void deleteToken();
signals:
    
public slots:

protected:
    Lonely::LaylaPatternToken* token_;
    
};

#endif // NODEEDITORSTACKEDWIDGET_H
