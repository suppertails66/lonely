#include "zoomcombobox.h"
#include "util/StringConversion.h"

using namespace Lonely;

ZoomComboBox::ZoomComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void ZoomComboBox::addZoomLevel(double zoom) {
    std::string label = StringConversion::toString<int>((zoom * 100));
    label += "%";

    addItem(label.c_str(), (double)zoom);
}
