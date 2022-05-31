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

    class Renderable;
    namespace detail { void forward_render(const Renderable *, RenderTarget *); }

    /// \brief an object that can be drawn to the screen
    class Renderable
    {
        friend void detail::forward_render(const Renderable *, RenderTarget *);

        protected:
            /// \brief queue object for drawing, this function should not be called by the user
            /// \param target: render context the object will be drawn to
            virtual void render(const RenderTarget* target) const = 0;
    };
}