#ifndef SETAREAEDWIDGET_H
#define SETAREAEDWIDGET_H

#include <QWidget>
#include "gamedata/LaylaPatternToken.h"

namespace Ui {
class SetAreaEdWidget;
}

class SetAreaEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetAreaEdWidget(QWidget *parent = 0);
    ~SetAreaEdWidget();

    void setToken(Lonely::SetAreaToken& token__);
protected:
    const static int caveIndex_ = 0;
    const static int baseIndex_ = 1;
    const static int bossIndex_ = 2;

    Lonely::SetAreaToken* token_;
    
private slots:
    void on_backgroundMetatileBox_valueChanged(int arg1);

    void on_areaTypeBox_activated(int index);

private:
    Ui::SetAreaEdWidget *ui;
};

#endif // SETAREAEDWIDGET_H
