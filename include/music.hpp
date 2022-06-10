// 
// Copyright 2022 Joshua Higginbotham
// Created on 23.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <SDL2/SDL_mixer.h>

#include <string>

#include <include/time.hpp>

namespace ts
{
    /// \brief memory management proxy for music, played with MusicHandler
    class Music
    {
        public:
            /// \brief ctor, does not allocate
            Music();

            /// \brief ctor, load music from a file, supports .wav, .mp3, .ogg, .flac
            /// \param path: absolute path
            Music(const std::string& path);

            /// \brief dtor, deallocates memory
            ~Music();

            /// \brief load music from a file, supports .wav, .mp3, .ogg, .flac
            /// \param path: absolute path
            /// \returns true if load successfully, false otherwise
            bool load(const std::string& path);

            /// \brief deallocate memory
            void unload();

            /// \brief get internal id
            /// \returns id as uint64
            size_t get_id() const;

            /// \brief get native SDL2 mixer music object
            /// \returns pointer
            Mix_Music* get_native();

        private:
            friend class MusicHandler;

            size_t _id;
            Mix_Music* _music;
    };
}