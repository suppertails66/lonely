#ifndef SETELEVATORSEDWIDGET_H
#define SETELEVATORSEDWIDGET_H

#include <QWidget>
#include "gamedata/LaylaPatternToken.h"

namespace Ui {
class SetElevatorsEdWidget;
}

class SetElevatorsEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetElevatorsEdWidget(QWidget *parent = 0);
    ~SetElevatorsEdWidget();

    void setToken(Lonely::SetElevatorsToken& token__);
protected:
    Lonely::SetElevatorsToken* token_;
    
private slots:
    void on_upDestinationBox_valueChanged(int arg1);

    void on_downDestinationBox_valueChanged(int arg1);

private:
    Ui::SetElevatorsEdWidget *ui;
};

#endif // SETELEVATORSEDWIDGET_H
