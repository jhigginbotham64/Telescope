// 
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_mixer.h>

#include <string>

namespace ts
{
    //
    class Sound
    {
        public:
            //
            Sound();

            //
            Sound(const std::string& path);

            //
            ~Sound();

            //
            void set_volume(float zero_to_one);

            //
            float get_volume() const;

            //
            bool load(const std::string& path);

            //
            void unload();

            //
            size_t get_id() const;

            //
            Mix_Chunk* get_native();

        private:
            friend class SoundHandler;

            size_t _id;
            Mix_Chunk* _chunk;
            float _volume = 1;
    };
}