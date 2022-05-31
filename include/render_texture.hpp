// 
// Copyright 2022 Clemens Cords
// Created on 5/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/texture.hpp>
#include <include/render_target.hpp>

namespace ts
{
    class RenderTexture : public RenderTarget, public Texture
    {
        public:
            //
            RenderTexture(Window* window);

            //
            void create(size_t width, size_t height);

            //
            void unload();

            //
            using RenderTarget::render;
    };
}