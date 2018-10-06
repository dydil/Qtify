#ifndef TRACK_H
#define TRACK_H

#include "ExternalUrl.h"
#include "Album.h"
#include "TrackLink.h"
#include "Restrictions.h"

namespace Qtify
{
    /** ************************************************************************************************
    * @class    Track
    *
    * @brief    Track information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#track-object-full
    ***************************************************************************************************/
    class Track
    {
        public:
            Track(const QJsonObject &json);

            const Album &getAlbum() const;
            const std::vector<Artist> &getArtists() const;
            const QStringList &getAvailableMarkets() const;
            int getDiscNumber() const;
            int getDurationMilliseconds() const;
            bool hasExplicitLyrics() const;
            const ExternalId &getExternalId() const;
            const ExternalUrl &getExternalUrl() const;
            const QString &getApiEndPoint() const;
            const QString &getId() const;
            bool isPlayable() const;
            const TrackLink &getOriginalRequestedTrack() const;
            const Restrictions &getRestrictions() const;
            const QString &getName() const;
            int getPopularity() const;
            const QString &getPreviewUrl() const;
            int getTrackNumber() const;
            const QString &getUri() const;
            bool isLocalFile() const;

        private:
            Album album;
            std::vector<Artist> artists; // Need to use std::vector because QVector doesn't support emplace_back
            QStringList available_markets;
            int disc_number;
            int duration_ms;
            bool explicit_lyrics;
            ExternalId external_ids;
            ExternalUrl external_urls;
            QString href;
            QString id;
            bool is_playable;
            TrackLink linked_from;
            Restrictions restrictions;
            QString name;
            int popularity;
            QString preview_url;
            int track_number;
            QString uri;
            bool is_local;
    };
}

#endif // TRACK_H
