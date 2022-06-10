// 
// Copyright 2022 Joshua Higginbotham
// Created on 24.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <SDL2/SDL_mixer.h>

#include <string>

namespace ts
{
    /// \brief class representing a small sound bite
    class Sound
    {
        public:
            /// \brief ctor, does no allocation
            Sound();

            /// \brief ctor, then load from path, supports .wav, .mp3, .ogg, .flac
            /// \param path: absolute path
            Sound(const std::string& path);

            /// \brief dtor, safely deallocates memory
            ~Sound();

            /// \brief multiply sound bite intensity, this is cumulative with the volume of the channel the sound bite will be played on
            /// \param volume, in [0, 1] where 0 is silence, 1 is the original volume
            void set_volume(float zero_to_one);

            /// \brief get volume
            /// \returns float in [0, 1]
            float get_volume() const;

            /// \brief load from path
            /// \param path: absolute path
            bool load(const std::string& path);

            /// \brief safely deallocate memory
            void unload();

            /// \brief get internal id
            /// \returns uint64
            size_t get_id() const;

            /// \brief get native SDL object
            /// \returns pointer
            Mix_Chunk* get_native();

        private:
            friend class SoundHandler;

            size_t _id;
            Mix_Chunk* _chunk;
            float _volume = 1;
    };
}