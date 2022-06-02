// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#include <stdexcept>
#include <iostream> //TODO

#include <glm/glm.hpp>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include <include/render_target.hpp>
#include <include/shape.hpp>

namespace ts
{
    void Shape::signal_vertices_updated()
    {
        update_xy();
        update_colors();
        update_uv();
    }

    void Shape::update_xy()
    {
        _xy.clear();
        _xy.reserve(2 * _vertices.size());
        for (auto& v : _vertices)
        {
            _xy.push_back(v.position.x);
            _xy.push_back(v.position.y);
        }
    }

    void Shape::update_colors()
    {
        _colors.clear();
        _colors.reserve(_vertices.size());
        for (auto& v : _vertices)
            _colors.push_back(v.color);
    }

    void Shape::update_uv()
    {
        _uv.clear();
        _uv.reserve(2 * _vertices.size());
        for (auto& v : _vertices)
        {
            _uv.push_back(v.tex_coord.x);
            _uv.push_back(v.tex_coord.y);
        }
    }

    void Shape::move(float x_offset, float y_offset)
    {
        for (auto& v : _vertices)
        {
            v.position.x += x_offset;
            v.position.y += y_offset;
        }

        update_xy();
    }

    void Shape::set_color(RGBA color, int vertex_index)
    {
        auto col = color.operator SDL_Color();
        if (vertex_index == -1)
        {
            for (auto& v : _vertices)
                v.color = col;
        }
        else
            _vertices.at(vertex_index).color = col;

        update_colors();
    }

    RGBA Shape::get_color(size_t vertex_index) const
    {
        auto& v = _vertices.at(vertex_index);
        return RGBA(v.color);
    }

    size_t Shape::get_n_vertices() const
    {
        return _vertices.size();
    }

    Texture * Shape::get_texture() const
    {
        return _texture;
    }

    void Shape::set_texture(Texture* texture)
    {
        _texture = texture;
    }

    Rectangle Shape::get_bounding_box() const
    {
        static auto infinity = std::numeric_limits<float>::max();
        static auto negative_infinity = std::numeric_limits<float>::min();

        float min_x = infinity;
        float min_y = infinity;
        float max_x = negative_infinity;
        float max_y = negative_infinity;

        for (auto& v : _vertices)
        {
            max_x = std::max(max_x, v.position.x);
            max_y = std::max(max_y, v.position.y);
            min_x = std::min(min_x, v.position.x);
            min_y = std::min(min_x, v.position.y);
        }

        return Rectangle{Vector2f{min_x, min_y}, Vector2f{max_x - min_x, max_y - min_y}};
    }

    void Shape::set_vertex_position(size_t index, Vector2f pos)
    {
        _vertices.at(index).position.x = pos.x;
        _vertices.at(index).position.y = pos.y;

        update_xy();
    }

    void Shape::set_vertex_color(size_t index, RGBA color)
    {
        _vertices.at(index).color = color.operator SDL_Color();

        update_colors();
    }

    void Shape::set_vertex_texture_coordinates(size_t index, Vector2f relative)
    {
        _vertices.at(index).tex_coord.x = relative.x;
        _vertices.at(index).tex_coord.y = relative.y;
    }

    Vector2f Shape::get_vertex_position(size_t index) const
    {
        auto pos = _vertices.at(index).position;
        return Vector2f{pos.x, pos.y};
    }

    Vector2f Shape::get_vertex_texture_coordinates(size_t index)
    {
        auto pos = _vertices.at(index).tex_coord;
        return Vector2f{pos.x, pos.y};
    }

    RGBA Shape::get_vertex_color(size_t index) const
    {
        return RGBA(_vertices.at(index).color);
    }
}

