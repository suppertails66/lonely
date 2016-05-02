#include "lonelyglobaldata.h"

LonelyGlobalData::LonelyGlobalData()
{
}

void LonelyGlobalData::setEditor(Lonely::LonelyEditor& editor__) {
    editor_ = &editor__;
}

Lonely::LonelyEditor& LonelyGlobalData::editor() {
    return *editor_;
}
