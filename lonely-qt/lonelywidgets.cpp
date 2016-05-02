#include "lonelywidgets.h"
#include "leveleditorwidget.h"
#include <cstdlib>

using namespace Lonely;

EditorWidget* LonelyWidgets::instantiateWidget(LonelyWidget widgetID,
                                          Lonely::IniFile& configFile,
                                          LonelyEditor& editor) {
    switch (widgetID) {
    case LonelyWidgets::levelEditor:
        return new LevelEditorWidget(configFile,
                                     editor.levelEditor());
        break;
    case LonelyWidgets::paletteEditor:
        return NULL;
        break;
    default:
        return NULL;
        break;
    }
}

std::string LonelyWidgets::widgetToolTip(LonelyWidget widgetID) {
    switch (widgetID) {
    case LonelyWidgets::levelEditor:
        return "Level editor";
        break;
    case LonelyWidgets::paletteEditor:
        return "Palette editor";
        break;
    default:
        return "NULL";
        break;
    }
}

std::string LonelyWidgets::widgetShortcut(LonelyWidget widgetID) {
    switch (widgetID) {
    case LonelyWidgets::levelEditor:
        return "Shift+L";
        break;
    case LonelyWidgets::paletteEditor:
        return "Shift+P";
        break;
    default:
        return "";
        break;
    }
}
