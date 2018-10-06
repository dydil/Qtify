#include "ExternalId.h"

namespace Qtify
{
    ExternalId::ExternalId(const QJsonObject &json)
    {
        auto iterator = json.begin();
        if (iterator != json.end())
        {
            key   = iterator.key();
            value = iterator.value().toString();
        }
    }

    const QString &Qtify::ExternalId::getKey() const
    {
        return key;
    }

    const QString &ExternalId::getValue() const
    {
        return value;
    }
}
