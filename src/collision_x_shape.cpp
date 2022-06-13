//
// Created by clem on 6/9/22.
//

#include <include/collision_circle_shape.hpp>
#include <include/collision_rectangle_shape.hpp>
#include <include/collision_triangle_shape.hpp>
#include <include/geometric_shapes.hpp>
#include <include/collision_line_shape.hpp>
#include <include/collision_wireframe_shape.hpp>

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

    CollisionWireframeShape::CollisionWireframeShape(PhysicsWorld* world, CollisionType type, const std::vector<Vector2f> & vertices)
        : CollisionWireframe(world, type, vertices)
    {
        assert(true);
    }
}
