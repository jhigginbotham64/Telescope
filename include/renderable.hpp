// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

namespace ts
{
    //
    using RenderTarget = SDL_Renderer;

    /// \brief an object that can be drawn to the screen
    struct Renderable
    {
        public:
            /// \brief queue object for drawing
            /// \param target: render context the object will be drawn to
            virtual void render(const RenderTarget* target) const = 0;
    };
}