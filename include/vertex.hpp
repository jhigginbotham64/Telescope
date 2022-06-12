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

    /// \brief 2d vertex
    struct Vertex
    {
        /// \brief position in 2d space, world coordinates
        Vector2f position = Vector2f(0, 0);

        /// \brief texture coordinate, local where (0, 0) is the top-left of the texture, (1, 1) is the bottom right
        Vector2f texture_coordinates = Vector2f(0, 0);

        /// \brief vertex color, will be multiplied with each pixels color data
        RGBA color = RGBA(1, 1, 1, 1);

        // no docs
        Vertex() = default;

        // no docs
        Vertex(float position_x, float position_y, float tex_coord_x, float tex_coord_y, float r, float g, float b, float a);

        // no docs
        Vertex(Vector2f position, Vector2f tex_coords, RGBA color);

        // no docs
        operator SDL_Vertex() const;
    };
}