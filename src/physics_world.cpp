// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/physics_world.hpp>
#include <include/window.hpp>

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

    Vector2f PhysicsWorld::native_to_world(Vector2f in) const
    {
        return Vector2f(in.x * pixel_ratio, in.y * pixel_ratio);
    }

    Vector2f PhysicsWorld::world_to_native(Vector2f in) const
    {
        return Vector2f(in.x / pixel_ratio, in.y / pixel_ratio);
    }

    float PhysicsWorld::get_skin_radius() const
    {
        // in Box2D, polygon (as opposed to circles) have a "skin" which is a buffer zone around the
        // physics object. If configured wrongly, objects will not look like they are colliding, because
        // their hitbox is increased by the size of the skin. If set too low, the objects will
        // seem to overlap.

        return 1.5f / pixel_ratio;
    }

    void PhysicsWorld::clear_forces()
    {
        _world.ClearForces();
    }

    void PhysicsWorld::set_gravity(Vector2f vec)
    {
        vec = world_to_native(vec);
        _world.SetGravity(b2Vec2(vec.x, vec.y));
    }

    Vector2f PhysicsWorld::get_gravity() const
    {
        auto gravity = _world.GetGravity();
        auto out = Vector2f(gravity.x, gravity.y);
        return native_to_world(out);
    }
}

