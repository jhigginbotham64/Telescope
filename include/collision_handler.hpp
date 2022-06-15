// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/6/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <unordered_map>
#include <deque>
#include <mutex>

#include <include/physics_world.hpp>
#include <include/collision_shape.hpp>

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

    /// \brief collision event, reports when and which physics entities touch
    struct CollisionEvent
    {
        /// \brief type of collision event
        enum CollisionEventType : bool
        {
            /// \brief shapes start to touch, they are now in contact
            CONTACT_START = true,

            /// \brief shapes cease to touch, they are now no longer contacting
            CONTACT_END = false,
        };

        /// \brief event type
        CollisionEventType type;

        /// \brief first shape
        CollisionShape* shape_a;

        /// \brief second shape
        CollisionShape* shape_b;
    };

    /// \brief high-level geometric queries in a physics world
    class CollisionHandler
    {
        friend class CollisionShape;

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

            /// \brief pop an event from the event queue, thread-safe. The event queue is automatically cleared every ts::PhysicsWorld::step
            /// \returns event
            CollisionEvent next_event();

            /// \brief clear the event queue
            void clear_events();

        private:
            PhysicsWorld* _world;

            std::mutex _queue_lock;
            std::deque<CollisionEvent> _event_queue;

            struct ContactListener : public b2ContactListener
            {
                ContactListener(CollisionHandler*);

                void BeginContact(b2Contact*) override;
                void EndContact(b2Contact*) override;

                void PreSolve(b2Contact*, const b2Manifold*) override;
                void PostSolve(b2Contact*, const b2ContactImpulse*) override;

                CollisionHandler* _handler;
            };
            ContactListener _contact_listener;
    };
}