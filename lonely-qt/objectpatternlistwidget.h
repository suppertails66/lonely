#ifndef OBJECTPATTERNLISTWIDGET_H
#define OBJECTPATTERNLISTWIDGET_H

#include <QListWidget>
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "gamedata/LaylaObjectPatternTable.h"
#include "structs/Graphic.h"
#include "nes/NesPatternTable.h"
#include "nes/NesPaletteQuad.h"

class ObjectPatternListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ObjectPatternListWidget(QWidget *parent = 0);
    
    void repopulate(const Lonely::LaylaObjectPatternTable& objects__,
                    int objectSetNum,
                    const Lonely::NesPatternTable& patternTable,
                    const Lonely::NesPaletteQuad& palettes);

    void repopulate(const Lonely::LaylaObjectPatternTable& objects__,
                    int levelNum,
                    int objectSetNum,
                    const Lonely::LaylaGraphics& graphics,
                    const Lonely::LaylaPalettes& palettes);

    void refreshItems(const Lonely::LaylaObjectPatternTable& objects__,
                    int levelNum,
                    int objectSetNum,
                    const Lonely::LaylaGraphics& graphics,
                    const Lonely::LaylaPalettes& palettes);
protected:
    void repopulateBase(const Lonely::LaylaObjectPatternTable& objects__);

signals:
    
public slots:
    
};

#endif // OBJECTPATTERNLISTWIDGET_H
