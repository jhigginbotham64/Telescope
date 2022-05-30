// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include <include/render_target.hpp>
#include <include/shape.hpp>
#include <stdexcept>

namespace ts
{
    void Shape::render(const RenderTarget* target) const
    {
        SDL_RenderGeometry(
            target->get_renderer(),
            (_texture != nullptr ? _texture->get_native() : nullptr),
            _vertices.data(),
            _vertices.size(),
            nullptr, 0);
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

    Vector2f Shape::get_origin() const
    {
        return _origin;
    }

    void Shape::set_origin(Vector2f origin)
    {
        _origin = origin;
    }
}

