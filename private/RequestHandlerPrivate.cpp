#include "RequestHandlerPrivate.h"

#include <QDesktopServices>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QEventLoop>
#include <QUrlQuery>
#include <QEvent>
#include <QThread>

namespace Qtify
{
    const QString RequestHandlerPrivate::API_URL{"https://api.spotify.com/"};
    const QUrl    RequestHandlerPrivate::AUTHORIZATION_URL{"https://accounts.spotify.com/authorize"};
    const QUrl    RequestHandlerPrivate::TOKEN_ACCESS_URL{"https://accounts.spotify.com/api/token"};

    /// The URL for all the requests in the enum SpotifyApiRequest.
    const QString RequestHandlerPrivate::REQUEST_URLS[]
    {
        "v1/me",
        "v1/me/player",
        "v1/me/player/pause",
        "v1/me/player/play",
        "v1/me/player/next",
        "v1/me/player/previous",
        "v1/me/player/seek",
    };

    /// The scope for all the requests in the enum SpotifyApiRequest.
    const QString RequestHandlerPrivate::REQUEST_SCOPE[]
    {
        "user-read-private",
        "user-read-playback-state",
        "user-modify-playback-state",
    };

    /// String display of error contexts.
    const QString RequestHandlerPrivate::ERROR_CONTEXT_STRINGS[]
    {
        "Unknown",
        "RefreshToken",
        "GetUserInformation",
        "GetCurrentPlayback",
        "PauseCurrentPlayback",
        "ResumeCurrentPlayback",
        "NextTrack",
        "PreviousTrack",
        "Seek",
    };

    /** ************************************************************************************************
    * @brief        Constructor.
    *
    * @details      Initialize the authentication object.
    *
    * @param[in]    clientId: The client ID of the Spotify application.
    * @param[in]    clientSecret: The client secret of the Spotify application.
    * @param[in]    replyPort: The port on which the API will send authorization replies.
    * @param[in]    parent: The QObject parent.
    ***************************************************************************************************/
    RequestHandlerPrivate::RequestHandlerPrivate(const QString &clientId, const QString &clientSecret, int replyPort, QObject *parent) :
        QObject(parent),
        m_clientId(clientId),
        m_clientSecret(clientSecret),
        m_replyPort(replyPort),
        m_tokenRefreshTimer(this)
    {
        // Control tables at compile time.
        static_assert(
               (sizeof(REQUEST_URLS) / sizeof(REQUEST_URLS[0]))
            == static_cast<int>(SpotifyApiRequest::SpotifyRequest_Count),
            "Invalid request table");
        static_assert(
                (sizeof(ERROR_CONTEXT_STRINGS) / sizeof(ERROR_CONTEXT_STRINGS[0]))
            == static_cast<int>(ErrorContext::Context_Count),
            "Invalid error context table");
    }

    /** ************************************************************************************************
    * @brief        Destructor.
    ***************************************************************************************************/
    RequestHandlerPrivate::~RequestHandlerPrivate() = default;

    /** ************************************************************************************************
    * @brief        Initialize the object.
    *
    * @details      This must be called in the thread in which the object lives (using invokeMethod).
    ***************************************************************************************************/
    void RequestHandlerPrivate::init()
    {
        m_networkAccessManager = std::make_unique<QNetworkAccessManager>(this);
        m_authManager = std::make_unique<QOAuth2AuthorizationCodeFlow>(m_networkAccessManager.get(), this);
        m_replyHandler = std::make_unique<QOAuthHttpServerReplyHandler>(m_replyPort, this);

        m_authManager->setReplyHandler(m_replyHandler.get());
        m_authManager->setAuthorizationUrl(AUTHORIZATION_URL);
        m_authManager->setAccessTokenUrl(TOKEN_ACCESS_URL);
        m_authManager->setClientIdentifier(m_clientId);
        m_authManager->setClientIdentifierSharedKey(m_clientSecret);

        // Build the scope list for each available request type.
        QString scope;
        for (const QString &scopeItem: REQUEST_SCOPE)
        {
            if (!scopeItem.isEmpty())
            {
                scope += scopeItem + " ";
            }
        }
        scope.chop(1); // Remove the final space.

        m_authManager->setScope(scope);

        // Open a webpage for login authorization.
        connect(m_authManager.get(), &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
                this,                &QDesktopServices::openUrl);

        // Handle successful access.
        connect(m_authManager.get(), &QOAuth2AuthorizationCodeFlow::granted,
                this,                &RequestHandlerPrivate::onAccessGranted);

        // Refresh token periodically.
        connect(&m_tokenRefreshTimer, &QTimer::timeout, this, &RequestHandlerPrivate::refreshToken);
        m_tokenRefreshTimer.setInterval(std::chrono::minutes(5));
        m_tokenRefreshTimer.setSingleShot(true);

        // When the thread is stopped, stop the timer.
        connect(thread(), &QThread::finished, &m_tokenRefreshTimer, &QTimer::stop);
    }

    /** ************************************************************************************************
    * @brief        Request to grant access to the API for.
    *
    * @details      This will open a webpage for the user to login.
    ***************************************************************************************************/
    void RequestHandlerPrivate::grant()
    {
        m_authManager->grant();
    }

    /** ************************************************************************************************
    * @brief        Restore tokens obtained previously with a call to grant().
    *
    * @details      Once the tokens are registered, they are refreshed in case they have expired.
    ***************************************************************************************************/
    void RequestHandlerPrivate::restoreTokens(const QString &accessToken, const QString &refreshToken)
    {
        m_authManager->setToken(accessToken);
        m_authManager->setRefreshToken(refreshToken);
        this->refreshToken();
    }

    /** ************************************************************************************************
    * @brief        Send a request to read current user information.
    ***************************************************************************************************/
    void RequestHandlerPrivate::getCurrentUserInformation()
    {
        get(buildUrl(SpotifyApiRequest::SpotifyRequest_UserInformation),
            &RequestHandlerPrivate::onGetCurrentUserInformationReplyReceived);
    }

    /** ************************************************************************************************
    * @brief        Send a request to get current track playback.
    ***************************************************************************************************/
    void RequestHandlerPrivate::getCurrentPlaybackInformation()
    {
        get(buildUrl(SpotifyApiRequest::SpotifyRequest_CurrentPlayback),
            &RequestHandlerPrivate::onGetCurrentPlaybackInformationReceived);
    }

    /** ************************************************************************************************
    * @brief        Send a request to resume the current playing track.
    ***************************************************************************************************/
    void RequestHandlerPrivate::resumePlayback()
    {
        put(buildUrl(SpotifyApiRequest::SpotifyRequest_ResumePlayback),
            ErrorContext::Context_ResumeCurrentPlayback);
    }

    /** ************************************************************************************************
    * @brief        Send a request to pause the current playing track.
    ***************************************************************************************************/
    void RequestHandlerPrivate::pausePlayback()
    {
        put(buildUrl(SpotifyApiRequest::SpotifyRequest_PausePlayback),
            ErrorContext::Context_PauseCurrentPlayback);
    }

    /** ************************************************************************************************
    * @brief        Send a request to go to the next track in the current playlist.
    ***************************************************************************************************/
    void RequestHandlerPrivate::nextTrack()
    {
        post(buildUrl(SpotifyApiRequest::SpotifyRequest_NextTrack),
             ErrorContext::Context_NextTrack);
    }

    /** ************************************************************************************************
    * @brief        Send a request to go to the previous track in the current playlist.
    ***************************************************************************************************/
    void RequestHandlerPrivate::previousTrack()
    {
        post(buildUrl(SpotifyApiRequest::SpotifyRequest_PreviousTrack),
             ErrorContext::Context_PreviousTrack);
    }

    /** ************************************************************************************************
    * @brief        Send a request to go to the given position in the current playback.
    *
    * @param[in]    positionMs: The position in milliseconds to seek to. Must be a positive number.
    *               Passing in a position that is greater than the length of the track will cause the
    *               player to start playing the next song.
    ***************************************************************************************************/
    void RequestHandlerPrivate::seek(int positionMs)
    {
        put(buildUrl(SpotifyApiRequest::SpotifyRequest_Seek, QVariantMap{{"position_ms", positionMs}}),
            ErrorContext::Context_Seek);
    }

    /** ************************************************************************************************
    * @brief        Build an URL to sent the given request type.
    *
    * @param[in]    requestType: The type of request to send.
    * @param[in]    parameters: The parameters of the request (optional).
    ***************************************************************************************************/
    QUrl RequestHandlerPrivate::buildUrl(RequestHandlerPrivate::SpotifyApiRequest requestType, const QVariantMap &parameters)
    {
        QUrl url{API_URL + REQUEST_URLS[static_cast<int>(requestType)]};

        if (!parameters.empty())
        {
            QUrlQuery query;

            QMapIterator<QString, QVariant> iterator(parameters);
            while(iterator.hasNext())
            {
                iterator.next();
                query.addQueryItem(iterator.key(), iterator.value().toString());
            }

            url.setQuery(query);
        }

        return url;
    }

    /** ************************************************************************************************
    * @brief        Refresh the token to access the API.
    *
    * @details      QOAuth2AuthorizationCodeFlow::refreshAccessToken() doesn't work with Spotify.
    *               The header "Authorization" value is incorrect. Until a way to override it is found,
    *               a custom request is sent.
    *               The query is the step 4 of the authorization flow documentation.
    *               https://developer.spotify.com/documentation/general/guides/authorization-guide/#authorization-code-flow
    ***************************************************************************************************/
    void RequestHandlerPrivate::refreshToken()
    {
        // Construct the URL.
        QUrl url(m_authManager->accessTokenUrl());

        // Add query parameters
        QUrlQuery query;
        query.addQueryItem("grant_type", "refresh_token");
        query.addQueryItem("refresh_token", m_authManager->refreshToken());
        url.setQuery(query);

        // Create the request.
        QNetworkRequest request(url);
        // The "Authorization" header is  a base64 encoding of the client ID and secret.
        QString authorization("Basic %1");
        QByteArray code;
        code.append(m_authManager->clientIdentifier() + ":" +  m_authManager->clientIdentifierSharedKey());
        request.setRawHeader("Authorization", QByteArray("Basic ") + code.toBase64());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        auto reply = m_networkAccessManager->post(request, QByteArray());
        connect(reply, &QNetworkReply::finished, this, &RequestHandlerPrivate::onRefreshTokenReplyReceived);
    }

    /** ************************************************************************************************
    * @brief        Send a GET request to the given Spotify API URL and  call callback when a reply is
    *               received.
    ***************************************************************************************************/
    void RequestHandlerPrivate::get(const QUrl &url, void (RequestHandlerPrivate::*callback)())
    {
        connect(m_authManager->get(url), &QNetworkReply::finished, this, callback);
    }

    /** ************************************************************************************************
    * @brief        Send a PUT request to the given Spotify API URL and  call callback when a reply is
    *               received.
    ***************************************************************************************************/
    void RequestHandlerPrivate::put(const QUrl &url, ErrorContext context)
    {
        QNetworkReply *reply = m_authManager->put(url);
        connect(reply, &QNetworkReply::finished, this, &RequestHandlerPrivate::onPutPostReplyReceived);
        m_postPutReplyContexts[reply] = context;
    }

    /** ************************************************************************************************
    * @brief        Send a POST request to the given Spotify API URL and  call callback when a reply is
    *               received.
    ***************************************************************************************************/
    void RequestHandlerPrivate::post(const QUrl &url, ErrorContext context)
    {
        QNetworkReply *reply = m_authManager->post(url);
        connect(reply, &QNetworkReply::finished, this, &RequestHandlerPrivate::onPutPostReplyReceived);
        m_postPutReplyContexts[reply] = context;
    }

    /** ************************************************************************************************
    * @brief        Handle any regular network error.
    *
    * @details      There are two types of error: regular and authentication. Each has its own JSON
    *               format. handleRegularError() and handleAuthenticationError() process the JSON
    *               data differently.
    ***************************************************************************************************/
    void RequestHandlerPrivate::handleRegularError(RequestHandlerPrivate::ErrorContext context, const QByteArray &errorData)
    {
        Error error(QJsonDocument::fromJson(errorData).object()["error"].toObject());
        qWarning() << "Error in " << ERROR_CONTEXT_STRINGS[static_cast<int>(context)]
                   << ":" << error.getStatus() << "-" << error.getMessage();

        // If the token has expired try to refresh it.
        if ("The access token expired" == error.getMessage())
        {
            // Do not try more than every 10 seconds.
            if (std::chrono::system_clock::now() - m_lastTokenRefresh >= std::chrono::seconds(10))
            {
                refreshToken();
            }
        }
    }

    /** ************************************************************************************************
    * @brief        Handle any regular network error.
    *
    * @details      There are two types of error: regular and authentication. Each has its own JSON
    *               format. handleRegularError() and handleAuthenticationError() process the JSON
    *               data differently.
    ***************************************************************************************************/
    void RequestHandlerPrivate::handleAuthenticationError(RequestHandlerPrivate::ErrorContext context, const QByteArray &errorData)
    {
        AuthenticationError error(QJsonDocument::fromJson(errorData).object());
        qWarning() << "Error in " << ERROR_CONTEXT_STRINGS[static_cast<int>(context)]
                   << ":" << error.getError() << "-" << error.getErrorDescription();
    }

    /** ************************************************************************************************
    * @brief        Function called when API access has been granted by the user.
    ***************************************************************************************************/
    void RequestHandlerPrivate::onAccessGranted()
    {
        emit accessGranted(m_authManager->token(), m_authManager->refreshToken());
        m_tokenRefreshTimer.start();
    }

    /** ************************************************************************************************
    * @brief        Function called when a reply is received after calling refreshToken().
    ***************************************************************************************************/
    void RequestHandlerPrivate::onRefreshTokenReplyReceived()
    {
        if (QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender()))
        {
            if (reply->error() != QNetworkReply::NoError)
            {
                handleAuthenticationError(ErrorContext::Context_RefreshTokenReply, reply->readAll());
            }
            else
            {
                m_authManager->setToken(QJsonDocument::fromJson(reply->readAll()).object()["access_token"].toString());
                emit tokenRefreshed(m_authManager->refreshToken());

                // Refresh the token later.
                m_tokenRefreshTimer.start();
            }

            reply->deleteLater();
        }

        m_lastTokenRefresh = std::chrono::system_clock::now();
    }

    /** ************************************************************************************************
    * @brief        Function called when a reply to any POST or PUT request is received.
    *
    * @details      The response is ignored, we just process any error and delete the reply.
    ***************************************************************************************************/
    void RequestHandlerPrivate::onPutPostReplyReceived()
    {
        if (QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender()))
        {
            auto iterator = m_postPutReplyContexts.find(reply);

            if (reply->error() != QNetworkReply::NoError)
            {
                auto context = ErrorContext::Context_Unknown;
                if (iterator != m_postPutReplyContexts.end())
                {
                    context = iterator.value();
                }

                handleRegularError(m_postPutReplyContexts.value(reply, context), reply->readAll());
            }

            reply->deleteLater();
            m_postPutReplyContexts.erase(iterator);
        }
    }

    /** ************************************************************************************************
    * @brief        Function called when a reply to getCurrentUserInformation is received.
    ***************************************************************************************************/
    void RequestHandlerPrivate::onGetCurrentUserInformationReplyReceived()
    {
        if (QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender()))
        {
            if (reply->error() != QNetworkReply::NoError)
            {
                handleRegularError(ErrorContext::Context_GetUserInformationReply, reply->readAll());
            }
            else
            {
                emit userDataAvailable(QSharedPointer<User>(new User(QJsonDocument::fromJson(reply->readAll()).object())));
            }

            reply->deleteLater();
        }
    }

    /** ************************************************************************************************
    * @brief        Function called when a reply to getCurrentPlaybackInformation is received.
    ***************************************************************************************************/
    void RequestHandlerPrivate::onGetCurrentPlaybackInformationReceived()
    {
        if (QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender()))
        {
            if (reply->error() != QNetworkReply::NoError)
            {
                handleRegularError(ErrorContext::Context_GetCurrentPlaybackReply, reply->readAll());
            }
            else
            {
                emit currentPlaybackUpdated(QSharedPointer<CurrentPlayback>(new CurrentPlayback(QJsonDocument::fromJson(reply->readAll()).object())));
            }

            reply->deleteLater();
        }
    }
}
