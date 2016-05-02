#ifndef QTFORMATCONVERSION_H
#define QTFORMATCONVERSION_H

#include "structs/Graphic.h"
#include <QPixmap>
#include <QImage>

class QtFormatConversion
{
public:
    static QPixmap graphicToPixmap(Lonely::Graphic& graphic);

    static QImage graphicToImage(Lonely::Graphic& graphic);

    static void drawGraphicToImage(QImage& image,
                                   Lonely::Graphic& graphic);

    static void drawGraphicToImageWithAlpha(QImage& image,
                                   Lonely::Graphic& graphic);
protected:

};

#endif // QTFORMATCONVERSION_H
