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
    /// \brief a texture that things can be renderer to
    class RenderTexture : public Texture, public RenderTarget
    {
        public:
            /// \brief create the render texture
            /// \param window: window to use to supply the rendering context
            RenderTexture(Window* window);

            /// \brief create the render texture, all pixels will have the value RGBA(0, 0, 0, 1)
            /// \param width: width of the texture, in pixels
            /// \param height: height of the texture, in pixels
            void create(size_t width, size_t height);

            /// \brief free the texture from memory, this function is automatically called when the render texture object calls its destructor
            void unload();

            /// \copydoc RenderTarget::render
            void render(const Renderable*, Transform = ts::Transform()) override;

            /// \copydoc RenderTarget::get_renderer
            SDL_Renderer* get_renderer() override;

        private:
            Window* _window;
    };
}