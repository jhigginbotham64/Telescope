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

    /*
    /// \brief an object that any Renderable can be drawn to
    class RenderTarget
    {
        public:
            /// \brief default construct
            RenderTarget();

            /// \brief safely deallocate
            /// \note any texture that constructed using the renderer will trigger undefined behavior if accessed after its renderer is destroyed
            ~RenderTarget();

            /// \brief create and instance from a window
            /// \param window
            void create(Window*);

            /// \brief queue a Renderable for drawing, invokes renderable.render(*this)
            /// \param renderable
            void render(Renderable* renderable) const;

            /// \brief expose the native SDL renderer
            /// \returns pointer to renderer
            SDL_Renderer* get_renderer() const;

        private:
            Window* _window;
            SDL_Renderer* _renderer;
    };
     */
}