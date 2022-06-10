// 
// Copyright 2022 Joshua Higginbotham
// Created on 24.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <mutex>
#include <array>

#include <include/sound.hpp>
#include <include/time.hpp>
#include <include/angle.hpp>

namespace ts
{
    /// \brief class for handling shorter sound bites, supports 256 channels
    class SoundHandler
    {
        public:
            /// \brief maximum number of channels
            static inline constexpr size_t n_channels = 256;

            /// \brief play sample on specified channel
            /// \param channel: channel index, [0, 255]
            /// \param sound: sound to play, user is responsible for the sound staying in memory
            /// \param n_loops: number of times the sample will be repeated, set to 0 if sample should only be played once
            /// \param fade_in_duration: duration of fade-in, set to 0 for no fade-in
            static void play(size_t channel, Sound&, size_t n_loops = 0, Time fade_in_duration = milliseconds(0));

            /// \brief play sample on specified channel
            /// \param sound: sound to play, user is responsible for the sound staying in memory
            /// \param channel: channel index, [0, 255]
            /// \param fade_out_duration: duration of fade-out, set to 0 for no fade-out
            static void stop(size_t channel, Time fade_out_duration = milliseconds(0));

            /// \brief pause a channel
            /// \param channel: channel index, [0, 255]
            static void pause(size_t channel);

            /// \brief unpause channel
            /// \param channel: channel index, [0, 255]
            static void unpause(size_t channel);

            /// \brief forcefully halt a channel
            /// \param channel: channel index, [0, 255]
            static void force_stop(size_t channel);

            /// \brief is channel currently busy playing a sound
            /// \param channel: channel index, [0, 255]
            static bool is_playing(size_t channel);

            /// \brief is channel currently paused or stopped
            /// \param channel: channel index, [0, 255]
            static bool is_paused(size_t channel);

            /// \brief is channel currently stopped but not paused
            /// \param channel: channel index, [0, 255]
            static bool is_stopped(size_t channel);

            /// \brief set volume of a channel
            /// \param channel: channel index, [0, 255]
            /// \param volume: volume in [0, 1], where 0 is silence, 1 is maximum volume
            static void set_volume(size_t channel, float zero_to_one);

            /// \brief set volume of a channel
            /// \param channel: channel index, [0, 255]
            /// \returns volume in [0, 1], where 0 is silence, 1 is maximum volume
            static float get_volume(size_t channel);

            /// \brief set panning of a channel
            /// \param channel: channel index, [0, 255]
            /// \param zero_to_360_degree: angle, clockwise: 0° is no panning, +90° is full right, 180° is no panning, +270° is full left
            static void set_panning(size_t channel, Angle angle);

            /// \brief get panning of channel
            /// \param channel: channel index, [0, 255]
            /// \returns angle in degree, clockwise: 0° is no panning, +90° is full right, 180° is no panning, +270° is full left
            static Angle get_panning(size_t channel);

        private:
            static int32_t forward_index(size_t channel, const std::string function_name);

            static inline std::array<float, n_channels> _volume = {};
            static inline std::array<size_t, n_channels> _panning = {};

            static inline std::mutex _lock = std::mutex();
    };
}