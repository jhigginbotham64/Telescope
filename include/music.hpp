// 
// Copyright 2022 Clemens Cords
// Created on 23.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_mixer.h>

#include <string>

#include <include/time.hpp>

namespace ts
{
    //
    class Music
    {
        public:
            //
            Music();

            //
            Music(const std::string& path);

            //
            ~Music();

            //
            bool load(const std::string& path);

            // call destroy, useful for memory management
            void unload();

            //
            size_t get_id() const;

            //
            Mix_Music* get_native();

        private:
            friend class MusicHandler;

            size_t _id;
            Mix_Music* _music;
    };
}