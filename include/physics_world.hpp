// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include "../../../../../usr/include/box2d/b2_world.h"

#include "vector.hpp"
#include "time.hpp"

#undef b2_maxPolygonVertices
#define b2_maxPolygonVertices 16

namespace ts
{
    class PhysicsObject;
    class CollisionListener;

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

        private:
            static inline b2Vec2 _default_gravity = {0, 0};
            b2World _world;

    };
}