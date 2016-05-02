#include "standardzoomcombobox.h"
#include "util/StringConversion.h"

using namespace Lonely;

StandardZoomComboBox::StandardZoomComboBox(QWidget* parent)
    : ZoomComboBox(parent) {
    addZoomLevel(0.25);
    addZoomLevel(0.50);
    addZoomLevel(1.00);
    addZoomLevel(2.00);
    addZoomLevel(3.00);
    addZoomLevel(4.00);
    setCurrentIndex(2); // 100%
}
