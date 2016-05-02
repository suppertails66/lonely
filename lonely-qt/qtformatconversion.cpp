#include "qtformatconversion.h"
#include <QImage>
#include <QPixmap>

using namespace Lonely;

QPixmap QtFormatConversion::graphicToPixmap(Graphic& graphic) {
    QImage temp(graphic.w(), graphic.h(), QImage::Format_RGB32);

    for (int j = 0; j < graphic.h(); j++) {
        QRgb* rgb = (QRgb*)(temp.scanLine(j));

        for (int i = 0; i < graphic.w(); i++) {
            Tcolor color = graphic.getPixel(i, j);
            (*rgb) = qRgba(color.r(),
                           color.g(),
                           color.b(),
                           color.a());
            ++rgb;
        }
    }

    return QPixmap().fromImage(temp);
}

QImage QtFormatConversion::graphicToImage(Lonely::Graphic& graphic) {
    QImage image(graphic.w(), graphic.h(), QImage::Format_ARGB32);

    for (int j = 0; j < graphic.h(); j++) {
        QRgb* rgb = (QRgb*)(image.scanLine(j));

        for (int i = 0; i < graphic.w(); i++) {
            Tcolor color = graphic.getPixel(i, j);
            (*rgb) = qRgba(color.r(),
                           color.g(),
                           color.b(),
                           color.a());
            ++rgb;
        }
    }

    return image;
}

void QtFormatConversion::drawGraphicToImage(QImage& image,
                               Lonely::Graphic& graphic) {
    if ((image.width() != graphic.w())
        || (image.height() != graphic.h())) {
        image = QImage(graphic.w(), graphic.h(), QImage::Format_RGB32);
    }

    for (int j = 0; j < graphic.h(); j++) {
        QRgb* rgb = (QRgb*)(image.scanLine(j));

        for (int i = 0; i < graphic.w(); i++) {
            Tcolor color = graphic.getPixel(i, j);
            (*rgb) = qRgba(color.r(),
                           color.g(),
                           color.b(),
                           color.a());
            ++rgb;
        }
    }
}

void QtFormatConversion::drawGraphicToImageWithAlpha(QImage& image,
                               Lonely::Graphic& graphic) {
    // extremely lazy cheat
    image = QImage(graphic.w(), graphic.h(), QImage::Format_ARGB32);
    drawGraphicToImage(image, graphic);
}
