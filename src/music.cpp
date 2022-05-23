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
        _id = std::hash<std::string>()(path);

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
        _id = -1;
        _music = nullptr;
    }

    Music::~Music()
    {
        unload();
    }
}

