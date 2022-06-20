//
// Created by clem on 6/9/22.
//

#include <include/collision_render_shape.hpp>

#include <iostream>

namespace ts
{
    CollisionRectangleShape::CollisionRectangleShape(
        PhysicsWorld* world,
        CollisionType type,
        Vector2f top_left,
        Vector2f size)
        : RectangleShape(top_left, size), CollisionPolygon(world, type, Rectangle{top_left, size})
    {}

    void CollisionRectangleShape::update()
    {
        RectangleShape::set_centroid(CollisionShape::get_centroid());

        auto is = _rotation.as_degrees();
        auto should_be = CollisionShape::get_rotation().as_degrees();
        auto delta = should_be - is;

        RectangleShape::rotate(degrees(delta));
        _rotation = degrees(is + delta);
    }

    CollisionTriangleShape::CollisionTriangleShape(
        PhysicsWorld* world,
        CollisionType type,
        Vector2f a,
        Vector2f b,
        Vector2f c)
        : TriangleShape(a, b, c), CollisionPolygon(world, type, Triangle{a, b, c})
    {}

    void CollisionTriangleShape::update()
    {
        TriangleShape::set_centroid(CollisionShape::get_centroid());

        auto is = _rotation.as_degrees();
        auto should_be = CollisionShape::get_rotation().as_degrees();
        auto delta = should_be - is;

        TriangleShape::rotate(degrees(delta));
        _rotation = degrees(is + delta);
    }

    CollisionCircleShape::CollisionCircleShape(
        PhysicsWorld* world,
        CollisionType type,
        Vector2f center,
        float radius)
        : CircleShape(center, radius, 32), CollisionCircle(world, type, Circle{center, radius})
    {}

    void CollisionCircleShape::update()
    {
        CircleShape::set_centroid(CollisionShape::get_centroid());

        auto is = _rotation.as_degrees();
        auto should_be = CollisionShape::get_rotation().as_degrees();
        auto delta = should_be - is;

        CircleShape::rotate(degrees(delta));
        _rotation = degrees(is + delta);
    }

    namespace detail
    {
        RectangleShape create_line_shape(Vector2f a, Vector2f b)
        {
            auto center = (a + b) / Vector2f(2, 2);
            auto length = glm::distance(a, b);

            auto out = RectangleShape(center - Vector2f(length / 2.f, 0), Vector2f(length, 1));
            auto angle = radians(std::atan2((b - center).x, (b - center).y));
            out.rotate(angle);
            return out;
        }
    }

    CollisionLineShape::CollisionLineShape(PhysicsWorld* world, CollisionType type, Vector2f a, Vector2f b)
        : CollisionLine(world, type, a, b, true), RectangleShape(detail::create_line_shape(a, b))
    {}

    void CollisionLineShape::update()
    {
        RectangleShape::set_centroid(CollisionShape::get_centroid());

        auto is = _rotation.as_degrees();
        auto should_be = CollisionShape::get_rotation().as_degrees();
        auto delta = should_be - is;

        RectangleShape::rotate(degrees(delta));
        _rotation = degrees(is + delta);
    }

    CollisionLineSequenceShape::CollisionLineSequenceShape(
        PhysicsWorld* world,
        CollisionType type,
        const std::vector<Vector2f>& vertices,
        bool is_two_sided)
        : CollisionLineSequence(world, type, vertices, is_two_sided)
    {
        auto center = Vector2f(0, 0);
        size_t n = vertices.size() - 1;
        for (size_t i = 0; i < n; ++i)
            center += (vertices.at(i) + vertices.at(i+1)) / Vector2f(2, 2);
        center /= Vector2f(n, n);

        for (size_t i = 0; i < vertices.size() - 1; ++i)
        {
            auto a = vertices.at(i);
            auto b = vertices.at(i+1);
            auto middle = (a + b) / Vector2f(2, 2);
            auto dist = glm::distance(a, b);
            _lines.emplace_back(middle - Vector2f(dist, 0), Vector2f(dist, 1));
            _lines.back().set_origin(middle - a);
            _lines.back().rotate(radians(std::atan2(a.x - b.x, a.y - b.y) - 0.5 * M_PI));
        }
    }

    std::vector<RectangleShape> &CollisionLineSequenceShape::get_shapes()
    {
        return _lines;
    }

    void CollisionLineSequenceShape::update()
    {}

    void CollisionLineSequenceShape::render(RenderTarget *target, Transform transform) const
    {
        for (auto& e : _lines)
            e.render(target, transform);
    }

    CollisionPolygonShape::CollisionPolygonShape(PhysicsWorld* world, CollisionType type, const std::vector<Vector2f> & vertices)
        : PolygonShape(vertices), CollisionPolygon(world, type, vertices)
    {}

    void CollisionPolygonShape::update()
    {
        PolygonShape::set_centroid(CollisionShape::get_centroid());

        auto is = _rotation.as_degrees();
        auto should_be = CollisionPolygonShape::get_rotation().as_degrees();
        auto delta = should_be - is;

        CollisionPolygonShape::rotate(degrees(delta));
        _rotation = degrees(is + delta);
    }
}
