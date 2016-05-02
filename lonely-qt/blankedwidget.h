#ifndef BLANKEDWIDGET_H
#define BLANKEDWIDGET_H

#include <QWidget>

namespace Ui {
class BlankEdWidget;
}

class BlankEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BlankEdWidget(QWidget *parent = 0);
    ~BlankEdWidget();
    
private:
    Ui::BlankEdWidget *ui;
};

#endif // BLANKEDWIDGET_H
