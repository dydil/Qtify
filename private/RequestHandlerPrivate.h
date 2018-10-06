#ifndef REQUESTHANDLERPRIVATE_H
#define REQUESTHANDLERPRIVATE_H

#include <chrono>
#include <memory>

#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QNetworkAccessManager>
#include <QSharedPointer>
#include <QTimer>
#include <QMap>

#include "models/User.h"
#include "models/CurrentPlayback.h"
#include "models/Error.h"
#include "models/AuthenticationError.h"

namespace Qtify
{
    /** ************************************************************************************************
    * @class    RequestHandlerPrivate
    *
    * @brief    Implementation of the RequestHandler class.
    ***************************************************************************************************/
    class RequestHandlerPrivate : public QObject
    {
        Q_OBJECT

        /** ************************************************************************************************
        * @enum     SpotifyApiRequest
        *
        * @brief    List of requests the API can handle.
        ***************************************************************************************************/
        enum class SpotifyApiRequest
        {
            SpotifyRequest_UserInformation, /// Data of the user currently logged in.
            SpotifyRequest_CurrentPlayback, /// Current Spotify client playback.
            SpotifyRequest_PausePlayback,   /// Pause current Spotify client playback.
            SpotifyRequest_ResumePlayback,  /// Resume current Spotify client playback.
            SpotifyRequest_NextTrack,       /// Go to the next track.
            SpotifyRequest_PreviousTrack,   /// Go to the previous track.
            SpotifyRequest_Seek,            /// Go to a position in the current track.

            SpotifyRequest_Count /// Number of available request types.
        };

        /** ************************************************************************************************
        * @enum     ErrorContext
        *
        * @brief    Lists of possible actions that triggered an error.
        ***************************************************************************************************/
        enum class ErrorContext
        {
            Context_Unknown,
            Context_RefreshTokenReply,
            Context_GetUserInformationReply,
            Context_GetCurrentPlaybackReply,
            Context_PauseCurrentPlayback,
            Context_ResumeCurrentPlayback,
            Context_NextTrack,
            Context_PreviousTrack,
            Context_Seek,

            Context_Count, // Number of available contexts.
        };

        static const QString API_URL;
        static const QUrl    AUTHORIZATION_URL;
        static const QUrl    TOKEN_ACCESS_URL;
        static const QString REQUEST_URLS[];
        static const QString REQUEST_SCOPE[];
        static const QString ERROR_CONTEXT_STRINGS[];

        public:
            explicit RequestHandlerPrivate(const QString &clientId, const QString &clientSecret, int replyPort, QObject *parent = nullptr);
            ~RequestHandlerPrivate();

            void init();
            void grant();
            void restoreTokens(const QString &accessToken, const QString &refreshToken);

            // Data access functions
            void getCurrentUserInformation();
            void getCurrentPlaybackInformation();

            // Interaction
            void resumePlayback();
            void pausePlayback();
            void nextTrack();
            void previousTrack();
            void seek(int positionMs);

        signals:
            void accessGranted(const QString &accessToken, const QString &refreshToken);
            void accessDenied(const QString &error);
            void tokenRefreshed(const QString &refreshToken);
            void userDataAvailable(const QSharedPointer<User> &user);
            void currentPlaybackUpdated(const QSharedPointer<CurrentPlayback> &playback);

        private:
            // Utility functions
            QUrl buildUrl(SpotifyApiRequest requestType, const QVariantMap &parameters = QVariantMap{});
            void refreshToken();
            void get(const QUrl &url, void (RequestHandlerPrivate::*callback)());
            void put(const QUrl &url, ErrorContext context);
            void post(const QUrl &url, ErrorContext context);
            void handleRegularError(ErrorContext context, const QByteArray &errorData);
            void handleAuthenticationError(ErrorContext context, const QByteArray &errorData);
            // Internal callbacks.
            void onAccessGranted();
            void onRefreshTokenReplyReceived();
            void onPutPostReplyReceived();
            void onGetCurrentUserInformationReplyReceived();
            void onGetCurrentPlaybackInformationReceived();

            std::unique_ptr<QNetworkAccessManager> m_networkAccessManager;
            std::unique_ptr<QOAuth2AuthorizationCodeFlow> m_authManager;
            std::unique_ptr<QOAuthHttpServerReplyHandler> m_replyHandler;
            QString m_clientId;
            QString m_clientSecret;
            int m_replyPort;
            QTimer m_tokenRefreshTimer;
            // Last time a refresh token reply was received (successful or not).
            std::chrono::time_point<std::chrono::system_clock> m_lastTokenRefresh;
            // Mapping of a QNetworkReply to an error context to know what context the POST or PUT
            // reply refers to.
            QMap<QNetworkReply*, ErrorContext> m_postPutReplyContexts;
    };
}
#endif // REQUESTHANDLERPRIVATE_H
