#ifndef DEVICE_H
#define DEVICE_H

#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @class    Device
    *
    * @brief    Device information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/player/get-a-users-available-devices/
    ***************************************************************************************************/
    class Device
    {
        public:
            Device(const QJsonObject &json);

            const QString &getId() const;
            bool isActive() const;
            bool isPrivateSession() const;
            bool isRestricted() const;
            const QString &getName() const;
            const QString &getType() const;
            int getVolumePercent() const;

        private:
            QString id;
            bool is_active;
            bool is_private_session;
            bool is_restricted;
            QString name;
            QString type;
            int volume_percent;
    };
}

#endif // DEVICE_H
