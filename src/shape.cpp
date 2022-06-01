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
        _xy.clear();
        _colors.clear();
        _uv.clear();

        _xy.reserve(_vertices.size() * 2);
        _colors.reserve(_vertices.size());
        _uv.reserve(_vertices.size() * 2);

        for (auto& v : _vertices)
        {
            _xy.push_back(v.position.x);
            _xy.push_back(v.position.y);
            _colors.push_back(v.color);
            _uv.push_back(v.tex_coord.x);
            _uv.push_back(v.tex_coord.y);
        }
    }

    void Shape::render(RenderTarget* target, Transform transform) const
    {
        auto xy = _xy;
        for (size_t i = 0; i < xy.size(); i += 2)
        {
            auto new_pos = transform.apply_to(Vector2f{xy.at(i), xy.at(i+1)});
            xy.at(i) = new_pos.x;
            xy.at(i+1) = new_pos.y;
        }

        SDL_RenderGeometryRaw(
            target->get_renderer(),
            _texture != nullptr ? _texture->get_native() : nullptr,
            xy.data(), 2 * sizeof(float),
            _colors.data(), sizeof(SDL_Color),
            _uv.data(), 2 * sizeof(float),
            _vertices.size(),
            (const void*) nullptr, 0, 0
        );
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
}

