#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>

class EditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EditorWidget(QWidget *parent = 0);

    virtual ~EditorWidget();
    
    virtual void refreshDisplay() =0;

    virtual void reload() =0;
signals:
    
public slots:
    
};

#endif // EDITORWIDGET_H
