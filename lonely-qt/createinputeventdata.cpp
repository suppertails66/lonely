#include "createinputeventdata.h"

using namespace Lonely;

CreateInputEventData::CreateInputEventData()
{
}

Lonely::InputEventData CreateInputEventData::create(
        QMouseEvent* event) {
    InputEventData data;

    data.setX(event->x());
    data.setY(event->y());
    data.setMouseLeftButton(event->buttons() & Qt::LeftButton);
    data.setMouseMiddleButton(event->buttons() & Qt::MiddleButton);
    data.setMouseRightButton(event->buttons() & Qt::RightButton);
    data.setControl(event->modifiers() & Qt::ControlModifier);
    data.setAlt(event->modifiers() & Qt::AltModifier);
    data.setShift(event->modifiers() & Qt::ShiftModifier);

    return data;
}
