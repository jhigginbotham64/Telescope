// 
// Copyright 2022 Joshua Higginbotham
// Created on 26.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <string>

#include <include/texture.hpp>
#include <include/color.hpp>

namespace ts
{
    class Window;

    /// \brief static, unchanging texture that lives on the graphics card
    class StaticTexture : public Texture
    {
        public:
            /// \brief virtual destructor
            virtual ~StaticTexture() = default;

            /// \brief constructor
            /// \param window: window that supplies the rendering context
            StaticTexture(Window* context);

            /// \brief create texture specified size, with each pixel set to one color
            /// \param width: x-dimension of texture, in pixels
            /// \param height: y-dimension of texture, in pixels
            /// \param color: color
            void create(size_t width, size_t height, RGBA color);

            /// \brief load the texture from a path
            /// \param path: absolute path to image file
            /// \note supported formats include: .bmp, .png, .jpg, .jpeg
            void load(const std::string& path);

            /// \brief free the memory of the texture, this function is automatically called when the texture object calls its destructor
            void unload();

        private:
            using Texture::_texture;
    };
}