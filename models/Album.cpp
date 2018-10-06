#include "Album.h"

#include <QMap>
#include <QJsonArray>
#include <QVariant>

namespace Qtify
{
    // To convert a string to a AlbumGroup.
    const QMap<QString, AlbumGroup> ALBUMGROUP_STRINGS
    {
        {"album"      , AlbumGroup_Album       },
        {"single"     , AlbumGroup_Single      },
        {"compilation", AlbumGroup_Compilation },
        {"appears_on" , AlbumGroup_AppearsOn   },
    };

    // To convert a string to a AlbumType.
    const QMap<QString, AlbumType> ALBUMTYPE_STRINGS
    {
        {"album"      , AlbumType_Album       },
        {"single"     , AlbumType_Single      },
        {"compilation", AlbumType_Compilation },
    };

    // To convert a string to a ReleaseDatePrecision.
    const QMap<QString, ReleaseDatePrecision> RELEASEDATEPRECISION_STRINGS
    {
        {"year" , ReleaseDataPrecision_Year  },
        {"month", ReleaseDataPrecision_Month },
        {"day"  , ReleaseDataPrecision_Day   },
    };

    Album::Album(const QJsonObject &json):
        album_group(ALBUMGROUP_STRINGS.value(json["album_group"].toString(), AlbumGroup_Unknown)),
        album_type(ALBUMTYPE_STRINGS.value(json["album_type"].toString(), AlbumType_Unknown)),
        available_markets(json["available_markets"].toVariant().toStringList()),
        external_urls(json["external_urls"].toObject()),
        href(json["href"].toString()),
        id(json["id"].toString()),
        name(json["name"].toString()),
        release_date(json["release_date"].toString()),
        release_date_precision(RELEASEDATEPRECISION_STRINGS.value(json["release_date_precision"].toString(), ReleaseDataPrecision_Unknown)),
        restrictions(json["restrictions"].toObject()),
        uri(json["uri"].toString())
    {
        auto jsonArtists = json["artists"].toArray();
        for (const auto &jsonArtist : jsonArtists)
        {
            artists.emplace_back(jsonArtist.toObject());
        }

        auto jsonImages = json["images"].toArray();
        for (const auto &jsonImage : jsonImages)
        {
            images.emplace_back(jsonImage.toObject());
        }
    }

    AlbumGroup Album::getAlbumGroup() const
    {
        return album_group;
    }

    AlbumType Album::getAlbumType() const
    {
        return album_type;
    }

    const std::vector<Artist> &Album::getArtists() const
    {
        return artists;
    }

    const QStringList &Album::getAvailableMarkets() const
    {
        return available_markets;
    }

    const ExternalUrl &Album::getExternalUrl() const
    {
        return external_urls;
    }

    const QString &Album::getApiEndPoint() const
    {
        return href;
    }

    const QString &Album::getId() const
    {
        return id;
    }

    const std::vector<Image> &Album::getImages() const
    {
        return images;
    }

    const QString &Album::getName() const
    {
        return name;
    }

    const QString &Album::getReleaseDate() const
    {
        return release_date;
    }

    ReleaseDatePrecision Album::getReleaseDatePrecision() const
    {
        return release_date_precision;
    }

    const Restrictions &Album::getRestrictions() const
    {
        return restrictions;
    }

    const QString &Album::getUri() const
    {
        return uri;
    }
}
