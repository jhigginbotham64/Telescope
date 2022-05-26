// 
// Copyright 2022 Clemens Cords
// Created on 25.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/vector.hpp>

namespace ts
{
    struct RGBA;
    struct HSVA;

    //
    struct RGBA
    {
        //
        RGBA();

        //
        RGBA(float, float, float, float);

        //
        RGBA(Vector4f);

        //
        operator Vector4f();

        //
        HSVA as_hsv() const;

        // in [0, 1]
        float red, green, blue;

        // in [0, 1]
        float alpha;
    };

    //
    struct HSVA
    {
        //
        HSVA();

        //
        HSVA(float, float, float, float);

        //
        HSVA(Vector4f);

        //
        operator Vector4f();

        //
        RGBA as_rgb() const;

        // in [0, 1]
        float hue, saturation, value;

        // in [0, 1]
        float alpha;
    };
}