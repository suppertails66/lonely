#ifndef HPEDITORWIDGET_H
#define HPEDITORWIDGET_H

#include <QWidget>
#include "structs/Tbyte.h"

namespace Ui {
class HpEditorWidget;
}

class HpEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HpEditorWidget(QWidget *parent = 0);
    ~HpEditorWidget();

    void editHp(Lonely::Tbyte& hp__);

    void clearHp();

    bool hpLoaded() const;

protected:
    Lonely::Tbyte* hp_;
    
private slots:
    void on_hpBox_valueChanged(int arg1);

private:
    Ui::HpEditorWidget *ui;
};

#endif // HPEDITORWIDGET_H
