#ifndef AUTHENTICATIONERROR_H
#define AUTHENTICATIONERROR_H

#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @enum     AuthenticationErrorType
    *
    * @brief    List of possible authentication errors according to RFC 6749 Section 5.2.
    *
    * @details  More info on https://tools.ietf.org/html/rfc6749#section-5.2
    ***************************************************************************************************/
    enum AuthenticationErrorType
    {
        AuthenticationError_InvalidRequest,
        AuthenticationError_InvalidClient,
        AuthenticationError_InvalidGrant,
        AuthenticationError_UnauthorizedClient,
        AuthenticationError_UnsupportedGrantType,
        AuthenticationError_InvalidScope,
        AuthenticationError_Unkown,
    };

    /** ************************************************************************************************
    * @class    AuthenticationError
    *
    * @brief    Authentication Error information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/#response-schema
    ***************************************************************************************************/
    class AuthenticationError
    {
        public:
            AuthenticationError(const QJsonObject &json);

            AuthenticationErrorType getError() const;
            const QString &getErrorDescription() const;

        private:
            AuthenticationErrorType error;
            QString error_description;
    };
}

#endif // AUTHENTICATIONERROR_H
