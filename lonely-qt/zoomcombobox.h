#ifndef ZOOMCOMBOBOX_H
#define ZOOMCOMBOBOX_H

#include <QComboBox>

class ZoomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ZoomComboBox(QWidget *parent = 0);
    
    void addZoomLevel(double zoom);
signals:
    
public slots:
    
};

#endif // ZOOMCOMBOBOX_H
