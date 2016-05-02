#include "inputqlabel.h"
#include "createinputeventdata.h"
#include <cstdlib>

using namespace Lonely;

InputQLabel::InputQLabel(QWidget *parent) :
    QLabel(parent),
    enterCallback_(NULL),
    leaveCallback_(NULL),
    mouseMoveCallback_(NULL),
    mousePressCallback_(NULL),
    mouseReleaseCallback_(NULL),
    mouseDoubleClickCallback_(NULL)
{
    setMouseTracking(true);
}

void InputQLabel::setCallbacks(void* obj__,
                  void (*enterCallback__)(void*),
                  void (*leaveCallback__)(void*),
                  void (*mouseMoveCallback__)(void*, Lonely::InputEventData),
                  void (*mousePressCallback__)(void*, Lonely::InputEventData),
                  void (*mouseReleaseCallback__)(void*, Lonely::InputEventData),
                  void (*mouseDoubleClickCallback__)(void*, Lonely::InputEventData)) {
    obj_ = obj__;
    enterCallback_ = enterCallback__;
    leaveCallback_ = leaveCallback__;
    mouseMoveCallback_ = mouseMoveCallback__;
    mousePressCallback_ = mousePressCallback__;
    mouseReleaseCallback_ = mouseReleaseCallback__;
    mouseDoubleClickCallback_ = mouseDoubleClickCallback__;
}

void InputQLabel::enterEvent(QEvent* event) {
    if (enterCallback_ != NULL) {
        enterCallback_(obj_);
    }

    emit entered(event);
    emit entered();
}

void InputQLabel::leaveEvent(QEvent* event) {
    if (leaveCallback_ != NULL) {
        leaveCallback_(obj_);
    }

    emit left(event);
    emit left();
}

void InputQLabel::mouseMoveEvent(QMouseEvent* event) {
    InputEventData data = CreateInputEventData::create(event);
    if (mouseMoveCallback_ != NULL) {
        mouseMoveCallback_(obj_, data);
    }

    emit mouseMoved(event);
    emit mouseMoved(data);
}

void InputQLabel::mousePressEvent(QMouseEvent* event) {
    InputEventData data = CreateInputEventData::create(event);
    if (mousePressCallback_ != NULL) {
        mousePressCallback_(obj_, data);
    }

    emit mousePressed(event);
    emit mousePressed(data);

}

void InputQLabel::mouseReleaseEvent(QMouseEvent* event) {
    InputEventData data = CreateInputEventData::create(event);
    if (mouseReleaseCallback_ != NULL) {
        mouseReleaseCallback_(obj_, data);
    }

    emit mouseReleased(event);
    emit mouseReleased(data);

}

void InputQLabel::mouseDoubleClickEvent(QMouseEvent* event) {
    InputEventData data = CreateInputEventData::create(event);
    if (mouseDoubleClickCallback_ != NULL) {
        mouseDoubleClickCallback_(obj_, data);
    }

    emit mouseDoubleClicked(event);
    emit mouseDoubleClicked(data);
}

/*void InputQLabel::wheelEvent(QWheelEvent* event) {
    emit wheelEventTriggered(event);
//    InputQLabel::wheelEvent(event);
//    event->accept();
} */
