// 
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#include <include/sound_handler.hpp>
#include <include/logging.hpp>
#include <include/music.hpp>

namespace ts
{
    size_t SoundHandler::assert_channel_index(const std::string& function_name, size_t i)
    {
        if (i >= n_channels)
        {
            Log::warning("In SoundHandler.", function_name, ": channel index ", i, " is not in [0, ", n_channels, "], instead using channel #", n_channels, " (the maximum index)");
            return n_channels;
        }
        else
            return i;
    }

    void SoundHandler::play(Sound& sound, size_t channel, size_t n_loops, Time fade_in_duration)
    {
        auto guard = std::lock_guard(_lock);
        channel = assert_channel_index("play", channel);

        if (fade_in_duration > Music::sample_rate / 1000)
            Mix_FadeInChannel(channel, sound._chunk, n_loops, fade_in_duration.as_milliseconds());
        else
            Mix_PlayChannel(channel, sound._chunk, n_loops);
    }

    
}

