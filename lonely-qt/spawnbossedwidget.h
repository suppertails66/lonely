#ifndef SPAWNBOSSEDWIDGET_H
#define SPAWNBOSSEDWIDGET_H

#include <QWidget>
#include "gamedata/LaylaPatternToken.h"

namespace Ui {
class SpawnBossEdWidget;
}

class SpawnBossEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SpawnBossEdWidget(QWidget *parent = 0);
    ~SpawnBossEdWidget();

    void setToken(Lonely::SpawnBossToken& token__);

    void refreshChoices();
protected:
    Lonely::SpawnBossToken* token_;
    
private slots:
    void on_bossSelectBox_activated(int index);

private:
    Ui::SpawnBossEdWidget *ui;
};

#endif // SPAWNBOSSEDWIDGET_H
