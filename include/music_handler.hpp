// 
// Copyright 2022 Joshua Higginbotham
// Created on 23.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <mutex>

#include <include/music.hpp>

namespace ts
{
    namespace detail { union MusicFunctionHook; }

    /// \brief manage music playback. Unlike sounds, only one music track can be active at the same time
    class MusicHandler
    {
        public:
            static inline constexpr size_t sample_rate = 44100; // in Hz

            /// \brief instantly play new music, clears any scheduled music
            /// \param music: music to be played, user is responsible for the music staying in memory
            /// \param should_loop: should the music infinitely repeat until stopped
            /// \param fade_in_duration: duration of fade-in, set to 0 for no fade-in
            static void play(Music&, bool should_loop = false, Time fade_in_duration = milliseconds(0));
    
            /// \brief queue music to be played after the current music is finished, or next() was called
            /// \param music: music to be played, user is responsible for the music staying in memory
            /// \param should_loop: should the next music infinitely repeat until stopped
            /// \param fade_in: duration of fade-in for next music, set to 0 for no fade-in
            static void play_next(Music&, bool should_loop = false, Time fade_in = milliseconds(0));

            /// \brief stop any music, clear any scheduled music
            /// \param fade_out_duration: duration of fade-out, set to 0 for no fade-out
            static void stop(Time fade_out_duration = milliseconds(0));

            /// \brief stop the current track, then play the track scheduled using play_next, if any
            /// \param fade_out_duration: duration of fade-out of the current track, set to 0 for no fade-out
            static void next(Time fade_out_duration = milliseconds(0));

            /// \brief undo the last scheduling done by play_next
            static void clear_next();

            /// \brief get scheduled music or nullptr
            /// \returns pointer to music, or nullptr if no music is scheduled
            static Music* get_next();

            /// \brief stop any music and effects
            static void force_stop();
    
            /// \brief pause the current track, when resumed using unpause, will pick up at the same position it was paused at. If no track is playing, does nothing
            static void pause();
    
            /// \brief unpause the track, does nothing if track is not currently paused
            static void unpause();

            /// \brief jump to a position in the current music track playing
            /// \param timestamp: time to jump to, where 0 is the beginning of the track
            static void skip_to(Time timestamp);
    
            /// \brief is the current track neither stopped nor paused
            static bool is_playing();
    
            /// \brief was the current track paused via pause()
            static bool is_paused();

            /// \brief has the current track ended
            static bool is_stopped();

            /// \brief get the current track, even if it is paused
            /// \returns nullptr if no track is active, pointer to track otherwise
            static Music * get_active();

            /// \brief set them volume of the mixer state
            /// \param zero_to_one: volume level, in [0, 1], where 0 is no sound, 1 is maximum volume
            static void set_volume(double zero_to_one);

            /// \brief get the current volume level
            /// \returns double in [0, 1], where 0 is no sound, 1 is maximum volume
            static double get_volume();

        private:
            friend union detail::MusicFunctionHook;

            static inline double _volume = 1;
            static inline Music* _active = nullptr;
            static inline Music* _next = nullptr;

            static inline std::mutex _lock = std::mutex();
    };
}

#include <src/music.inl>