#include "Device.h"

namespace Qtify
{
    Device::Device(const QJsonObject &json):
        id(json["id"].toString()),
        is_active(json["is_active"].toBool()),
        is_private_session(json["is_private_session"].toBool()),
        is_restricted(json["is_restricted"].toBool()),
        name(json["name"].toString()),
        type(json["type"].toString()),
        volume_percent(json["volume_percent"].toInt())
    {

    }

    const QString &Device::getId() const
    {
        return id;
    }

    bool Device::isActive() const
    {
        return is_active;
    }

    bool Device::isPrivateSession() const
    {
        return is_private_session;
    }

    bool Device::isRestricted() const
    {
        return is_restricted;
    }

    const QString &Device::getName() const
    {
        return name;
    }

    const QString &Device::getType() const
    {
        return type;
    }

    int Device::getVolumePercent() const
    {
        return volume_percent;
    }

}
