// 
// Copyright 2022 Joshua Higginbotham
// Created on 25.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <SDL2/SDL_render.h>

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
        Vector2f texture_coordinates = Vector2f(0, 0);

        //
        RGBA color = RGBA(1, 1, 1, 1);

        //
        Vertex() = default;

        //
        Vertex(float position_x, float position_y, float tex_coord_x, float tex_coord_y, float r, float g, float b, float a);

        //
        Vertex(Vector2f position, Vector2f tex_coords, RGBA color);

        //
        operator SDL_Vertex() const;
    };
}