#ifndef CACHEDQIMAGE_H
#define CACHEDQIMAGE_H

#include <QImage>
#include "qtformatconversion.h"

class CachedQImage
{
public:
    CachedQImage();

    void clear();

    void directInit(Lonely::Graphic& graphic);

    void drawGraphic(Lonely::Graphic& graphic);

    void drawGraphicWithAlpha(Lonely::Graphic& graphic);

    QImage& image();
protected:
    QImage image_;

    void resizeIfNeeded(Lonely::Graphic& graphic);
};

#endif // CACHEDQIMAGE_H
