// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/transform.hpp>

namespace ts
{
    class RenderTarget;
    class Renderable;

    namespace detail { void forward_render(RenderTarget*, const Renderable*, Transform); }

    /// \brief an object that can be drawn to the screen
    class Renderable
    {
        friend void detail::forward_render(RenderTarget*, const Renderable*, Transform);

        protected:
            /// \brief queue object for drawing, this function is called by RenderTarget, not by the user
            /// \param target: render context the object will be drawn to
            /// \param transform: affine transform applied to all vertex positions before drawing
            virtual void render(RenderTarget* target, Transform transform) const = 0;
    };
}