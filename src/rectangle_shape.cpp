// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#include <include/rectangle_shape.hpp>

namespace ts
{
    RectangleShape::RectangleShape(Vector2f top_left, Vector2f size)
        : Shape({
                // in order of tri decomposition: top left, top_right, bottom_left, bottom_right
                Vertex(top_left, Vector2f(0, 0), RGBA(1, 1, 1, 1)),
                Vertex(top_left + Vector2f(size.x, 0), Vector2f(0, 1), RGBA(1, 1, 1, 1)),
                Vertex(top_left + Vector2f(0, size.y), Vector2f(1, 0), RGBA(1, 1, 1, 1)),
                Vertex(top_left + size, Vector2f(1, 1), RGBA(1, 1, 1, 1))
            })
    {}

    RectangleShape::RectangleShape(float top_left_x, float top_left_y, float width, float height)
        : RectangleShape(Vector2f(top_left_x, top_left_y), Vector2f(width, height))
    {}

    Vector2f RectangleShape::get_size() const
    {
        static auto infinity = std::numeric_limits<float>::max();
        static auto negative_infinity = std::numeric_limits<float>::min();

        // TODO: only works if rectangle is axis-aligned

        float max_x = infinity;
        float max_y = infinity;
        float min_x = negative_infinity;
        float min_y = negative_infinity;

        for (auto& v : _vertices)
        {
            max_x = std::max(max_x, v.position.x);
            max_y = std::max(max_y, v.position.y);
            min_x = std::min(min_x, v.position.x);
            min_y = std::min(min_x, v.position.y);
        }

        return Vector2f(max_x - min_x, max_y - min_y);
    }
}

