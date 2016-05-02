#include "areatypecombobox.h"
#include "gamedata/LaylaPatternToken.h"

using namespace Lonely;

AreaTypeComboBox::AreaTypeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    addItem("Cave",
            SetAreaToken::caveID);
    addItem("Base",
            SetAreaToken::baseFlag);
    addItem("Boss",
            SetAreaToken::bossFlag);
}

void AreaTypeComboBox::selectFromAreaType(Lonely::Tbyte areaType) {
    setCurrentIndex(indexFromAreaType(areaType));
}

void AreaTypeComboBox::changeAreaText(Lonely::Tbyte areaType,
                    const Lonely::Tstring& text) {
    setItemText(indexFromAreaType(areaType),
                text.c_str());
}

int AreaTypeComboBox::indexFromAreaType(Lonely::Tbyte areaType) const {
    if (areaType & SetAreaToken::bossFlag) {
        return 2;
    }
    else if (areaType & SetAreaToken::baseFlag) {
        return 1;
    }
    else {
        return 0;
    }
}
