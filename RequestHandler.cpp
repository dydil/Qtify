#include "RequestHandler.h"

#include <QThread>
#include <QtGlobal>

#include "private/RequestHandlerPrivate.h"

Q_DECLARE_METATYPE(QSharedPointer<Qtify::User>);
Q_DECLARE_METATYPE(QSharedPointer<Qtify::CurrentPlayback>);

namespace Qtify
{
    /** ************************************************************************************************
    *   The core of the API works on a separate thread.
    *   This prevent blocking calls (can happen with QNetworkManager) from blocking the whole application.
    *   To work around the threaded worker, all calls go through signals and slots.
    *   Calls to the worker are done using QMetaObject::invokeMethod. When a call needs parameters,
    * invokeMethod uses a lambda with the worker object as context so that the lambda is executed in the
    * worker thread.
    *   Queued signal parameters require to be declared with Q_DECLARE_METATYPE, which requires the type
    * to be default constructible. We don't want the API types to be default constructible so a
    * QSharedPointer is used to communicate with the worker object. The pointer is dereferenced in order
    * to provide a non-pointer object to the user of the API.
    ***************************************************************************************************/

    struct RequestHandler::RequestHandlerData
    {
        // The object actually doing all the work.
        RequestHandlerPrivate requestHandlerImpl;
        // The thread in which the worker is ran.
        QThread               requestHandlerThread;

        RequestHandlerData(const QString &clientId, const QString &clientSecret, int replyPort):
            requestHandlerImpl(clientId, clientSecret, replyPort)
        {

        }
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
    RequestHandler::RequestHandler(const QString &clientId, const QString &clientSecret, int replyPort, QObject *parent) :
        QObject(parent),
        m_data(new RequestHandlerData(clientId, clientSecret, replyPort))
    {
        qRegisterMetaType<QSharedPointer<User>>("QSharedPointer<User>");
        qRegisterMetaType<QSharedPointer<CurrentPlayback>>("QSharedPointer<CurrentPlayback>");

        connect(&m_data->requestHandlerImpl, &RequestHandlerPrivate::accessGranted,
                this,                        &RequestHandler::accessGranted);
        connect(&m_data->requestHandlerImpl, &RequestHandlerPrivate::tokenRefreshed,
                this,                        &RequestHandler::tokenRefreshed);
        connect(&m_data->requestHandlerImpl, &RequestHandlerPrivate::userDataAvailable, this,
                [this](const QSharedPointer<User> &user)
                {
                    emit userDataAvailable(*user);
                });
        connect(&m_data->requestHandlerImpl, &RequestHandlerPrivate::currentPlaybackUpdated, this,
                [this](const QSharedPointer<CurrentPlayback> &playback)
                {
                    emit currentPlaybackUpdated(*playback);
                });

        // Move the worker to a separate thread and initialize it from this thread.
        m_data->requestHandlerImpl.moveToThread(&m_data->requestHandlerThread);
        m_data->requestHandlerThread.start();
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::init);
    }

    /** ************************************************************************************************
    * @brief        Request to grant access to the API for.
    *
    * @details      This will open a webpage for the user to login.
    ***************************************************************************************************/
    void RequestHandler::grant()
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::grant);
    }

    /** ************************************************************************************************
    * @brief        Request to grant access to the API with an existing token.
    *
    * @param[in]    token: A token previously obtained with a call to grant().
    ***************************************************************************************************/
    void RequestHandler::restoreTokens(const QString &accessToken, const QString &refreshToken)
    {

        QMetaObject::invokeMethod(&m_data->requestHandlerImpl,
                                  std::bind(&RequestHandlerPrivate::restoreTokens,
                                            &m_data->requestHandlerImpl, accessToken, refreshToken)
                                  );
    }

    /** ************************************************************************************************
    * @brief        Get current user information.
    ***************************************************************************************************/
    void RequestHandler::getCurrentUserInformation()
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::getCurrentUserInformation);
    }

    /** ************************************************************************************************
    * @brief        Get information on the current playback.
    ***************************************************************************************************/
    void RequestHandler::getCurrentPlayback()
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::getCurrentPlaybackInformation);
    }

    /** ************************************************************************************************
    * @brief        Resume the current playing track.
    ***************************************************************************************************/
    void RequestHandler::resumePlayback()
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::resumePlayback);
    }

    /** ************************************************************************************************
    * @brief        Pause the current playing track.
    ***************************************************************************************************/
    void RequestHandler::pausePlayback()
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::pausePlayback);
    }

    /** ************************************************************************************************
    * @brief        Go to the next track in the current playlist.
    ***************************************************************************************************/
    void RequestHandler::nextTrack()
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::nextTrack);
    }

    /** ************************************************************************************************
    * @brief        Go to the previous track in the current playlist.
    ***************************************************************************************************/
    void RequestHandler::previousTrack()
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl, &RequestHandlerPrivate::previousTrack);
    }

    /** ************************************************************************************************
    * @brief        Go to the given position in the current playback.
    *
    * @param[in]    positionMs: The position in milliseconds to seek to. Must be a positive number.
    *               Passing in a position that is greater than the length of the track will cause the
    *               player to start playing the next song.
    ***************************************************************************************************/
    void RequestHandler::seek(int positionMs)
    {
        QMetaObject::invokeMethod(&m_data->requestHandlerImpl,
                                  std::bind(&RequestHandlerPrivate::seek, &m_data->requestHandlerImpl, positionMs));
    }

    /** ************************************************************************************************
    * @brief        Destructor.
    ***************************************************************************************************/
    RequestHandler::~RequestHandler()
    {
        m_data->requestHandlerThread.quit();
        m_data->requestHandlerThread.wait();
    }
}
