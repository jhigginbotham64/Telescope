// 
// Copyright 2022 Clemens Cords
// Created on 23.05.22 by clem (mail@clemens-cords.com)
//

#include <include/music.hpp>
#include <include/logging.hpp>
#include <SDL2/SDL_mixer.h>

namespace ts
{
    Music::Music(const std::string& path, bool should_loop)
    {
        load(path, should_loop);
    }

    bool Music::load(const std::string& path, bool should_loop)
    {
        _music = Mix_LoadMUS(path.c_str());
        _is_looping = should_loop;

        if (_music == nullptr)
        {
            Log::warning("In Music.load : unable to load \"", path, "\"");
            return false;
        }
        else
            return true;
    }

    void Music::unload()
    {
        Mix_FreeMusic(_music);
        _music = nullptr;
    }

    Music::~Music()
    {
        unload();
    }

    bool Music::is_looping() const
    {
        return _is_looping;
    }

    void Music::play(Time fade_in_delay) const
    {
        if (fade_in_delay.as_milliseconds() < 1)
            Mix_PlayMusic(_music, (_is_looping ? -1 : 1));
        else
            Mix_FadeInMusic(_music, (_is_looping ? -1 : 1), fade_in_delay.as_milliseconds());
    }

    void Music::stop(Time fade_out_delay) const
    {
        Mix_FadeOutMusic(ceil(fade_out_delay.as_milliseconds()));
        // ceil prevents popping by always doing a minimal fade-out
    }

    void Music::pause() const
    {
        if (Mix_PausedMusic())
        {
            Log::warning("In Music.pause: music is already paused");
            return;
        }

        Mix_PauseMusic();
    }

    void Music::force_stop() const
    {
        Mix_HaltMusic();
    }

    void Music::unpause() const
    {
        if (not Mix_PausedMusic())
        {
            Log::warning("In Music.unpause: music is not paused");
            return;
        }

        Mix_ResumeMusic();
    }

    bool Music::is_paused() const
    {
        return Mix_PausedMusic();
    }

    bool Music::is_playing() const
    {
        return Mix_PlayingMusic();
    }

    void Music::skip_to(Time timestamp) const
    {
        Mix_SetMusicPosition(timestamp.as_seconds());
    }
}

