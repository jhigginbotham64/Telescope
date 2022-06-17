// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <SDL2/SDL_render.h>

#include <include/circle_shape.hpp>

namespace ts
{
    CircleShape::CircleShape(Vector2f center, float radius, size_t n_outer_vertices)
        : _center(center), _radius(radius), _n_vertices(n_outer_vertices)
    {
        update();
    }

    CircleShape::CircleShape(float center_x, float center_y, float radius, size_t n_outer_vertices)
        : CircleShape({center_x, center_y}, radius, n_outer_vertices)
    {}

    void CircleShape::update()
    {
        // triangle fan decomposition

        size_t n = _n_vertices + 1;
        size_t step = 360 / _n_vertices;

        std::vector<RGBA> colors;
        if (not _vertices.empty())
        {
            for (auto& v : _vertices)
                colors.emplace_back(v.color);
        }

        auto middle = SDL_Vertex();
        middle.position.x = _center.x;
        middle.position.y = _center.y;
        middle.tex_coord.x = 0.5;
        middle.tex_coord.y = 0.5;

        // TODO: optimize this, outers are allocated twice redundantly

        auto outer = std::vector<SDL_Vertex>();
        for (size_t angle = 0; angle < 360; angle += step)
        {
            outer.emplace_back();
            float radians = angle * M_PI / 180;
            outer.back().position.x = _center.x + cos(radians) * _radius;
            outer.back().position.y = _center.y + sin(radians) * _radius;
            outer.back().tex_coord.x = 0.5 + cos(radians) * 0.5;
            outer.back().tex_coord.y = 0.5 + sin(radians) * 0.5;
        }

        _vertices.clear();
        _vertices.push_back(middle);
        for (size_t i = 1; i < outer.size(); ++i)
            _vertices.push_back(outer.at(i));

        _vertex_indices.clear();

        for (size_t i = 2; i < outer.size(); ++i)
        {
            _vertex_indices.push_back(0);
            _vertex_indices.push_back(i-1);
            _vertex_indices.push_back(i);
        }

        _vertex_indices.push_back(0);
        _vertex_indices.push_back(outer.size() - 1);
        _vertex_indices.push_back(1);

        for (size_t i = 0; i < _vertices.size(); ++i)
        {
            if (not colors.empty())
                _vertices.at(i).color = colors.at(i).operator SDL_Color();
            else
                _vertices.at(i).color = RGBA(1, 1, 1, 1).operator SDL_Color();
        }

        Shape::signal_vertices_updated();
    }

    void CircleShape::set_centroid(Vector2f center)
    {
        _center = center;
        update();
    }

    float CircleShape::get_radius() const
    {
        return _radius;
    }

    void CircleShape::set_radius(float radius)
    {
        _radius = radius;
        update();
    }
}

