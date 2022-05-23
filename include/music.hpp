// 
// Copyright 2022 Clemens Cords
// Created on 23.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_mixer.h>

#include <include/resource_id.hpp>
#include <include/time.hpp>

namespace ts
{
    //
    class Music
    {
        friend class MusicHandler;
        public:
            //
            Music();

            //
            Music(const std::string& path, bool should_loop = true);

            //
            ~Music();

            //
            bool load(const std::string& path, bool should_loop = true);

            // call destroy, useful for memory management
            void unload();

        private:
            size_t _id;
            bool _is_looping = true;
            Mix_Music* _music;
    };
}