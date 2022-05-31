// 
// Copyright 2022 Clemens Cords
// Created on 5/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/color.hpp>

namespace ts
{
    //
    static inline constexpr SDL_PixelFormatEnum PIXEL_FORMAT = SDL_PIXELFORMAT_RGBA32;

    //
    enum TextureFilteringMode
    {
        //
        NEAREST_NEIGHBOUR = 0,

        //
        LINEAR = 1,

        //
        ANISOTROPIC = 2
    };

    //
    enum TextureBlendMode
    {
        //
        NONE = SDL_BLENDMODE_NONE,

        //
        ALPHA = SDL_BLENDMODE_BLEND,

        //
        ADD = SDL_BLENDMODE_ADD,

        //
        MULTIPLY = SDL_BLENDMODE_MOD
    };

    class Window;

    //
    class Texture
    {
        public:
            //
            Texture() = delete;

            //
            ~Texture();

            //
            void set_color(RGBA);

            //
            RGBA get_color() const;

            //
            void set_blend_mode(TextureBlendMode);

            //
            TextureBlendMode get_blend_mode() const;

            //
            void set_filtering_mode(TextureFilteringMode);

            //
            TextureFilteringMode get_filtering_mode() const;

            //
            SDL_Texture* get_native();

            //
            Window* get_window() const;

        protected:
            Texture(Window*);
            SDL_Texture* _texture;
            void update();

        private:
            Window* _window;
            RGBA _color = RGBA(1, 1, 1, 1);
            TextureBlendMode _blend_mode = ALPHA;
            TextureFilteringMode _filtering_mode = LINEAR;
    };
}