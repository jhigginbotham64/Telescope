// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/triangle_shape.hpp>
#include <include/vertex.hpp>

namespace ts
{
    TriangleShape::TriangleShape(Vector2f a, Vector2f b, Vector2f c)
    {
        // find bounding box to infer texture coordinates

        static auto infinity = std::numeric_limits<float>::max();
        static auto negative_infinity = std::numeric_limits<float>::min();

        float min_x = infinity;
        float min_y = infinity;
        float max_x = negative_infinity;
        float max_y = negative_infinity;

        for (auto &v: {a, b, c})
        {
            max_x = std::max(max_x, v.x);
            max_y = std::max(max_y, v.y);
            min_x = std::min(min_x, v.x);
            min_y = std::min(min_x, v.y);
        }

        auto top_left = Vector2f(min_x, min_y);
        auto size = Vector2f(max_x - min_x, max_y - min_y);

        _vertices = {
                Vertex(a, abs(a - top_left) / size, RGBA(1, 1, 1, 1)),
                Vertex(b, abs(b - top_left) / size, RGBA(1, 1, 1, 1)),
                Vertex(c, abs(c - top_left) / size, RGBA(1, 1, 1, 1))
        };
        _vertex_indices = {0, 1, 2};
        Shape::signal_vertices_updated();
    }

    TriangleShape::TriangleShape(Triangle triangle)
        : TriangleShape(triangle.a, triangle.b, triangle.c)
    {}
}
