// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#include <include/physics/physics_world.hpp>

namespace ts
{
    PhysicsWorld::PhysicsWorld()
        : _world(_default_gravity)
    {}

    PhysicsWorld::~PhysicsWorld()
    {}

    void PhysicsWorld::step(Time timestep, int32_t velocity_iterations, int32_t position_iterations)
    {
        _world.Step(timestep.as_seconds(), velocity_iterations, position_iterations);
    }

    b2World *PhysicsWorld::get_native()
    {
        return &_world;
    }

    void PhysicsWorld::clear_forces()
    {
        _world.ClearForces();
    }

    void PhysicsWorld::set_gravity(Vector2f vec)
    {
        _world.SetGravity(b2Vec2(vec.x, vec.y));
    }

    Vector2f PhysicsWorld::get_gravity() const
    {
        auto out = _world.GetGravity();
        return Vector2f(out.x, out.y);
    }
}

