// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/triangle_shape.hpp>
#include <include/vertex.hpp>

namespace ts
{
    TriangleShape::TriangleShape(Vector2f a, Vector2f b, Vector2f c)
        : _coordinates{a, b, c}
    {
        update();
    }

    TriangleShape::TriangleShape(Triangle triangle)
        : TriangleShape(triangle.a, triangle.b, triangle.c)
    {}

    void TriangleShape::update()
    {
        auto& a = _coordinates.at(0);
        auto& b = _coordinates.at(1);
        auto& c = _coordinates.at(2);

        if (_vertices.empty())
        {
            // find bounding box and infer texture coordinates

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
        }
        else
        {
            for (size_t i = 0; i < 3; ++i)
            {
                _vertices.at(i).position.x = _coordinates.at(i).x;
                _vertices.at(i).position.y = _coordinates.at(i).y;
            }
        }

        float x_sum = 0, y_sum = 0;
        for (size_t i = 0; i < 3; ++i)
        {
            x_sum += _vertices.at(i).position.x;
            y_sum += _vertices.at(i).position.y;
        }

        _centroid = {x_sum / 3.f, y_sum / 3.f};
        _vertex_indices = {0, 1, 2};
        Shape::signal_vertices_updated();
    }

    void TriangleShape::set_centroid(Vector2f position)
    {
        auto offset = position - _centroid;

        for (auto& v : _coordinates)
            v += offset;

        update();
    }

    Vector2f TriangleShape::get_centroid() const
    {
        return _centroid;
    }
}
