// 
// Copyright 2022 Joshua Higginbotham
// Created on 23.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/music.hpp>
#include <include/logging.hpp>
#include <SDL2/SDL_mixer.h>

namespace ts
{
    Music::Music()
        : _music(nullptr)
    {}

    Music::Music(const std::string& path)
    {
        load(path);
    }

    bool Music::load(const std::string& path)
    {
        _music = Mix_LoadMUS(path.c_str());
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

    size_t Music::get_id() const
    {
        return _id;
    }

    Mix_Music* Music::get_native()
    {
        return _music;
    }
}

