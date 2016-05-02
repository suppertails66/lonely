#ifndef LONELYWIDGETS_H
#define LONELYWIDGETS_H

#include "editor/LonelyEditor.h"
#include <QWidget>
#include "editorwidget.h"
#include "util/IniFile.h"
#include <string>

class LonelyWidgets
{
public:

    const static int numWidgets = 1;
    enum LonelyWidget {
        levelEditor = 0,
        paletteEditor
    };

    static EditorWidget* instantiateWidget(LonelyWidget widgetID,
                                      Lonely::IniFile& configFile,
                                      Lonely::LonelyEditor& editor);

    static std::string widgetToolTip(LonelyWidget widgetID);

    static std::string widgetShortcut(LonelyWidget widgetID);
};

#endif // LONELYWIDGETS_H
