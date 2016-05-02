#include "palettecombobox.h"
#include "util/StringConversion.h"

using namespace Lonely;

PaletteComboBox::PaletteComboBox(QWidget *parent) :
    QComboBox(parent)
{
    for (int i = 0; i < 4; i++) {
        addItem(StringConversion::toString(i).c_str(),
                i);
    }
}
