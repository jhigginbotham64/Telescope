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
            const_cast<RenderTarget*>(target),
            _texture != nullptr ? _texture->get_native() : nullptr,
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

    /*
    Vector2f Shape::get_origin() const
    {
        return _origin;
    }

    void Shape::set_origin(Vector2f origin)
    {
        _origin = origin;
    }
     */

    Rectangle Shape::get_bounding_box() const
    {
        static auto infinity = std::numeric_limits<float>::max();
        static auto negative_infinity = std::numeric_limits<float>::min();

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

        return Rectangle{Vector2f{min_x, min_y}, Vector2f{max_x - min_x, max_y - min_y}};
    }
}

