#include "CurrentPlayback.h"

#include <QMap>

namespace Qtify
{
    // To convert a string to a CurrentlyPlayingType.
    const QMap<QString, CurrentlyPlayingType> PLAYING_TYPE_STRINGS
    {
        {"track",   Type_Track   },
        {"episode", Type_Episode },
        {"ad",      Type_Ad      },
        {"unknown", Type_Unknown }
    };

    // To convert a string to a RepeatState.
    const QMap<QString, RepeatState> REPEAT_TYPE_STRINGS
    {
        {"off",     Repeat_Off     },
        {"track",   Repeat_Track   },
        {"context", Repeat_Context }
    };

    CurrentPlayback::CurrentPlayback(const QJsonObject &json):
        device(json["device"].toObject()),
        repeat_state(REPEAT_TYPE_STRINGS.value(json["repeat_state"].toString(), Repeat_Unknown)),
        shuffle_state(json["shuffle_state"].toBool(false)),
        context(json["context"].toObject()),
        timestamp(json["timestamp"].toInt(0)),
        progress_ms(json["progress_ms"].toInt(0)),
        is_playing(json["is_playing"].toBool(false)),
        item(json["item"].toObject()),
        currently_playing_type(PLAYING_TYPE_STRINGS.value(json["currently_playing_type"].toString(), Type_Unknown))
    {

    }

    const Device &CurrentPlayback::getDevice() const
    {
        return device;
    }

    RepeatState CurrentPlayback::getRepeatState() const
    {
        return repeat_state;
    }

    bool CurrentPlayback::isShuffleEnabled() const
    {
        return shuffle_state;
    }

    const Context &CurrentPlayback::getContext() const
    {
        return context;
    }

    int CurrentPlayback::getDataTimeStamp() const
    {
        return timestamp;
    }

    int CurrentPlayback::getProgressMilliseconds() const
    {
        return progress_ms;
    }

    bool CurrentPlayback::isPlaying() const
    {
        return is_playing;
    }

    const Track &CurrentPlayback::getTrack() const
    {
        return item;
    }

    CurrentlyPlayingType CurrentPlayback::getCurrentlyPlayingType() const
    {
        return currently_playing_type;
    }
}
