#ifndef AREATYPECOMBOBOX_H
#define AREATYPECOMBOBOX_H

#include <QComboBox>
#include "structs/Tbyte.h"
#include "structs/Tstring.h"

class AreaTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AreaTypeComboBox(QWidget *parent = 0);
    
    void selectFromAreaType(Lonely::Tbyte areaType);

    void changeAreaText(Lonely::Tbyte areaType,
                        const Lonely::Tstring& text);
protected:
    int indexFromAreaType(Lonely::Tbyte areaType) const;
signals:
    
public slots:
    
};

#endif // AREATYPECOMBOBOX_H
