// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <box2d/b2_world.h>

#include <include/vector.hpp>
#include <include/time.hpp>

namespace ts
{
    class PhysicsObject;
    class CollisionListener;

    //
    class PhysicsWorld
    {
        public:
            PhysicsWorld();

            //
            void step(Time timestep, int32_t velocity_iterations);

            //
            void clear_forces();

            //
            Vector2f get_gravity() const;

            //
            void set_gravity(Vector2f);

            //
            b2World* get_native();

        private:
            static inline b2Vec2 _default_gravity = {0, 0};
            b2World _world;

    };
}