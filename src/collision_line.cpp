// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/collision_line.hpp>
#include <include/physics_world.hpp>
#include <include/logging.hpp>

namespace ts
{
    CollisionLine::CollisionLine(PhysicsWorld* world, CollisionType type, Vector2f a, Vector2f b, bool two_sided)
        : CollisionShape(world, type, (a + b) / Vector2f(2, 2))
    {
        if (type == ts::DYNAMIC)
        {
            static bool once = false;

            if (not once)
            {
                Log::warning("In ts::CollisionLine Constructor: collision lines cannot be dynamic because they have no volume. Type will be set to ts::KINEMATIC instead.");
                once = true;
            }

            type = ts::KINEMATIC;
        }

        auto center = (a + b) / Vector2f(2, 2);

        _shape = b2EdgeShape();
        a = _world->world_to_native(a - center);
        b = _world->world_to_native(b - center);

        if (two_sided)
            _shape.SetTwoSided(b2Vec2(a.x, a.y), b2Vec2(b.x, b.y));
        else
            _shape.SetOneSided(b2Vec2(a.x, a.y), b2Vec2(a.x, a.y), b2Vec2(b.x, b.y), b2Vec2(b.x, b.y));

        auto def = create_fixture_def(&_shape);
        _fixture = _body->CreateFixture(&def);
    }

    b2Shape* CollisionLine::get_native_shape()
    {
        return &_shape;
    }
}