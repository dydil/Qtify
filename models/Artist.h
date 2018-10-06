#ifndef ARTIST_H
#define ARTIST_H

#include "ExternalUrl.h"

namespace Qtify
{
    /** ************************************************************************************************
    * @class    Artist
    *
    * @brief    Artist information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#artist-object-simplified
    ***************************************************************************************************/
    class Artist
    {
        public:
            Artist(const QJsonObject &json);

            const ExternalUrl &getExternalUrl() const;
            const QString &getApiEndPoint() const;
            const QString &getId() const;
            const QString &getName() const;
            const QString &getUri() const;

        private:
            ExternalUrl external_urls;
            QString href;
            QString id;
            QString name;
            QString uri;
    };
}

#endif // ARTIST_H
