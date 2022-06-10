// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/vertex.hpp>

namespace ts
{
    Vertex::Vertex(
        float position_x,
        float position_y,
        float tex_coord_x,
        float tex_coord_y,
        float r,
        float g,
        float b,
        float a)
        : Vertex(Vector2f(position_x, position_y), Vector2f(tex_coord_x, tex_coord_y), RGBA(r, g, b, a))
    {}

    Vertex::Vertex(Vector2f pos, Vector2f tex_coords, RGBA col)
        : position(pos), texture_coordinates(tex_coords), color(col)
    {}

    Vertex::operator SDL_Vertex() const
    {
        SDL_Vertex out;

        SDL_FPoint pos;
        pos.x = position.x;
        pos.y = position.y;

        out.color.r = color.red * 255;
        out.color.g = color.green * 255;
        out.color.b = color.blue * 255;
        out.color.a = color.alpha * 255;

        SDL_FPoint tex_coord;
        tex_coord.x = texture_coordinates.x;
        tex_coord.y = texture_coordinates.y;

        out.position = pos;
        out.tex_coord = tex_coord;

        return out;
    }
}

