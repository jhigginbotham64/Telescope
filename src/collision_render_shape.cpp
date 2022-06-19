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
        : CollisionPolygon(world, type, Rectangle{top_left, size}), RectangleShape(top_left, size)
    {}

    void CollisionRectangleShape::update()
    {
        RectangleShape::set_centroid(CollisionShape::get_centroid());
        RectangleShape::rotate(CollisionShape::get_rotation());
    }

    CollisionTriangleShape::CollisionTriangleShape(
        PhysicsWorld* world,
        CollisionType type,
        Vector2f a,
        Vector2f b,
        Vector2f c)
        : CollisionPolygon(world, type, Triangle{a, b, c}), TriangleShape(a, b, c)
    {}

    void CollisionTriangleShape::update()
    {
        TriangleShape::set_centroid(CollisionShape::get_centroid());
        TriangleShape::rotate(CollisionShape::get_rotation());
    }

    CollisionCircleShape::CollisionCircleShape(
        PhysicsWorld* world,
        CollisionType type,
        Vector2f center,
        float radius)
        : CollisionCircle(world, type, Circle{center, radius}), CircleShape(center, radius, 32)
    {}

    void CollisionCircleShape::update()
    {
        CircleShape::set_centroid(CollisionShape::get_centroid());
        CircleShape::rotate(CollisionShape::get_rotation());
    }

    CollisionLineShape::CollisionLineShape(PhysicsWorld* world, CollisionType type, Vector2f a, Vector2f b)
        : CollisionLine(world, type, a, b, true), RectangleShape((a + b) / Vector2f(2, 2), Vector2f(glm::distance(a, b), 1))
    {
        RectangleShape::rotate(CollisionLine::get_rotation());
    }

    void CollisionLineShape::update()
    {
        RectangleShape::set_centroid(CollisionLine::get_centroid());
        RectangleShape::rotate(CollisionLine::get_rotation());
    }

    CollisionLineSequenceShape::CollisionLineSequenceShape(
        PhysicsWorld* world,
        CollisionType type,
        const std::vector<Vector2f>& vertices,
        bool is_two_sided)
        : CollisionLineSequence(world, type, vertices, is_two_sided)
    {
        for (size_t i = 0; i < vertices.size() - 1; ++i)
        {
            auto a = vertices.at(i);
            auto b = vertices.at(i+1);
            auto center = (a + b) / Vector2f(2, 2);
            _lines.push_back(RectangleShape(center, Vector2f(glm::distance(a, b), 1)));
            _lines.back().rotate(radians(std::atan2((b-a).x, (b-a).y)));
        }
    }

    std::vector<RectangleShape> &CollisionLineSequenceShape::get_shapes()
    {
        return _lines;
    }

    void CollisionLineSequenceShape::update()
    {
    }

    void CollisionLineSequenceShape::render(RenderTarget *target, Transform transform) const
    {
        for (auto& e : _lines)
            e.render(target, transform);
    }

    CollisionPolygonShape::CollisionPolygonShape(PhysicsWorld* world, CollisionType type, const std::vector<Vector2f> & vertices)
        : CollisionPolygon(world, type, vertices), PolygonShape(vertices)
    {}

    void CollisionPolygonShape::update()
    {
        PolygonShape::set_centroid(CollisionPolygon::get_centroid());
        PolygonShape::rotate(CollisionPolygon::get_rotation());
    }
}
