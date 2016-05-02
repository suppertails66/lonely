#include "numberedqaction.h"

NumberedQAction::NumberedQAction(int number__,
                const QIcon& icon,
                const QString& text,
                QObject *parent) :
    QAction(icon, text, parent),
    number_(number__) {
    QObject::connect(this,
                     SIGNAL(triggered()),
                     this,
                     SLOT(on_StandardTrigger()));
}

int NumberedQAction::number() const {
    return number_;
}

void NumberedQAction::setNumber(int number__) {
    number_ = number__;
}

void NumberedQAction::on_StandardTrigger() {
    emit triggered(number_);
}
