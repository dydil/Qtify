#include "AuthenticationError.h"

#include <QMap>

namespace Qtify
{
    // To convert a string to a AuthenticationError.
    const QMap<QString, AuthenticationErrorType> AUTH_ERROR_STRINGS
    {
        {"invalid_request",        AuthenticationError_InvalidRequest      },
        {"invalid_client",         AuthenticationError_InvalidClient       },
        {"invalid_grant",          AuthenticationError_InvalidGrant        },
        {"unauthorized_client",    AuthenticationError_UnauthorizedClient  },
        {"unsupported_grant_type", AuthenticationError_UnsupportedGrantType},
        {"invalid_scope",          AuthenticationError_InvalidScope        },
    };

    AuthenticationError::AuthenticationError(const QJsonObject &json):
        error(AUTH_ERROR_STRINGS.value(json["error"].toString(), AuthenticationError_Unkown)),
        error_description(json["error_description"].toString())
    {

    }

    AuthenticationErrorType AuthenticationError::getError() const
    {
        return error;
    }

    const QString &AuthenticationError::getErrorDescription() const
    {
        return error_description;
    }
}
