// 
// Copyright 2022 Clemens Cords
// Created on 25.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/vector.hpp>
#include <include/color.hpp>

namespace ts
{
    class Texture;

    //
    struct Vertex
    {
        //
        Vector2f position = Vector2f(0, 0);

        //
        Texture* texture = nullptr;

        //
        Vector2f texture_coordinates = Vector2f(0, 0);

        //
        RGBA color = RGBA(1, 1, 1, 1);
    };
}