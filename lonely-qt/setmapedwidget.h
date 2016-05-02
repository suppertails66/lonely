#ifndef SETMAPEDWIDGET_H
#define SETMAPEDWIDGET_H

#include <QWidget>
#include "gamedata/LaylaPatternToken.h"

namespace Ui {
class SetMapEdWidget;
}

class SetMapEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetMapEdWidget(QWidget *parent = 0);
    ~SetMapEdWidget();

    void setToken(Lonely::SetMapToken& token__);
protected:
    Lonely::SetMapToken* token_;
    
private slots:
    void on_mapNumBox_valueChanged(int arg1);

    void on_param2Box_valueChanged(int arg1);

private:
    Ui::SetMapEdWidget *ui;
};

#endif // SETMAPEDWIDGET_H
