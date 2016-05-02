#ifndef SETSCROLLINGEDWIDGET_H
#define SETSCROLLINGEDWIDGET_H

#include <QWidget>
#include "gamedata/LaylaPatternToken.h"

namespace Ui {
class SetScrollingEdWidget;
}

class SetScrollingEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetScrollingEdWidget(QWidget *parent = 0);
    ~SetScrollingEdWidget();

    void setToken(Lonely::SetScrollingToken& token__);
protected:
    Lonely::SetScrollingToken* token_;

private slots:
    void on_killsToNextFoodBox_valueChanged(int arg1);

    void on_scrollingEnabledButton_clicked();

    void on_scrollingDisabledButton_clicked();

private:
    Ui::SetScrollingEdWidget *ui;
};

#endif // SETSCROLLINGEDWIDGET_H
