// 
// Copyright 2022 Joshua Higginbotham
// Created on 23.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/music_handler.hpp>
#include <include/logging.hpp>

namespace ts
{
    void MusicHandler::play(Music& music, bool should_loop, Time fade_in_delay)
    {
        auto guard = std::lock_guard(_lock);

        if (music._music == nullptr)
        {
            Log::warning("In MusicHandler.play: trying to play music even though it is uninitialized");
            return;
        }

        if (fade_in_delay.as_milliseconds() < sample_rate / 1000.f)
            Mix_PlayMusic(music._music, (should_loop ? -1 : 1));
        else
            Mix_FadeInMusic(music._music, (should_loop ? -1 : 1), (int32_t) fade_in_delay.as_milliseconds());

        _active = &music;
    }

    void MusicHandler::stop(Time fade_out_delay)
    {
        auto guard = std::lock_guard(_lock);

        if (fade_out_delay.as_milliseconds() < sample_rate / 1000)
            Mix_HaltMusic();
        else
            Mix_FadeOutMusic(fade_out_delay.as_milliseconds());

        _active = nullptr;
    }

    void MusicHandler::pause()
    {
        auto guard = std::lock_guard(_lock);

        Mix_PauseMusic();
        // keeps _active set
    }

    void MusicHandler::play_next(Music& music, bool should_loop, Time fade_in)
    {
        auto guard = std::lock_guard(_lock);

        detail::MusicFunctionHook::function = [music_ptr = &music, should_loop, fade_in]() -> void
        {
            MusicHandler::_next = nullptr;
            MusicHandler::play((*music_ptr), should_loop, fade_in);
        };
        Mix_HookMusicFinished(&detail::MusicFunctionHook::invoke_once);
        _next = &music;
    }

    void MusicHandler::clear_next()
    {
        Mix_HookMusicFinished(nullptr);
        detail::MusicFunctionHook::clear();
        _next = nullptr;
    }

    void MusicHandler::next(Time fade_out_duration)
    {
        Mix_HookMusicFinished(nullptr);
        detail::MusicFunctionHook::invoke_once();
    }

    Music * MusicHandler::get_next()
    {
        return _next;
    }

    void MusicHandler::force_stop()
    {
        auto guard = std::lock_guard(_lock);
        clear_next();
        Mix_HaltMusic();
        _active = nullptr;
        _next = nullptr;
    }

    void MusicHandler::unpause()
    {
        auto guard = std::lock_guard(_lock);
        Mix_ResumeMusic();
    }

    bool MusicHandler::is_paused()
    {
        return Mix_PausedMusic();
    }

    bool MusicHandler::is_stopped()
    {
        return not Mix_PlayingMusic();
    }

    bool MusicHandler::is_playing()
    {
        return not Mix_PausedMusic() and Mix_PlayingMusic();
    }

    Music * MusicHandler::get_active()
    {
        auto guard = std::lock_guard(_lock);
        return _active;
    }

    void MusicHandler::skip_to(Time timestamp)
    {
        auto guard = std::lock_guard(_lock);

        if (timestamp.as_seconds() == 0)
            Mix_RewindMusic();
        else
            Mix_SetMusicPosition(timestamp.as_seconds());
    }

    void MusicHandler::set_volume(double zero_to_one)
    {
        auto guard = std::lock_guard(_lock);
        if (zero_to_one > 1.0)
        {
            Log::warning("In MusicHandler.set_volume: volume level ", zero_to_one,
                         " is outside [0, 1]; volume will instead be set to 1 (the maximum).");
            zero_to_one = 1.0;
        }
        else if (zero_to_one < 0)
        {
            Log::warning("In MusicHandler.set_volume: volume level ", zero_to_one,
                         " is outside [0, 1]; volume will instead be set to 0 (the minimum).");
            zero_to_one = 0.0;
        }

        Mix_VolumeMusic(uint8_t(zero_to_one * 128));
    }

    double MusicHandler::get_volume()
    {
        return _volume;
    }
}