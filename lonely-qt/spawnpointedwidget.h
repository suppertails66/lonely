#ifndef SPAWNPOINTEDWIDGET_H
#define SPAWNPOINTEDWIDGET_H

#include <QWidget>

namespace Ui {
class SpawnPointEdWidget;
}

class SpawnPointEdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SpawnPointEdWidget(QWidget *parent = 0);
    ~SpawnPointEdWidget();
    
private:
    Ui::SpawnPointEdWidget *ui;
};

#endif // SPAWNPOINTEDWIDGET_H
