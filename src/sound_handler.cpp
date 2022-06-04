// 
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#include <include/sound_handler.hpp>
#include <include/logging.hpp>
#include <include/music_handler.hpp>

namespace ts
{
    int32_t SoundHandler::forward_index(size_t channel, const std::string function_name)
    {
        if (channel < n_channels)
            return channel;
        else
        {
            Log::warning("In ts::SoundHandler.", function_name, ": channel index ", channel, " is not in [0, ", n_channels-1, "], instead using any channel #", n_channels-1);
            return n_channels - 1;
        }
    }

    void SoundHandler::play(size_t channel, Sound& sound,  size_t n_loops, Time fade_in_duration)
    {
        channel = forward_index(channel, "play");

        if (fade_in_duration.as_milliseconds() > MusicHandler::sample_rate / 1000)
            Mix_FadeInChannel(channel, sound._chunk, n_loops, fade_in_duration.as_milliseconds());
        else
            Mix_PlayChannel(channel, sound._chunk, n_loops);
    }

    void SoundHandler::stop(size_t channel, Time fade_out_duration)
    {
        auto guard = std::lock_guard(_lock);
        channel = forward_index(channel, "stop");

        if (fade_out_duration.as_milliseconds() > MusicHandler::sample_rate / 1000)
            Mix_FadeOutChannel(channel, fade_out_duration.as_milliseconds());
        else
            Mix_HaltChannel(channel);
    }

    void SoundHandler::pause(size_t channel)
    {
        auto guard = std::lock_guard(_lock);
        Mix_Pause(forward_index(channel, "pause"));
    }

    void SoundHandler::unpause(size_t channel)
    {
        auto guard = std::lock_guard(_lock);
        Mix_Resume(forward_index(channel, "unpause"));
    }

    void SoundHandler::force_stop(size_t channel)
    {
        auto guard = std::lock_guard(_lock);
        Mix_ExpireChannel(forward_index(channel, "force_stop"), 0);
    }

    bool SoundHandler::is_playing(size_t channel)
    {
        auto guard = std::lock_guard(_lock);
        channel = forward_index(channel, "is_playing");
        return not Mix_Paused(channel) and not Mix_Playing(channel);
    }

    bool SoundHandler::is_paused(size_t channel)
    {
        return Mix_Paused(forward_index(channel, "is_paused"));
    }

    bool SoundHandler::is_stopped(size_t channel)
    {
        return not Mix_Playing(forward_index(channel, "is_stopped"));
    }

    void SoundHandler::set_volume(size_t channel, float zero_to_one)
    {
        channel = forward_index(channel, "set_volume");
        auto guard = std::lock_guard(_lock);
        if (zero_to_one > 1.0)
        {
            Log::warning("In ts::SoundHandler.set_volume: volume level ", zero_to_one,
                         " is outside [0, 1]; volume will instead be set to 1 (the maximum).");
            zero_to_one = 1.0;
        }
        else if (zero_to_one < 0)
        {
            Log::warning("In ts::SoundHandler.set_volume: volume level ", zero_to_one,
                         " is outside [0, 1]; volume will instead be set to 0 (the minimum).");
            zero_to_one = 0.0;
        }

        Mix_Volume(channel, zero_to_one * 128);
        _volume[channel] = zero_to_one;
    }

    float SoundHandler::get_volume(size_t channel)
    {
        return _volume[forward_index(channel, "get_volume")];
    }

    void SoundHandler::set_panning(size_t channel, Angle angle)
    {
        auto guard = std::lock_guard(_lock);

        channel = forward_index(channel, "set_panning");
        _panning[channel] = angle.as_degrees();

        Mix_SetPosition(channel, angle.as_degrees(), 0);
    }

    size_t SoundHandler::get_panning(size_t channel)
    {
        return _volume[forward_index(channel, "get_panning")];
    }
}

