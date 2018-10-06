#include "Artist.h"

namespace Qtify
{
    Artist::Artist(const QJsonObject &json):
        external_urls(json["external_urls"].toObject()),
        href(json["href"].toString()),
        id(json["id"].toString()),
        name(json["name"].toString()),
        uri(json["uri"].toString())
    {

    }

    const ExternalUrl &Artist::getExternalUrl() const
    {
        return external_urls;
    }

    const QString &Artist::getApiEndPoint() const
    {
        return href;
    }

    const QString &Artist::getId() const
    {
        return id;
    }

    const QString &Artist::getName() const
    {
        return name;
    }

    const QString &Artist::getUri() const
    {
        return uri;
    }
}
