// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include "include/collision_circle.hpp"

namespace ts
{
    CollisionCircle::CollisionCircle(PhysicsWorld* world, CollisionType type, Vector2f center, float radius)
        : CollisionShape(world, type, center - Vector2f(radius, radius))
    {
        // local coordinates, origin is center at (0, 0)

        _shape = b2CircleShape();
        _shape.m_p.Set(0, 0);
        _shape.m_radius = radius;

        auto def = default_fixture_def;
        def.shape = &_shape;

        _fixture = _body->CreateFixture(&def);
    }

    CollisionCircle::CollisionCircle(PhysicsWorld* world, CollisionType type, ts::Circle circle)
        : CollisionShape(world, type, circle.center - Vector2f(circle.radius, circle.radius))
    {
        _shape = b2CircleShape();
        _shape.m_p.Set(circle.center.x, circle.center.y);
        _shape.m_radius = circle.radius;

        auto def = default_fixture_def;
        def.shape = &_shape;

        _fixture = _body->CreateFixture(&def);
    }

    CollisionCircle::CollisionCircle(PhysicsWorld* world, CollisionType type, const CircleShape& shape)
        : CollisionShape(world, type, shape.get_centroid() - Vector2f(shape.get_radius(), shape.get_radius()))
    {
        _shape = b2CircleShape();
        _shape.m_p.Set(shape.get_centroid().x, shape.get_centroid().y);
        _shape.m_radius = shape.get_radius();

        auto def = default_fixture_def;
        def.shape = &_shape;

        _fixture = _body->CreateFixture(&def);
    }

    b2Shape* CollisionCircle::get_native_shape()
    {
        return &_shape;
    }
}