#ifndef NUMBEREDQACTION_H
#define NUMBEREDQACTION_H

#include <QAction>

class NumberedQAction : public QAction
{
    Q_OBJECT
public:
    NumberedQAction(int number__,
                    const QIcon& icon,
                    const QString& text,
                    QObject *parent = 0);

    int number() const;
    void setNumber(int number__);

protected:
    int number_;

signals:
    void triggered(int num);

public slots:

private slots:
    void on_StandardTrigger();
    
};

#endif // NUMBEREDQACTION_H
