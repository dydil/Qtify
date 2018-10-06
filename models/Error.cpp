#include "Error.h"

namespace Qtify
{
    Error::Error(const QJsonObject &json):
        status(json["status"].toInt()),
        message(json["message"].toString())
    {

    }

    int Error::getStatus() const
    {
        return status;
    }

    const QString &Error::getMessage() const
    {
        return message;
    }
}
