// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/renderable.hpp>

namespace ts
{
    //
    class RenderTarget
    {
        public:
            // invokes renderable.render(*this)
            virtual void render(Renderable& renderable) const = 0;

            //
            SDL_Renderer* get_renderer() const;

        private:
            SDL_Renderer* _renderer;
    };
}