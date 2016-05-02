#ifndef JUMPTOPOSITIONEDWIDGET_H
#define JUMPTOPOSITIONEDWIDGET_H

#include <QWidget>
#include "gamedata/LaylaPatternToken.h"

namespace Ui {
class JumpToPositionEdWidget;
}

class JumpToPositionEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit JumpToPositionEdWidget(QWidget *parent = 0);
    ~JumpToPositionEdWidget();

    void setToken(Lonely::JumpToPositionToken& token__);
protected:
    Lonely::JumpToPositionToken* token_;
    
private slots:
    void on_offsetBox_valueChanged(int arg1);

private:
    Ui::JumpToPositionEdWidget *ui;
};

#endif // JUMPTOPOSITIONEDWIDGET_H
