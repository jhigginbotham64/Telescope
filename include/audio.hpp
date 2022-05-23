// 
// Copyright 2022 Clemens Cords
// Created on 23.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_mixer.h>

#include <include/resource_id.hpp>

namespace ts
{
    //
    class Music
    {
        public:
            //
            Music(const std::string& path);

            //
            void play(bool fade_in = false);

            //
            void pause();

            //
            void stop(bool fade_out = false);

            //
            bool is_playing() const;

            //
            bool is_paused() const;


        private:
    };
}