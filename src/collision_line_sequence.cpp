//
// Created by clem on 6/19/22.
//

#include <include/collision_line_sequence.hpp>
#include <include/logging.hpp>
#include <include/physics_world.hpp>

#include <exception>

namespace ts
{
    CollisionLineSequence::CollisionLineSequence(
            PhysicsWorld *world,
            CollisionType type,
            const std::vector<Vector2f>& vertices,
            bool two_sided)
            : CollisionShape(world, type, [&]() -> Vector2f {

        if (vertices.size() < 2)
            throw std::invalid_argument("In CollisionLineSequence Constructor: at least 2 vertices need to be specified.");

        auto sum = Vector2f(0, 0);
        size_t n = vertices.size() - 1;
        for (size_t i = 0; i < n; ++i)
            sum += (vertices.at(i) + vertices.at(i+1)) / Vector2f(2, 2);

        return sum / Vector2f(n, n);
    }())
    {
        if (type == ts::DYNAMIC)
        {
            static bool once = false;

            if (not once)
            {
                Log::warning("In ts::CollisionLineSequence Constructor: collision lines cannot be dynamic because they have no volume. Type will be set to ts::KINEMATIC instead.");
                once = true;
            }

            type = ts::KINEMATIC;
        }

        auto center = Vector2f(0, 0);
        size_t n = vertices.size() - 1;
        for (size_t i = 0; i < n; ++i)
            center += (vertices.at(i) + vertices.at(i+1)) / Vector2f(2, 2);
        center /= Vector2f(n, n);

        for (size_t i = 0; i < n; ++i)
        {
            auto previous = i != 0 ? vertices.at(i-1) : vertices.at(0);
            auto next = i != vertices.size() - 1 ? vertices.at(i+1) : vertices.back();

            auto a = vertices.at(i);
            auto b = vertices.at(i+1);

            _shape = b2EdgeShape();
            a = _world->world_to_native(a - center);
            b = _world->world_to_native(b - center);
            next = _world->world_to_native(next - center);
            previous = _world->world_to_native(previous - center);

            if (two_sided)
            {
                _shape.SetTwoSided(b2Vec2(a.x, a.y), b2Vec2(b.x, b.y));
            }
            else
            {
                _shape.SetOneSided(
                        b2Vec2(previous.x, previous.y),
                        b2Vec2(a.x, a.y),
                        b2Vec2(b.x, b.y),
                        b2Vec2(next.x, next.y));
            }

            auto def = create_fixture_def(&_shape);
            _subsequent_fixtures.push_back(_body->CreateFixture(&def));
        }
        _fixture = _subsequent_fixtures.front();
    }

    b2Shape *CollisionLineSequence::get_native_shape()
    {
        return &_shape;
    }
}