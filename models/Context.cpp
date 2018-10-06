#include "Context.h"

#include <QMap>

namespace Qtify
{
    // To convert a string to a ContextType.
    const QMap<QString, ContextType> CONTEXT_TYPE_STRINGS
    {
        {"album"   , Context_Album    },
        {"artist"  , Context_Artist   },
        {"playlist", Context_Playlist },
    };

    Context::Context(const QJsonObject &json):
        uri(json["uri"].toString()),
        href(json["href"].toString()),
        external_urls(json["external_urls"].toObject()),
        type(CONTEXT_TYPE_STRINGS.value(json["type"].toString(), Context_Unknown))
    {

    }

    const QString &Qtify::Context::getUri() const
    {
        return uri;
    }

    const QString &Qtify::Context::getHref() const
    {
        return href;
    }

    const Qtify::ExternalUrl &Qtify::Context::getExternalUrl() const
    {
        return external_urls;
    }

    Qtify::ContextType Qtify::Context::getType() const
    {
        return type;
    }
}
