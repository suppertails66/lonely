#ifndef DELAYEDITWIDGET_H
#define DELAYEDITWIDGET_H

#include <QWidget>
#include "objecteditorsubwidget.h"
#include "gamedata/LaylaObjectPatternToken.h"

namespace Ui {
class DelayEditWidget;
}

class DelayEditWidget : public ObjectEditorSubWidget
{
    Q_OBJECT
    
public:
    explicit DelayEditWidget(QWidget *parent = 0);
    ~DelayEditWidget();
    
    void refreshDisplay();

    void setDelayToken(Lonely::LaylaObjectDelayToken& delayToken__);
protected:
    Lonely::LaylaObjectDelayToken* delayToken_;

private slots:
    void on_delayBox_valueChanged(int arg1);

private:
    Ui::DelayEditWidget *ui;
};

#endif // DELAYEDITWIDGET_H
