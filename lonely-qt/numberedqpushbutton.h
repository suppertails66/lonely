#ifndef NUMBEREDQPUSHBUTTON_H
#define NUMBEREDQPUSHBUTTON_H

#include <QPushButton>

class NumberedQPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit NumberedQPushButton(int number__,
                                QWidget *parent = 0);

    int number() const;
    void setNumber(int number__);
protected:
    int number_;
    
signals:
    void clicked(int num, bool checked);

public slots:

private slots:
    void on_StandardClick(bool checked);
};

#endif // NUMBEREDQPUSHBUTTON_H
