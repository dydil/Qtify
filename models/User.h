#ifndef USER_H
#define USER_H

#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @class    User
    *
    * @brief    User information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/users-profile/get-current-users-profile/
    ***************************************************************************************************/
    class User
    {
        public:
            User(const QJsonObject &json);

            const QString   &getCountry()     const;
            const QString   &getDisplayName() const;
            const QString   &getEmail()       const;
            const QString   &getApiEndPoint() const;
            const QString   &getId()          const;
            const QString   &getProduct()     const;
            const QString   &getType()        const;
            const QString   &getSpotifyUri()  const;

        private:
            QString country;
            QString display_name;
            QString email;
            QString href;
            QString id;
            QString product;
            QString type;
            QString uri;

    };
}

#endif // USER_H
