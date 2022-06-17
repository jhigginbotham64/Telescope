// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/rectangle_shape.hpp>
#include <include/vertex.hpp>

namespace ts
{
    void RectangleShape::update()
    {
        if (_vertices.empty())
        {
            auto tl = Vertex(_top_left, Vector2f(0, 0), RGBA(1, 1, 1, 1));
            auto tr = Vertex(_top_left + Vector2f(_size.x, 0), Vector2f(0, 1), RGBA(1, 1, 1, 1));
            auto bl = Vertex(_top_left + Vector2f(0, _size.y), Vector2f(1, 0), RGBA(1, 1, 1, 1));
            auto br = Vertex(_top_left + _size, Vector2f(1, 1), RGBA(1, 1, 1, 1));

            _vertices = {tl, tr, bl, br};
        }
        else
        {
            auto tl = _top_left;
            auto tr = _top_left + Vector2f(_size.x, 0);
            auto bl = _top_left + Vector2f(0, _size.y);
            auto br = _top_left + _size;

            _vertices.at(0).position.x = tl.x;
            _vertices.at(0).position.y = tl.y;

            _vertices.at(1).position.x = tr.x;
            _vertices.at(1).position.y = tr.y;

            _vertices.at(2).position.x = bl.x;
            _vertices.at(2).position.y = bl.y;

            _vertices.at(3).position.x = br.x;
            _vertices.at(3).position.y = br.y;
        }

        _vertex_indices = {0, 1, 2, 1, 2, 3};
        Shape::signal_vertices_updated();
    }

    RectangleShape::RectangleShape(Vector2f top_left, Vector2f size)
        : _top_left(top_left), _size(size)
    {
        update();
    }

    RectangleShape::RectangleShape(float top_left_x, float top_left_y, float width, float height)
        : RectangleShape(Vector2f(top_left_x, top_left_y), Vector2f(width, height))
    {}

    RectangleShape::RectangleShape(Rectangle rectangle)
        : RectangleShape(rectangle.top_left, rectangle.size)
    {}

    void RectangleShape::set_size(Vector2f size)
    {
        _size = size;
        update();
    }

    Vector2f RectangleShape::get_size() const
    {
        return _size;
    }

    void RectangleShape::set_centroid(Vector2f center)
    {
        _top_left = center - (_size / Vector2f{2, 2});
        update();
    }

    Vector2f RectangleShape::get_centroid() const
    {
        return _top_left + (_size / Vector2f{2, 2});
    }

    Vector2f RectangleShape::get_top_left() const
    {
        return _top_left;
    }

    void RectangleShape::set_top_left(Vector2f top_left)
    {
        _top_left = top_left;
        update();
    }
}

