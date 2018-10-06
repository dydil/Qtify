#ifndef TRACKLINK_H
#define TRACKLINK_H

#include "ExternalUrl.h"

namespace Qtify
{
    /** ************************************************************************************************
    * @class    TrackLink
    *
    * @brief    Track link information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#track-link
    ***************************************************************************************************/
    class TrackLink
    {
        public:
            TrackLink(const QJsonObject &json);

            const ExternalUrl &getExternalUrl() const;
            const QString &getApiEndPoint() const;
            const QString &getId() const;
            const QString &getUri() const;

        private:
            ExternalUrl external_urls;
            QString href;
            QString id;
            QString uri;
    };
}

#endif // TRACKLINK_H
