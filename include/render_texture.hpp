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
    class RenderTexture : public Texture, public RenderTarget
    {
        public:
            //
            RenderTexture(Window* window);

            //
            void create(size_t width, size_t height);

            //
            void unload();

            //
            void render(const Renderable*, Transform = ts::Transform()) override;

            //
            SDL_Renderer* get_renderer() override;

        private:
            Window* _window;
    };
}