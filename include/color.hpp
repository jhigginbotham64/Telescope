// 
// Copyright 2022 Clemens Cords
// Created on 25.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/vector.hpp>

namespace ts
{
    struct RGBA;
    struct HSVA;

    /// \brief color in RGBA format
    struct RGBA
    {
        /// \brief default construct
        RGBA();

        /// \brief construct from individual components
        /// \param r: red component, in [0, 1]
        /// \param g: green component, in [0, 1]
        /// \param b: blue component, in [0, 1]
        /// \param a: transparency component, in [0, 1]
        RGBA(float r, float g, float b, float a);

        /// \brief cast to vector
        operator Vector4f();

        /// \brief cast to SDL color
        operator SDL_Color();

        /// \brief cast to RGBA
        operator HSVA();

        /// \brief construct from SDL color
        RGBA(SDL_Color);

        /// \brief convert to HSVA
        HSVA as_hsv() const;

        /// \brief red component, in [0, 1]
        float red;

        /// \brief green component, in [0, 1]
        float green;

        /// \brief blue component, in [0, 1]
        float blue;

        /// \brief transparency component, in [0, 1]
        float alpha;
    };

    /// \brief color in HSVA format
    struct HSVA
    {
        /// \brief default construct
        HSVA();

        /// \brief construct from components
        /// \param h: hue component, in [0, 1]
        /// \param s: saturation component, in [0, 1]
        /// \param v: value component, in [0, 1]
        /// \param a: transparency component, in [0, 1]
        HSVA(float, float, float, float);

        /// \brief cast to vector
        operator Vector4f();

        /// \brief cast to RGBA
        operator RGBA();

        /// \brief convert to RGBA
        RGBA as_rgb() const;

        /// \brief hue component, in [0, 1]
        float hue;

        /// \brief saturation component, in [0, 1]
        float saturation;

        /// \brief value component, in [0, 1]
        float value;

        /// \brief transparency component, in [0, 1]
        float alpha;
    };
}