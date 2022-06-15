// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <mutex>
#include <deque>

#include <box2d/b2_world.h>

#include <include/vector.hpp>
#include <include/time.hpp>

#undef b2_maxPolygonVertices
#define b2_maxPolygonVertices 32

namespace ts
{
    class Window;
    class CollisionShape;

    /// \brief object returned by ts::PhysicsWorld::ray_cast
    struct RayCastInformation
    {
        /// \brief do the ray and the shape overlap at all
        bool are_colliding;

        /// \brief normal vector of the intersection point (if any)
        Vector2f normal_vector; // from a to b

        /// \brief earliest contact point with the ray (if any)
        Vector2f contact_point;
    };

    /// \brief object returned by ts::PhysicsWorld::distance_between
    struct DistanceInformation
    {
        /// \brief the distance, in world units (1 pixel if the world transform is identity)
        float distance;

        /// \brief the two closest points of shape a and b respectively
        std::pair<Vector2f, Vector2f> closest_points;
    };

    /// \brief collision event, reports when and which physics entities touch
    struct CollisionEvent
    {
        CollisionEvent() = default;

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

    /// \brief world instance, contains all physics objects. Only objects within the same world can interact
    class PhysicsWorld
    {
        public:
            /// \brief construct, gravity will be 0 in both directions
            PhysicsWorld();

            /// \brief destroy, the user is responsible for safely allocating any object referencing this world
            ~PhysicsWorld();

            /// \brief advance the physics simulation
            /// \param timestep: time to advance
            /// \param velocity_iterations: iterations, dictate velocity step resolution
            /// \param position_iterations: iterations, dictate velocity step resolution
            void step(Time timestep, int32_t velocity_iterations = 8, int32_t position_iterations = 3);

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
            /// \param event: [out] event, will be modified if event queue is not empty
            /// \returns true if event queue was non-empty and the input event pointer was updated, false otherwise
            bool next_event(CollisionEvent*);

            /// \brief clear the event queue
            void clear_events();

            /// \brief set all forces in the world to zero
            void clear_forces();

            /// \brief get the global gravity
            Vector2f get_gravity() const;

            /// \brief set the global gravity
            /// \param force_vector: where +x: screen-right, -x: screen-left, +y: screen-down, -y: screen-up
            void set_gravity(Vector2f);

            /// \brief access the native box2d world
            /// \returns pointer to world
            b2World* get_native();

            /// no docs
            static inline const float pixel_ratio = 1000;

            // no docs
            Vector2f world_to_native(Vector2f) const;

            // no docs
            Vector2f native_to_world(Vector2f) const;

            // no docs
            float get_skin_radius() const;

        private:
            static inline b2Vec2 _default_gravity = {0, 0};
            b2World _world;

            std::mutex _queue_lock;
            std::deque<CollisionEvent> _event_queue;

            struct ContactListener : public b2ContactListener
            {
                ContactListener(PhysicsWorld*);

                void BeginContact(b2Contact*) override;
                void EndContact(b2Contact*) override;

                void PreSolve(b2Contact*, const b2Manifold*) override;
                void PostSolve(b2Contact*, const b2ContactImpulse*) override;

                PhysicsWorld* _world;
            };
            ContactListener _contact_listener;
    };
}