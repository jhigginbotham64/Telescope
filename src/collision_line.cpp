// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#include <include/physics/collision_line.hpp>

namespace ts
{
    CollisionLine::CollisionLine(PhysicsWorld* world, CollisionType type, Vector2f a, Vector2f b, bool two_sided)
        : CollisionShape(world, type, (a + b) / Vector2f(2, 2))
    {
        _shape = b2EdgeShape();

        if (two_sided)
            _shape.SetTwoSided(b2Vec2(a.x, a.y), b2Vec2(b.x, b.y));
        else
            _shape.SetOneSided(b2Vec2(a.x, a.y), b2Vec2(a.x, a.y), b2Vec2(b.x, b.y), b2Vec2(b.x, b.y));

        auto def = default_fixture_def;
        def.shape = &_shape;

        _fixture = _body->CreateFixture(&def);
    }

    b2Shape* CollisionLine::get_native_shape()
    {
        return &_shape;
    }
}