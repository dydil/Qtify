#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @class    Image
    *
    * @brief    Image information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#image-object
    ***************************************************************************************************/
    class Image
    {
        public:
            Image(const QJsonObject &json);

            int getHeight() const;
            const QString &getUrl() const;
            int getWidth() const;

        private:
            int height;
            QString url;
            int width;
    };
}

#endif // IMAGE_H
