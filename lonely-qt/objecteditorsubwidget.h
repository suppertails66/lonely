#ifndef OBJECTEDITORSUBWIDGET_H
#define OBJECTEDITORSUBWIDGET_H

#include <QWidget>

class ObjectEditorSubWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectEditorSubWidget(QWidget *parent = 0);
    virtual ~ObjectEditorSubWidget();
    
    virtual void refreshDisplay();
signals:
    
public slots:
    
};

#endif // OBJECTEDITORSUBWIDGET_H
