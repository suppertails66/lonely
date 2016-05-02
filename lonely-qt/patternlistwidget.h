#ifndef PATTERNLISTWIDGET_H
#define PATTERNLISTWIDGET_H

#include <QListWidget>
#include "editor/LonelyLevelEditor.h"

class PatternListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit PatternListWidget(QWidget *parent = 0);
    
    void setLevelEditor(Lonely::LonelyLevelEditor& levelEditor__);

    void refreshPatternList();
signals:
    
public slots:

protected:
    Lonely::LonelyLevelEditor* levelEditor_;
};

#endif // PATTERNLISTWIDGET_H
