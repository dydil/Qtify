#ifndef CURRENTPLAYBACK_H
#define CURRENTPLAYBACK_H

#include <QJsonObject>

#include "Context.h"
#include "Track.h"
#include "Device.h"

namespace Qtify
{
    enum CurrentlyPlayingType
    {
        Type_Track,
        Type_Episode,
        Type_Ad,
        Type_Unknown
    };

    enum RepeatState
    {
        Repeat_Off,
        Repeat_Track,
        Repeat_Context,
        Repeat_Unknown,
    };

    /** ************************************************************************************************
    * @class    CurrentPlayback
    *
    * @brief    Playback information according to Spotify API.
    *
    * @details  More info on
    *           https://developer.spotify.com/documentation/web-api/reference/player/get-the-users-currently-playing-track/
    ***************************************************************************************************/
    class CurrentPlayback
    {
        public:
            CurrentPlayback(const QJsonObject &json);

            const Device &getDevice() const;
            RepeatState getRepeatState() const;
            bool isShuffleEnabled() const;
            const Context &getContext() const;
            int getDataTimeStamp() const;
            int getProgressMilliseconds() const;
            bool isPlaying() const;
            const Track &getTrack() const;
            CurrentlyPlayingType getCurrentlyPlayingType() const;

        private:
            Device device;
            RepeatState repeat_state;
            bool shuffle_state;
            Context context;
            int timestamp;
            int progress_ms;
            bool is_playing;
            Track item;
            CurrentlyPlayingType currently_playing_type;
    };
}

#endif // CURRENTPLAYBACK_H
