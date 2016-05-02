#ifndef INPUTQLABEL_H
#define INPUTQLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include "structs/InputEventData.h"
#include "structs/DisplayAreaManager.h"

class InputQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit InputQLabel(QWidget *parent = 0);

    void setCallbacks(void* obj__,
                      void (*enterCallback__)(void*),
                      void (*leaveCallback__)(void*),
                      void (*mouseMoveCallback__)(void*, Lonely::InputEventData),
                      void (*mousePressCallback__)(void*, Lonely::InputEventData),
                      void (*mouseReleaseCallback__)(void*, Lonely::InputEventData),
                      void (*mouseDoubleClickCallback__)(void*, Lonely::InputEventData));
protected:
    void* obj_;
    void (*enterCallback_)(void*);
    void (*leaveCallback_)(void*);
    void (*mouseMoveCallback_)(void*, Lonely::InputEventData);
    void (*mousePressCallback_)(void*, Lonely::InputEventData);
    void (*mouseReleaseCallback_)(void*, Lonely::InputEventData);
    void (*mouseDoubleClickCallback_)(void*, Lonely::InputEventData);

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

//    void wheelEvent(QWheelEvent* event);

signals:
    void entered(QEvent* event);
    void left(QEvent* event);
    void mouseMoved(QMouseEvent* event);
    void mousePressed(QMouseEvent* event);
    void mouseReleased(QMouseEvent* event);
    void mouseDoubleClicked(QMouseEvent* event);

    void wheelEventTriggered(QWheelEvent* event);

    void entered();
    void left();
    void mouseMoved(Lonely::InputEventData event);
    void mousePressed(Lonely::InputEventData event);
    void mouseReleased(Lonely::InputEventData event);
    void mouseDoubleClicked(Lonely::InputEventData event);

public slots:
    
};

#endif // INPUTQLABEL_H
