// 
// Copyright 2022 Clemens Cords
// Created on 6/6/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <unordered_map>
#include <deque>
#include <mutex>

#include <include/physics/physics_world.hpp>
#include <include/physics/collision_shape.hpp>

namespace ts
{
    //
    struct RayCastInformation
    {
        // true if overlapping
        bool are_colliding;

        //
        Vector2f normal_vector; // from a to b

        //
        Vector2f contact_point;
    };

    //
    struct DistanceInformation
    {
        //
        float distance;

        //
        std::pair<Vector2f, Vector2f> closest_points;
    };

    //
    struct CollisionHandler
    {
        public:
            //
            CollisionHandler(PhysicsWorld*);

            // geometric query: minimum distance between two shapes
            DistanceInformation distance_between(CollisionShape* a, CollisionShape* b);

            // geometric query: does point overlap with shape
            bool is_point_in_shape(CollisionShape* a, Vector2f point);

            // geometric query: does line overlap with shape
            RayCastInformation ray_cast(CollisionShape* a, Vector2f ray_start, Vector2f ray_end);

        private:
            PhysicsWorld* _world;

            struct ContactListener : public b2ContactListener
            {
                ContactListener(CollisionHandler*);

                void BeginContact(b2Contact*) override;
                void EndContact(b2Contact*) override;
                void PreSolve(b2Contact*, const b2Manifold*) override;
                void PostSolve(b2Contact*, const b2ContactImpulse*) override;

                CollisionHandler* _handler;
            };

            CollisionShape* fixture_to_shape(b2Fixture*);
            ContactListener _contact_listener;
    };
}