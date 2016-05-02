#include "objectpatternlistwidget.h"
#include "qtformatconversion.h"
#include <iostream>

using namespace Lonely;

ObjectPatternListWidget::ObjectPatternListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setIconSize(QSize(32, 32));
}

void ObjectPatternListWidget::repopulate(
        const Lonely::LaylaObjectPatternTable& objects__,
        int objectSetNum,
        const NesPatternTable& patternTable,
        const NesPaletteQuad& palettes) {
    repopulateBase(objects__);

    for (int i = 0; i < objects__.numPatternTokens(); i++) {
        Graphic icon;
        objects__.drawEntryIcon(icon,
                                i,
                                objectSetNum,
                                patternTable,
                                palettes);

        item(i)->setIcon(QIcon(
                             QtFormatConversion::graphicToPixmap(icon)));
    }
}

void ObjectPatternListWidget::repopulate(
                const Lonely::LaylaObjectPatternTable& objects__,
                int levelNum,
                int objectSetNum,
                const Lonely::LaylaGraphics& graphics,
                const Lonely::LaylaPalettes& palettes) {
    repopulateBase(objects__);

    refreshItems(objects__,
                 levelNum,
                 objectSetNum,
                 graphics,
                 palettes);
}

void ObjectPatternListWidget
    ::repopulateBase(const Lonely::LaylaObjectPatternTable& objects__) {
    int oldRow = currentRow();

    clear();
    for (int i = 0; i < objects__.numPatternTokens(); i++) {
        addItem(objects__.descriptionString(i).c_str());
        item(i)->setData(Qt::UserRole, i);
    }

    // Try to maintain current row across population changes
    if ((oldRow >= 0) && (oldRow < count())) {
        setCurrentRow(oldRow, QItemSelectionModel::Select);
    }
}

void ObjectPatternListWidget
    ::refreshItems(const Lonely::LaylaObjectPatternTable& objects__,
                int levelNum,
                int objectSetNum,
                const Lonely::LaylaGraphics& graphics,
                const Lonely::LaylaPalettes& palettes) {


    for (int i = 0; i < objects__.numPatternTokens(); i++) {
        item(i)->setText(objects__.descriptionString(i).c_str());
        item(i)->setData(Qt::UserRole, i);

        Graphic icon;
        objects__.drawEntryIcon(icon,
                                i,
                                levelNum,
                                objectSetNum,
                                graphics,
                                palettes);

        item(i)->setIcon(QIcon(
                             QtFormatConversion::graphicToPixmap(icon)));
    }
}
