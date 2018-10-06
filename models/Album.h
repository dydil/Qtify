#ifndef ALBUM_H
#define ALBUM_H

#include <vector>

#include <QStringList>

#include "ExternalUrl.h"
#include "ExternalId.h"
#include "Artist.h"
#include "Image.h"
#include "Restrictions.h"

namespace Qtify
{
    enum AlbumGroup
    {
        AlbumGroup_Album,
        AlbumGroup_Single,
        AlbumGroup_Compilation,
        AlbumGroup_AppearsOn,
        AlbumGroup_Unknown,
    };

    enum AlbumType
    {
        AlbumType_Album,
        AlbumType_Single,
        AlbumType_Compilation,
        AlbumType_Unknown,
    };

    enum ReleaseDatePrecision
    {
        ReleaseDataPrecision_Year,
        ReleaseDataPrecision_Month,
        ReleaseDataPrecision_Day,
        ReleaseDataPrecision_Unknown,
    };

    /** ************************************************************************************************
    * @class    Album
    *
    * @brief    Album information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#album-object-simplified
    ***************************************************************************************************/
    class Album
    {
        public:
            Album(const QJsonObject &json);

            AlbumGroup getAlbumGroup() const;
            AlbumType  getAlbumType() const;
            const std::vector<Artist> &getArtists() const;
            const QStringList &getAvailableMarkets() const;
            const ExternalUrl &getExternalUrl() const;
            const QString &getApiEndPoint() const;
            const QString &getId() const;
            const std::vector<Image> &getImages() const;
            const QString &getName() const;
            const QString &getReleaseDate() const;
            ReleaseDatePrecision getReleaseDatePrecision() const;
            const Restrictions &getRestrictions() const;
            const QString &getUri() const;

        private:
            AlbumGroup album_group;
            AlbumType  album_type;
            std::vector<Artist> artists; // Need to use std::vector because QVector doesn't support emplace_back
            QStringList available_markets;
            ExternalUrl external_urls;
            QString href;
            QString id;
            std::vector<Image> images; // Need to use std::vector because QVector doesn't support emplace_back
            QString name;
            QString release_date;
            ReleaseDatePrecision release_date_precision;
            Restrictions restrictions;
            QString uri;
    };

}

#endif // ALBUM_H
