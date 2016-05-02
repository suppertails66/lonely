#include "levelviewwidget.h"
#include "ui_levelviewwidget.h"
#include <QWheelEvent>
#include <iostream>

LevelViewWidget::LevelViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LevelViewWidget)
{
    ui->setupUi(this);

/*    QObject::connect(ui->levelViewLabel,
                     SIGNAL(wheelEventTriggered(QWheelEvent*)),
                     this,
                     SLOT(viewWheelEvent(QWheelEvent*)));
    QObject::connect(ui->controlViewLabel,
                     SIGNAL(wheelEventTriggered(QWheelEvent*)),
                     this,
                     SLOT(viewWheelEvent(QWheelEvent*)));

    ui->levelViewLabel->installEventFilter(this);
    ui->controlViewLabel->installEventFilter(this); */
}

LevelViewWidget::~LevelViewWidget()
{
    delete ui;
}

InputQLabel& LevelViewWidget::levelViewLabel() {
    return *(ui->levelViewLabel);
}

InputQLabel& LevelViewWidget::controlViewLabel() {
    return *(ui->controlViewLabel);
}

QScrollBar& LevelViewWidget::horizontalScrollBar() {
    return *(ui->horizontalScrollBar);
}

QScrollBar& LevelViewWidget::verticalScrollBar() {
    return *(ui->verticalScrollBar);
}

void LevelViewWidget::viewWheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ShiftModifier) {
//        ui->horizontalScrollBar->event(event);
    }
    else {
//        ui->verticalScrollBar->event(event);
    }
}

/*bool LevelViewWidget::eventFilter(QObject* obj, QEvent* event) {
    // Intercept wheel events for the level view labels and send them
    // to the scroll bars instead
    if ((obj == ui->levelViewLabel)
            || (obj == ui->controlViewLabel)) {
        if (event->type() == QEvent::Wheel) {
            QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

            if (wheelEvent->modifiers() & Qt::ShiftModifier) {
                // QScrollBar's default implementation of the WheelEvent handler
                // uses the Shift modifier to indicate "faster" scrolling. I
                // prefer to use it to distinguish between horizontal and vertical
                // scrolling, so we have to clear the Shift modifier before passing
                // the event on.
                Qt::KeyboardModifiers mods = wheelEvent->modifiers();
                mods ^= Qt::ShiftModifier;
                wheelEvent->setModifiers(mods);

                QCoreApplication::sendEvent(ui->horizontalScrollBar,
                                            wheelEvent);
            }
            else {
                if (wheelEvent->modifiers() & Qt::ControlModifier) {
                    return false;
                }

                QCoreApplication::sendEvent(ui->verticalScrollBar,
                                            wheelEvent);
            }

            return true;
        }
        else {
            return false;
        }
    }
    else {
        return QObject::eventFilter(obj, event);
    }
} */
