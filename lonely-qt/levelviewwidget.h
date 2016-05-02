#ifndef LEVELVIEWWIDGET_H
#define LEVELVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QScrollBar>
#include <QWheelEvent>
#include "inputqlabel.h"

namespace Ui {
class LevelViewWidget;
}

class LevelViewWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LevelViewWidget(QWidget *parent = 0);
    ~LevelViewWidget();

    InputQLabel& levelViewLabel();
    InputQLabel& controlViewLabel();
    QScrollBar& horizontalScrollBar();
    QScrollBar& verticalScrollBar();

public slots:
    void viewWheelEvent(QWheelEvent* event);
    
private:
    Ui::LevelViewWidget *ui;

protected:
//    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // LEVELVIEWWIDGET_H
