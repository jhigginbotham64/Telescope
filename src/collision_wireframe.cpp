// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#include <include/physics/collision_wireframe.hpp>

namespace ts
{
    b2Shape* CollisionWireFrame::get_native_shape()
    {
        return &_shape;
    }

    CollisionWireFrame::CollisionWireFrame(PhysicsWorld* world, CollisionType type, const std::vector<Vector2f>& vec)
        : CollisionShape(world, type, [&]() -> Vector2f {

            auto sum = Vector2f(0, 0);
            for (auto& v : vec)
                sum += v;

            return sum / Vector2f(vec.size(), vec.size());
        }())
    {
        _shape = b2ChainShape();

        std::vector<b2Vec2> points;
        points.reserve(vec.size());

        for (auto& v : vec)
            points.emplace_back(v.x, v.y);

        _shape.CreateLoop(points.data(), points.size());

        auto def = default_fixture_def;
        def.shape = &_shape;

        _fixture = _body->CreateFixture(&def);
    }
}