// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#include <include/circle_shape.hpp>

namespace ts
{
    CircleShape::CircleShape(Vector2f center, float radius, size_t n_vertices)
        : _n_vertices(n_vertices), _radius(radius), Shape({})
    {
        update(center);
    }

    void CircleShape::update(Vector2f center)
    {
        // triangle fan decomposition

        size_t n = _n_vertices + 1;
        size_t step = 360 / _n_vertices;

        std::vector<SDL_Vertex> old = _vertices;
        _vertices.clear();

        _vertices.emplace_back();
        _vertices.back().position = center;
        _vertices.back().color = old.at(0).color;
        _vertices.back().tex_coord.x = 0.5;
        _vertices.back().tex_coord.y = 0.5;

        for (size_t angle = 0, i = 1; angle < 360; angle += step, i++)
        {
            _vertices.emplace_back();
            _vertices.back().position.x = center.x + cos(angle) * radius;
            _vertices.back().position.x = center.y + sin(angle) * radius;
            _vertices.back().color = old.at(i).color;
            _vertices.back().tex_coord.x = 0.5 + cos(angle) * 0.5;
            _vertices.back().tex_coord.y = 0.5 + sin(angle) * 0.5;
        }
    }

    Vector2f CircleShape::get_center() const
    {
        auto sum = Vector2f(0, 0);

        for (auto& v : _vertices)
        {
            sum.x += v.position.x;
            sum.y += v.position.y;
        }

        return sum / _vertices.size();
    }

    void CircleShape::set_center(Vector2f center) const
    {
        update(center);
    }

    void CircleShape::set_radius(float radius) const
    {
        _radius = radius;
        update();
    }
}

