#include "Track.h"

#include <QJsonArray>
#include <QVariant>

namespace Qtify
{
    Track::Track(const QJsonObject &json):
        album(json["album"].toObject()),
        available_markets(json["available_markets"].toVariant().toStringList()),
        disc_number(json["disc_number"].toInt()),
        duration_ms(json["duration_ms"].toInt()),
        explicit_lyrics(json["explicit"].toBool(false)),
        external_ids(json["external_ids"].toObject()),
        external_urls(json["external_urls"].toObject()),
        href(json["href"].toString()),
        id(json["id"].toString()),
        is_playable(json["is_playable"].toBool(true)),
        linked_from(json["linked_from"].toObject()),
        restrictions(json["restrictions"].toObject()),
        name(json["name"].toString()),
        popularity(json["popularity"].toInt()),
        preview_url(json["preview_url"].toString()),
        track_number(json["track_number"].toInt()),
        uri(json["uri"].toString()),
        is_local(json["is_local"].toBool())
    {
        auto jsonArtists = json["artists"].toArray();

        for (const auto &jsonArtist : jsonArtists)
        {
            artists.emplace_back(jsonArtist.toObject());
        }
    }

    const Album &Track::getAlbum() const
    {
        return album;
    }

    const std::vector<Artist> &Track::getArtists() const
    {
        return artists;
    }

    const QStringList &Track::getAvailableMarkets() const
    {
        return available_markets;
    }

    int Track::getDiscNumber() const
    {
        return disc_number;
    }

    int Track::getDurationMilliseconds() const
    {
        return duration_ms;
    }

    bool Track::hasExplicitLyrics() const
    {
        return explicit_lyrics;
    }

    const ExternalId &Track::getExternalId() const
    {
        return external_ids;
    }

    const ExternalUrl &Track::getExternalUrl() const
    {
        return external_urls;
    }

    const QString &Track::getApiEndPoint() const
    {
        return href;
    }

    const QString &Track::getId() const
    {
        return id;
    }

    bool Track::isPlayable() const
    {
        return is_playable;
    }

    const TrackLink &Track::getOriginalRequestedTrack() const
    {
        return linked_from;
    }

    const Restrictions &Track::getRestrictions() const
    {
        return restrictions;
    }

    const QString &Track::getName() const
    {
        return name;
    }

    int Track::getPopularity() const
    {
        return popularity;
    }

    const QString &Track::getPreviewUrl() const
    {
        return preview_url;
    }

    int Track::getTrackNumber() const
    {
        return track_number;
    }

    const QString &Track::getUri() const
    {
        return uri;
    }

    bool Track::isLocalFile() const
    {
        return is_local;
    }
}
