// 
// Copyright 2022 Clemens Cords
// Created on 26.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <string>

#include <include/texture.hpp>
#include <include/color.hpp>

namespace ts
{
    class Window;

    //
    class StaticTexture : public Texture
    {
        public:
            //
            StaticTexture(Window* context);

            //
            void create(size_t width, size_t height, RGBA color);

            //
            void load(const std::string& path);

            //
            void unload();

        private:
            using Texture::_texture;
    };
}