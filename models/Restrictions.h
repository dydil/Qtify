#ifndef RESTRICTIONS_H
#define RESTRICTIONS_H

#include <QString>
#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @class    Restrictions
    *
    * @brief    Restrictions information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/
    ***************************************************************************************************/
    class Restrictions
    {
        public:
            Restrictions(const QJsonObject &json);

            const QString &getReason() const;

        private:
            QString reason;
    };
}

#endif // RESTRICTIONS_H
