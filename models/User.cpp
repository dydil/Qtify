#include "User.h"

namespace Qtify
{
    User::User(const QJsonObject &json):
        country     (json["country"     ].toString()),
        display_name(json["display_name"].toString()),
        email       (json["email"       ].toString()),
        href        (json["href"        ].toString()),
        id          (json["id"          ].toString()),
        product     (json["product"     ].toString()),
        type        (json["type"        ].toString()),
        uri         (json["uri"         ].toString())
    {

    }

    const QString &User::getCountry() const
    {
        return country;
    }

    const QString &User::getDisplayName() const
    {
        return display_name;
    }

    const QString &User::getEmail() const
    {
        return email;
    }

    const QString &User::getApiEndPoint() const
    {
        return href;
    }

    const QString &User::getId() const
    {
        return id;
    }

    const QString &User::getProduct() const
    {
        return product;
    }

    const QString &User::getType() const
    {
        return type;
    }

    const QString &User::getSpotifyUri() const
    {
        return uri;
    }
}
