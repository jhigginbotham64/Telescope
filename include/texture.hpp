// 
// Copyright 2022 Joshua Higginbotham
// Created on 5/31/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/color.hpp>

namespace ts
{
    /// \brief target texture pixel format: RGBA, 32-bit floating point
    static inline constexpr SDL_PixelFormatEnum PIXEL_FORMAT = SDL_PIXELFORMAT_RGBA32;

    /// \brief texture filtering mode, governs how the texture will behave when scaled
    enum TextureFilteringMode
    {
        /// \brief nearest neighbour scaling
        NEAREST_NEIGHBOUR = SDL_ScaleModeNearest,

        /// \brief linear interpolation
        LINEAR = SDL_ScaleModeLinear,

        /// \brief non-linear interpolation and anti-aliasing
        ANISOTROPIC = SDL_ScaleModeBest
    };

    /// \brief texture blend mode, governs how texture behave when rendered on top of an already rendered area
    enum TextureBlendMode
    {
        /// \brief no blending, only the latest texture drawn will show up
        NONE = SDL_BLENDMODE_NONE,

        /// \brief blend based on textures transparency value
        ALPHA = SDL_BLENDMODE_BLEND,

        /// \brief add the two textures color values
        ADD = SDL_BLENDMODE_ADD,

        /// \brief multiply the two texture color values
        MULTIPLY = SDL_BLENDMODE_MOD
    };

    class Window;

    /// \brief an abstract texture object
    class Texture
    {
        public:
            /// \brief default constructor deleted, use RenderTexture or StaticTexture to create a Texture
            Texture() = delete;

            /// \brief destructor, automatically deallocates memory GPU-side
            virtual ~Texture();

            /// \brief set the color each pixels value will be multiplied by
            /// \param color
            void set_color(RGBA);

            /// \brief get the color each pixels value will be multiplied by
            /// \returns color
            RGBA get_color() const;

            /// \brief set the textures blend-mode, this affect all subsequent rendering of the texture
            /// \param blend_mode: blend mode to be set
            void set_blend_mode(TextureBlendMode);

            /// \brief get the textures blend-mode
            /// \returns currently active blend-mode, TextureBlendMode::ALPHA by default
            TextureBlendMode get_blend_mode() const;

            /// \brief set the textures filtering mode, this will affect all subsequent rendering of the texture
            /// \param filtering_mode: filtering mode to be set
            void set_filtering_mode(TextureFilteringMode);

            /// \brief get the textures filtering-mode
            /// \returns currently active filtering-mode, TextureFilteringMode::ANISOTROPIC by default
            TextureFilteringMode get_filtering_mode() const;

            /// \brief get the native SDL_Texture handle
            /// \returns pointer to SDL_Texture
            SDL_Texture* get_native();

            /// \brief get the window used as the rendering context of the texture
            /// \returns pointer to Window
            Window* get_window() const;

            /// \brief get the textures size
            /// \returns size, in pixels
            Vector2ui get_size() const;

        protected:
            Texture(Window*);
            SDL_Texture* _texture;
            void update();

        private:
            Window* _window;
            RGBA _color = RGBA(1, 1, 1, 1);
            TextureBlendMode _blend_mode = ALPHA;
            TextureFilteringMode _filtering_mode = LINEAR;

            Vector2f _size;
    };
}