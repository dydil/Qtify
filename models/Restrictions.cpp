#include "Restrictions.h"

namespace Qtify
{
    Restrictions::Restrictions(const QJsonObject &json):
        reason(json["reason"].toString())
    {

    }

    const QString &Restrictions::getReason() const
    {
        return reason;
    }
}
