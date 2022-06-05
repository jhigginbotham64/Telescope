// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_shape.hpp>

namespace ts
{
    //
    class CollisionCircle : public CollisionShape
    {
        public:
            //
            CollisionCircle(Vector2f center, float radius, size_t n_outer_vertices);

            //
            CollisionCircle(Circle);

            //
            CollisionCircle(const CircleShape&);

        private:
            b2CircleShape _b2_shape;
    };
}