#ifndef CREATEINPUTEVENTDATA_H
#define CREATEINPUTEVENTDATA_H

#include "structs/InputEventData.h"
#include <QMouseEvent>

class CreateInputEventData
{
public:
    CreateInputEventData();

    static Lonely::InputEventData create(
            QMouseEvent* event);
};

#endif // CREATEINPUTEVENTDATA_H
