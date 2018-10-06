#ifndef EXTERNALURL_H
#define EXTERNALURL_H

#include <QString>
#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @class    ExternalUrl
    *
    * @brief    ExternalUrl information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#external-url-object
    ***************************************************************************************************/
    class ExternalUrl
    {
        public:
            ExternalUrl(const QJsonObject &json);

            const QString &getKey() const;
            const QString &getValue() const;

        private:
            QString key;
            QString value;
    };
}

#endif // EXTERNALURL_H
