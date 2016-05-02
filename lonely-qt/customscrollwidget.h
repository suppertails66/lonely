#ifndef CUSTOMSCROLLWIDGET_H
#define CUSTOMSCROLLWIDGET_H

#include <QWidget>

namespace Ui {
class CustomScrollWidget;
}

class CustomScrollWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CustomScrollWidget(QWidget *parent = 0);
    ~CustomScrollWidget();
    
private:
    Ui::CustomScrollWidget *ui;
};

#endif // CUSTOMSCROLLWIDGET_H
