// 
// Copyright 2022 Clemens Cords
// Created on 6/1/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/renderable.hpp>

namespace ts
{
    /// \brief abstract interface of an object that things can be drawn to
    struct RenderTarget
    {
        /// \brief render an object to the render target
        /// \param object: renderable to be rendered
        /// \param transform: affine transform to be applied before rendering. Does not affect the actual object
        virtual void render(const Renderable* object, Transform transform = ts::Transform()) = 0;

        /// \brief get the SDL renderer context
        /// \returns poitner to SDL_Renderer
        virtual SDL_Renderer* get_renderer() = 0;
    };
}