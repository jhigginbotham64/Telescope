// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#include <SDL2/SDL_render.h>

#include <include/render_target.hpp>
#include <include/shape.hpp>

namespace ts
{
    void Shape::render(const RenderTarget* target) const
    {
        SDL_RenderGeometry(
            target->get_renderer(),
            _texture->get_native(),
            _vertices.data(),
            _vertices.size(),
            nullptr, 0);
    }

    Shape::Shape(std::vector<Vertex> vertices)
    {
        _vertices.reserve(vertices.size());
        for (auto& v : vertices)
            _vertices.push_back(v.operator SDL_Vertex());
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

    void Shape::set_position(Vector2f position)
    {
        auto old_position =  Vector2f(_vertices.at(0).position.x,  _vertices.at(0).position.y);
        Vector2f offset = - position;

        for (auto& v : _vertices)
        {
            auto current_pos = Vector2f(v.position.x, v.position.y);
            auto new_pos = current_pos + offset;
            v.position.x = new_pos.x;
            v.position.y = new_pos.y;
        }
    }

    Vector2f Shape::get_position(Vector2f position)
    {
        auto& pos = _vertices.at(0).position;
        return Vector2f(pos.x, pos.y);
    }
}

