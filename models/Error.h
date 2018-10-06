#ifndef ERROR_H
#define ERROR_H

#include <QJsonObject>

namespace Qtify
{
    /** ************************************************************************************************
    * @class    Error
    *
    * @brief    Error information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/object-model/#error-object
    ***************************************************************************************************/
    class Error
    {
        public:
            Error(const QJsonObject &json);

            int getStatus() const;
            const QString &getMessage() const;

        private:
            int status;
            QString message;
    };
}

#endif // ERROR_H
