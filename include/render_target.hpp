// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/renderable.hpp>

namespace ts
{
    class Window;

    //
    class RenderTarget
    {
        public:
            //
            RenderTarget();

            //
            ~RenderTarget();

            //
            void create(Window*);

            // invokes renderable.render(*this)
            void render(Renderable* renderable) const;

            //
            SDL_Renderer* get_renderer() const;

        private:
            Window* _window;
            SDL_Renderer* _renderer;
    };
}