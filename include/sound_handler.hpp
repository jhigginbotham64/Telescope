// 
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <mutex>

#include <include/sound.hpp>
#include <include/time.hpp>

namespace ts
{
    class SoundHandler
    {
        public:
            static inline constexpr size_t n_channels = 256;

            //
            static void play(Sound&, size_t channel, size_t n_loops = 0, Time fade_in_duration = milliseconds(0));

            //
            static void stop(Sound&, size_t channel, Time fade_out_duration = milliseconds(0));

            //
            static void pause(size_t channel);

            //
            static void resume(size_t channel);

            //
            static void force_stop(size_t channel);

            //
            static void is_playing(size_t channel);

            //
            static void is_paused(size_t channel);

            //
            static void is_stopped(size_t channel);

            //
            static Sound* get_active(size_t channel);

            //
            static void set_volume(size_t channel, float zero_to_one);

            //
            static void set_panning(size_t channel, size_t zero_to_360_degree);

        private:
            static size_t assert_channel_index(const std::string function_name, size_t);
            static inline std::mutex _lock = std::mutex();
    };
}