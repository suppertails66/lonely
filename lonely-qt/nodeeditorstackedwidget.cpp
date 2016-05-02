#include "nodeeditorstackedwidget.h"
#include "blankedwidget.h"
#include "spawnpointedwidget.h"
#include "spawnbossedwidget.h"
#include "setscrollingedwidget.h"
#include "setareaedwidget.h"
#include "setmapedwidget.h"
#include "setelevatorsedwidget.h"
#include "jumptopositionedwidget.h"
#include "gamedata/LaylaArea.h"
#include "gamedata/LaylaPatternToken.h"
#include <cstdlib>

using namespace Lonely;

NodeEditorStackedWidget::NodeEditorStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    token_(NULL)
{
    for (int i = 0;
         i < LaylaPatternTypes::numLaylaPatternTypes;
         i++) {
        LaylaPatternTypes::LaylaPatternType patternType
                = static_cast<LaylaPatternTypes::LaylaPatternType>(i);

        switch (patternType) {
        case LaylaPatternTypes::spawnBoss:
            addWidget(new SpawnBossEdWidget());
            break;
        case LaylaPatternTypes::setScrolling:
            addWidget(
                        new SetScrollingEdWidget());
            break;
        case LaylaPatternTypes::setArea:
            addWidget(
                        new SetAreaEdWidget());
            break;
        case LaylaPatternTypes::setMap:
            addWidget(
                        new SetMapEdWidget());
            break;
        case LaylaPatternTypes::setElevators:
            addWidget(
                        new SetElevatorsEdWidget());
            break;
        case LaylaPatternTypes::jumpToPosition:
            addWidget(
                        new JumpToPositionEdWidget());
            break;

        case LaylaPatternTypes::spawnPoint:
        case LaylaPatternTypes::standardPattern:
        default:
            addWidget(
                        new BlankEdWidget());
            break;
        }
    }
}

void NodeEditorStackedWidget::setToken(Lonely::LaylaPatternToken* token__) {

    token_ = token__;
    LaylaPatternToken& patternToken
            = *token__;
    LaylaPatternTypes::LaylaPatternType patternType
            = patternToken.type();
    setCurrentIndex(
                    patternType);
    switch (patternType) {
    case LaylaPatternTypes::spawnBoss:
    {
        SpawnBossEdWidget* widget = dynamic_cast<SpawnBossEdWidget*>(
                    currentWidget());
        widget->setToken(dynamic_cast<SpawnBossToken&>(patternToken));
    }
        break;
    case LaylaPatternTypes::setScrolling:
    {
        SetScrollingEdWidget* widget = dynamic_cast<SetScrollingEdWidget*>(
                    currentWidget());
        widget->setToken(dynamic_cast<SetScrollingToken&>(patternToken));
    }
        break;
    case LaylaPatternTypes::setArea:
    {
        SetAreaEdWidget* widget = dynamic_cast<SetAreaEdWidget*>(
                    currentWidget());
        widget->setToken(dynamic_cast<SetAreaToken&>(patternToken));
    }
        break;
    case LaylaPatternTypes::setMap:
    {
        SetMapEdWidget* widget = dynamic_cast<SetMapEdWidget*>(
                    currentWidget());
        widget->setToken(dynamic_cast<SetMapToken&>(patternToken));
    }
        break;
    case LaylaPatternTypes::setElevators:
    {
        SetElevatorsEdWidget* widget = dynamic_cast<SetElevatorsEdWidget*>(
                    currentWidget());
        widget->setToken(dynamic_cast<SetElevatorsToken&>(patternToken));
    }
        break;
    case LaylaPatternTypes::jumpToPosition:
    {
        JumpToPositionEdWidget* widget = dynamic_cast<JumpToPositionEdWidget*>(
                    currentWidget());
        widget->setToken(dynamic_cast<JumpToPositionToken&>(patternToken));
    }
        break;
    default:
        break;
    }
}

void NodeEditorStackedWidget::deleteToken() {
    delete token_;
}
