// 
// Copyright 2022 Joshua Higginbotham
// Created on 24.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/sound.hpp>
#include <include/logging.hpp>

namespace ts
{
    Sound::Sound()
        : _id(-1), _chunk(nullptr)
    {}

    Sound::Sound(const std::string& path)
    {
        load(path);
    }

    Sound::~Sound()
    {
        unload();
    }

    bool Sound::load(const std::string& path)
    {
        _chunk = Mix_LoadWAV(path.c_str());
         _id = std::hash<std::string>()(path);

        if (_chunk == nullptr)
        {
            Log::warning("In ts::Sound.load : unable to load \"", path, "\"");
            return false;
        }
        else
            return true;
    }

    void Sound::unload()
    {
        Mix_FreeChunk(_chunk);
        _chunk = nullptr;
        _id = -1;
    }

    size_t Sound::get_id() const
    {
        return _id;
    }

    Mix_Chunk * Sound::get_native()
    {
        return _chunk;
    }

    void Sound::set_volume(float zero_to_one)
    {
        if (zero_to_one > 1.0)
        {
            Log::warning("In ts::Sound.set_volume: volume level ", zero_to_one,
                         " is outside [0, 1]; volume will instead be set to 1 (the maximum).");
            zero_to_one = 1.0;
        }
        else if (zero_to_one < 0)
        {
            Log::warning("In ts::Sound.set_volume: volume level ", zero_to_one,
                         " is outside [0, 1]; volume will instead be set to 0 (the minimum).");
            zero_to_one = 0.0;
        }

        Mix_VolumeChunk(_chunk, int32_t(zero_to_one * 128));
        _volume = zero_to_one;
    }

    float Sound::get_volume() const
    {
        return _volume;
    }
}

