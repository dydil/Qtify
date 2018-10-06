#include "TrackLink.h"

namespace Qtify
{
    TrackLink::TrackLink(const QJsonObject &json):
        external_urls(json["external_urls"].toObject()),
        href(json["href"].toString()),
        id(json["id"].toString()),
        uri(json["uri"].toString())
    {

    }

    const ExternalUrl &TrackLink::getExternalUrl() const
    {
        return external_urls;
    }

    const QString &TrackLink::getApiEndPoint() const
    {
        return href;
    }

    const QString &TrackLink::getId() const
    {
        return id;
    }

    const QString &TrackLink::getUri() const
    {
        return uri;
    }
}
