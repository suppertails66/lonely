#ifndef ADDREMOVELISTWIDGET_H
#define ADDREMOVELISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
class AddRemoveListWidget;
}

class AddRemoveListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AddRemoveListWidget(QWidget *parent = 0);
    ~AddRemoveListWidget();

    QListWidget& list();

    QPushButton& addButton();
    QPushButton& removeButton();
    
private:
    Ui::AddRemoveListWidget *ui;

signals:
    void addButtonClicked();
    void removeButtonClicked();
private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
};

#endif // ADDREMOVELISTWIDGET_H
