#ifndef EXTERNALID_H
#define EXTERNALID_H

#include <QString>
#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @class    ExternalId
    *
    * @brief    ExternalId information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#external-id-object
    ***************************************************************************************************/
    class ExternalId
    {
        public:
            ExternalId(const QJsonObject &json);

            const QString &getKey() const;
            const QString &getValue() const;

        private:
            QString key;
            QString value;
    };
}

#endif // EXTERNALID_H
