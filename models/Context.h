#ifndef CONTEXT_H
#define CONTEXT_H

#include <QString>

#include "ExternalUrl.h"

namespace Qtify
{
    enum ContextType
    {
        Context_Album,
        Context_Artist,
        Context_Playlist,
        Context_Unknown,
    };

    /** ************************************************************************************************
    * @class    Context
    *
    * @brief    Context information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#context-object
    ***************************************************************************************************/
    class Context
    {
        public:
            Context(const QJsonObject &json);

            const QString &getUri() const;
            const QString &getHref() const;
            const ExternalUrl &getExternalUrl() const;
            ContextType getType() const;

        private:
            QString uri;
            QString href;
            ExternalUrl external_urls;
            ContextType type;
    };

}

#endif // CONTEXT_H
