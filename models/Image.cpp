#include "Image.h"

namespace Qtify
{
    Image::Image(const QJsonObject &json):
        height(json["height"].toInt()),
        url(json["url"].toString()),
        width(json["width"].toInt())
    {

    }

    int Image::getHeight() const
    {
        return height;
    }

    const QString &Image::getUrl() const
    {
        return url;
    }

    int Image::getWidth() const
    {
        return width;
    }
}
