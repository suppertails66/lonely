#ifndef LONELYGLOBALDATA_H
#define LONELYGLOBALDATA_H

#include "editor/LonelyEditor.h"

class LonelyGlobalData
{
public:
    LonelyGlobalData();

    void setEditor(Lonely::LonelyEditor& editor__);
    Lonely::LonelyEditor& editor();
protected:
    Lonely::LonelyEditor* editor_;
};

#endif // LONELYGLOBALDATA_H
