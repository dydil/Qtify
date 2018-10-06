#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QScopedPointer>

#include "models/User.h"
#include "models/CurrentPlayback.h"

namespace Qtify
{
    /** ************************************************************************************************
    * @class    RequestHandler
    *
    * @brief    Main class to use to get any Spotify data.
    *           All requests are asynchronous. Use signals to get the result.
    ***************************************************************************************************/
    class RequestHandler : public QObject
    {
        Q_OBJECT

        struct RequestHandlerData;

        public:
            explicit RequestHandler(const QString &clientId, const QString &clientSecret, int replyPort, QObject *parent = nullptr);
            ~RequestHandler();

            void grant();
            void restoreTokens(const QString &accessToken, const QString &refreshToken);

            void getCurrentUserInformation();
            void getCurrentPlayback();
            void resumePlayback();
            void pausePlayback();
            void nextTrack();
            void previousTrack();
            void seek(int positionMs);

        signals:
            void accessGranted(const QString &accessToken, const QString &refreshToken);
            void accessDenied(const QString &error);
            void tokenRefreshed(const QString &refreshToken);
            void userDataAvailable(const User &user);
            void currentPlaybackUpdated(const CurrentPlayback &playback);

        private:
            QScopedPointer<RequestHandlerData> m_data;
    };
}

#endif // REQUESTHANDLER_H
