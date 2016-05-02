#include "numberedqpushbutton.h"

NumberedQPushButton::NumberedQPushButton(
        int number__,
        QWidget *parent) :
    QPushButton(parent),
    number_(number__)
{
    QObject::connect(this,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(on_StandardClick(bool)));
}

int NumberedQPushButton::number() const {
    return number_;
}

void NumberedQPushButton::setNumber(int number__) {
    number_ = number__;
}

void NumberedQPushButton::on_StandardClick(bool checked) {
    emit clicked(number_, checked);
}
