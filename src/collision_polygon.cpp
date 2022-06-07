//
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <array>
#include <exception>
#include <sstream>

#include <include/physics/collision_polygon.hpp>

namespace ts
{
    b2Shape *CollisionPolygon::get_native_shape()
    {
        return &_shape;
    }

    CollisionPolygon::CollisionPolygon(Triangle tri)
    {
        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;
        points.reserve(3);

        points.emplace_back(tri.a.x, tri.a.y);
        points.emplace_back(tri.b.x, tri.b.y);
        points.emplace_back(tri.c.x, tri.c.y);

        _shape.Set(points.data(), points.size());
    }

    CollisionPolygon::CollisionPolygon(Rectangle rect)
    {
        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;
        points.reserve(3);

        auto w = rect.size.x,
             h = rect.size.y;

        auto x = rect.top_left.x,
             y = rect.top_left.y;

        points.emplace_back(x, y);
        points.emplace_back(x + w, y);
        points.emplace_back(x + w, y + h);
        points.emplace_back(x, y + h);

        _shape.Set(points.data(), points.size());
    }

    CollisionPolygon::CollisionPolygon(const std::vector<Vector2f>& vec)
    {
        if (vec.size() < b2_maxPolygonVertices)
        {
            std::stringstream str;
            str << "In ts::CollisionPolygon CTor: Maximum number of vertices (" << b2_maxPolygonVertices << ") exceeded." \
                << "To achieve the desired geometry, instead decompose it into multiple smaller polygons and attach them to the same body." << std::endl;

            throw std::invalid_argument(str.str());
        }

        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;
        points.reserve(vec.size());

        for (auto& point : vec)
            points.emplace_back(point.x, point.y);

        _shape.Set(points.data(), points.size());
    }

    CollisionPolygon::CollisionPolygon(const TriangleShape& tri)
    {
        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;
        points.reserve(3);

        auto a = tri.get_vertex_position(0);
        points.emplace_back(a.x, a.y);

        auto b = tri.get_vertex_position(1);
        points.emplace_back(b.x, b.y);

        auto c= tri.get_vertex_position(2);
        points.emplace_back(c.x, c.y);

        _shape.Set(points.data(), points.size());
    }

    CollisionPolygon::CollisionPolygon(const RectangleShape& rect)
    {
        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;
        points.reserve(4);

        for (size_t i = 0; i < 4; ++i)
            points.emplace_back(rect.get_vertex_position(i).x, rect.get_vertex_position(i).y);

        _shape.Set(points.data(), points.size());
    }

    CollisionPolygon::CollisionPolygon(const PolygonShape & poly)
    {
        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;

        for (size_t i = 0; i < poly.get_n_vertices(); ++i)
            points.emplace_back(poly.get_vertex_position(i).x, poly.get_vertex_position(i).y);

        _shape.Set(points.data(), points.size());
    }
}