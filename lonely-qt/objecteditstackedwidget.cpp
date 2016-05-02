#include "objecteditstackedwidget.h"
#include "delayeditwidget.h"
#include "standardspawneditwidget.h"
#include "objecteditorsubwidget.h"
#include "lonelyglobals.h"
#include "gamedata/LaylaObjectPatternToken.h"

using namespace Lonely;

ObjectEditStackedWidget::ObjectEditStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    objectView_(globals_.editor().levelEditor().objectView())
{
    for (int i = 0;
         i < LaylaObjectPatternTypes::numObjectPatternTypes;
         i++) {
        LaylaObjectPatternTypes::LaylaObjectPatternType patternType
                = static_cast<LaylaObjectPatternTypes::LaylaObjectPatternType>(
                    i);

        switch (patternType) {
        case LaylaObjectPatternTypes::none:
            addWidget(new ObjectEditorSubWidget(this));
            break;
        case LaylaObjectPatternTypes::delay:
            addWidget(new DelayEditWidget(this));
            break;
        case LaylaObjectPatternTypes::standardSpawn:
            addWidget(new StandardSpawnEditWidget(this));
            break;
        case LaylaObjectPatternTypes::expandedSpawn:
            addWidget(new ObjectEditorSubWidget(this));
            break;
        default:
            break;
        }
    }
}

void ObjectEditStackedWidget::editToken(Lonely::LaylaObjectPatternToken& token) {
    setCurrentIndex(token.type());

    ObjectEditorSubWidget& targetWidget
            = *(dynamic_cast<ObjectEditorSubWidget*>(currentWidget()));

    switch (token.type()) {
    case LaylaObjectPatternTypes::none:
        break;
    case LaylaObjectPatternTypes::delay:
        dynamic_cast<DelayEditWidget&>(targetWidget)
                .setDelayToken(
                    dynamic_cast<LaylaObjectDelayToken&>(token));
        break;
    case LaylaObjectPatternTypes::standardSpawn:
        dynamic_cast<StandardSpawnEditWidget&>(targetWidget)
                .setSpawnToken(
                    dynamic_cast<LaylaObjectStandardSpawnToken&>(token));
        break;
    case LaylaObjectPatternTypes::expandedSpawn:
        break;
    default:
        break;
    }

    targetWidget.refreshDisplay();
}

void ObjectEditStackedWidget::refreshActiveWidget() {
    ObjectEditorSubWidget& targetWidget
            = *(dynamic_cast<ObjectEditorSubWidget*>(currentWidget()));

    targetWidget.refreshDisplay();
}
