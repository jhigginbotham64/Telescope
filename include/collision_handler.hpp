// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/6/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include "../../../../../usr/include/c++/12/unordered_map"
#include "../../../../../usr/include/c++/12/deque"
#include "../../../../../usr/include/c++/12/mutex"

#include "physics_world.hpp"
#include "collision_shape.hpp"

namespace ts
{
    /// \brief object returned by ts::CollisionHandler::ray_cast
    struct RayCastInformation
    {
        /// \brief do the ray and the shape overlap at all
        bool are_colliding;

        /// \brief normal vector of the intersection point (if any9
        Vector2f normal_vector; // from a to b

        /// \brief earliest contact point of the ray
        Vector2f contact_point;
    };

    /// \brief object returned by ts::CollisionHandler::distance_between
    struct DistanceInformation
    {
        /// \brief the distance, in pixels
        float distance;

        /// \brief the two closest points of shape a and b respectively
        std::pair<Vector2f, Vector2f> closest_points;
    };

    /// \brief high-level geometric queries in a physics world
    class CollisionHandler
    {
        public:
            /// \brief constructor
            /// \param world: world all the query should be performed in
            CollisionHandler(PhysicsWorld*);

            /// \brief get the minimum distance between two shapes in the world
            /// \param a: first shape
            /// \param b: second shape
            /// \returns object of type ts::DistanceInformation
            DistanceInformation distance_between(CollisionShape* a, CollisionShape* b);

            /// \brief is a point located inside a shape in the world
            /// \param a: shape
            /// \param point: 2d coordinate
            /// \returns true if point is inside shape, false otherwise
            bool is_point_in_shape(CollisionShape* a, Vector2f point);

            /// \brief perform a ray cast at a shape
            /// \param a: shape to be cast at
            /// \param ray_start: starting point of the ray
            /// \param ray_end: ending point of the ray
            /// \returns object of type ts::RayCastInformation
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