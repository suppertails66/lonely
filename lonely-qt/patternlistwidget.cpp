#include "patternlistwidget.h"
#include "util/StringConversion.h"
#include "cachedqimage.h"
#include <cstdlib>

using namespace Lonely;

PatternListWidget::PatternListWidget(QWidget *parent) :
    QListWidget(parent),
    levelEditor_(NULL)
{
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setMaximumWidth(224);

    setIconSize(QSize(48, 112));
    setGridSize(QSize(48, 128));
    setViewMode(QListWidget::IconMode);
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setMovement(QListWidget::Static);
    setFlow(QListWidget::LeftToRight);
    setWrapping(true);
}

void PatternListWidget::setLevelEditor(Lonely::LonelyLevelEditor& levelEditor__) {
    levelEditor_ = &levelEditor__;
}

void PatternListWidget::refreshPatternList() {
    PatternPreviewCollection patternPreviews;
    levelEditor_->generatePatternPreviews(patternPreviews);

    clear();

    for (int i = 0; i < patternPreviews.size(); i++) {
        addItem(StringConversion::toString(i).c_str());
        item(i)->setData(Qt::UserRole, i);
        CachedQImage image;
        image.directInit(patternPreviews[i]);
        item(i)->setIcon(QPixmap().fromImage(image.image()));
    }
}
