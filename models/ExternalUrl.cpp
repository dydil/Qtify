#include "ExternalUrl.h"

namespace Qtify
{
    ExternalUrl::ExternalUrl(const QJsonObject &json)
    {
        auto iterator = json.begin();
        if (iterator != json.end())
        {
            key   = iterator.key();
            value = iterator.value().toString();
        }
    }

    const QString &Qtify::ExternalUrl::getKey() const
    {
        return key;
    }

    const QString &ExternalUrl::getValue() const
    {
        return value;
    }
}
