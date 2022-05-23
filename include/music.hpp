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

            //
            bool is_looping() const;

            // always restarts
            void play(Time fade_in_delay = milliseconds(0)) const;

            //
            void stop(Time fade_out_delay = milliseconds(0)) const;

            // set position
            void skip_to(Time timestamp) const;

            // stop and skip all effects;
            void force_stop() const;

            //
            void pause() const;

            //
            void unpause() const;

            //
            bool is_playing() const;

            //
            bool is_paused() const;


        private:
            bool _is_looping = true;
            Mix_Music* _music;
    };
}