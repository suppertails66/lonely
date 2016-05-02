#include "cachedqimage.h"

CachedQImage::CachedQImage()
{
}

void CachedQImage::clear() {
    image_ = QImage();
}

void CachedQImage::directInit(Lonely::Graphic& graphic) {
    image_ = QImage((const uchar*)(graphic.imgdat()),
                    graphic.w(),
                    graphic.h(),
                    QImage::Format_ARGB32);
}

void CachedQImage::drawGraphic(Lonely::Graphic& graphic) {
    resizeIfNeeded(graphic);

    QtFormatConversion::drawGraphicToImage(
                image_,
                graphic);
}

void CachedQImage::drawGraphicWithAlpha(Lonely::Graphic& graphic) {
    resizeIfNeeded(graphic);

    QtFormatConversion::drawGraphicToImageWithAlpha(
                image_,
                graphic);
}

QImage& CachedQImage::image() {
    return image_;
}

void CachedQImage::resizeIfNeeded(Lonely::Graphic& graphic) {
    if ((image_.width() != graphic.w())
            || (image_.height() != graphic.h())) {
        image_ = QImage(graphic.w(), graphic.h(), image_.format());
    }
}
