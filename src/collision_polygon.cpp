//
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <array>
#include <exception>
#include <sstream>
#include <algorithm>

#include <include/collision_polygon.hpp>
#include <include/physics_world.hpp>

namespace ts
{
    // NOTE
    //
    // all vertex coordinates are local coordinates, where (0, 0) is the **centroid** of the shape
    // for example, if a square of size s has its centroid at global position (cx, cy), then the...
    //
    // upper left  vertex will be at (cx - 0.5s, cy - 0.5s)
    // upper right vertex will be at (cx + 0.5s, cy - 0.5s)
    // lower right vertex will be at (cx + 0.5s, cy + 0.5s)
    // lower left  vertex will be at (cx - 0.5s, cy + 0.5s)
    //
    // (where -y: screen-up, +y: screen-down, -x: screen-left, +x: screen-right)
    //
    // Because coordinates are local, during the constructors below the centroid will always be at (0, 0)
    //

    b2Shape *CollisionPolygon::get_native_shape()
    {
        return &_shape;
    }

    CollisionPolygon::CollisionPolygon(PhysicsWorld* world, CollisionType type, Triangle tri)
        : CollisionShape(world, type, (tri.a + tri.b + tri.c) / Vector2f(3, 3))
    {
        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;
        points.reserve(3);

        auto center = (tri.a + tri.b + tri.c) / Vector2f(3, 3);
        auto a = _world->world_to_native(tri.a - center);
        auto b = _world->world_to_native(tri.b - center);
        auto c = _world->world_to_native(tri.c - center);

        points.emplace_back(a.x, a.y);
        points.emplace_back(b.x, b.y);
        points.emplace_back(c.x, c.y);

        _shape.Set(points.data(), points.size());
        _shape.m_radius = _world->get_skin_radius();

        auto def = create_fixture_def(&_shape);
        _fixture = _body->CreateFixture(&def);
    }

    CollisionPolygon::CollisionPolygon(PhysicsWorld* world, CollisionType type, const TriangleShape& tri)
        : CollisionPolygon(world, type, [&]() -> Triangle {
            return Triangle{tri.get_vertex_position(0), tri.get_vertex_position(1), tri.get_vertex_position(2)};
        }())
    {}

    CollisionPolygon::CollisionPolygon(PhysicsWorld* world, CollisionType type, Rectangle rect)
        : CollisionShape(world, type, rect.top_left + rect.size * Vector2f(0.5, 0.5))
    {
        _shape = b2PolygonShape();

        std::vector<b2Vec2> points;
        points.reserve(4);

        auto size = rect.size * Vector2f(0.5, 0.5);
        size = _world->world_to_native(size);

        points.emplace_back(-size.x, -size.y);
        points.emplace_back( size.x, -size.y);
        points.emplace_back( size.x,  size.y);
        points.emplace_back(-size.x,  size.y);

        _shape.Set(points.data(), points.size());
        _shape.m_radius = _world->get_skin_radius();

        auto def = create_fixture_def(&_shape);
        _fixture = _body->CreateFixture(&def);
    }

    CollisionPolygon::CollisionPolygon(PhysicsWorld* world, CollisionType type, const RectangleShape& rect)
        : CollisionPolygon(world, type, [&]() -> Rectangle {
            return Rectangle{rect.get_top_left(), rect.get_size()};
        }())
    {}

    CollisionPolygon::CollisionPolygon(PhysicsWorld* world, CollisionType type, const std::vector<Vector2f>& positions)
        : CollisionShape(world, type, [&]() -> Vector2f {

           auto out = Vector2f(0, 0);
           for (auto& v : positions)
               out += v;

           return out / Vector2f(positions.size(), positions.size());
        }())
    {
        if (positions.size() > 8)
        {
            std::stringstream str;
            str << "In ts::CollisionPolygon CTor: Maximum number of vertices (" << 8 << ") exceeded." \
                << "To achieve the desired geometry, instead decompose the polygon into multiple smaller polygons." << std::endl;
            // TODO: implement multi-fixure physics body to make this easier

            throw std::invalid_argument(str.str());
        }

        auto center = Vector2f(0, 0);
        for (auto& pos : positions)
            center += pos;

        size_t n = positions.size();
        center /= Vector2f(n, n);

        std::vector<std::pair<Vector2f, Angle>> by_angle;
        for (auto& pos : positions)
            by_angle.emplace_back(pos, radians(std::atan2(pos.x - center.x, pos.y - center.y)));

        std::sort(by_angle.begin(), by_angle.end(), [](const std::pair<Vector2f, Angle>& a, const std::pair<Vector2f, Angle>& b)
        {
            return a.second.as_degrees() < b.second.as_degrees();
        });

        auto points = std::vector<b2Vec2>();
        for (auto& pair : by_angle)
        {
            auto point = pair.first;
            point -= center;
            point = _world->world_to_native(point);
            points.emplace_back(b2Vec2(point.x, point.y));
        }

        _shape.Set(points.data(), points.size());
        _shape.m_radius = _world->get_skin_radius();

        auto def = create_fixture_def(&_shape);
        _fixture = _body->CreateFixture(&def);
    }

    CollisionPolygon::CollisionPolygon(PhysicsWorld* world, CollisionType type, const PolygonShape & poly)
        : CollisionPolygon(world, type, [&]() -> std::vector<Vector2f> {

            auto out = std::vector<Vector2f>();
            out.reserve(poly.get_n_vertices());

            for (size_t i = 0; i < poly.get_n_vertices(); ++i)
                out.push_back(poly.get_vertex_position(i));

            return out;
        }())
    {}
}