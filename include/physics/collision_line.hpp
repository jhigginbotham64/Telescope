// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_shape.hpp>

namespace ts
{
    //
    class CollisionLine : public CollisionShape
    {
        public:
            // chain shape without loop
            CollisionLine(const std::vector<Vector2f>&);

            //
            CollisionLine(Vector2f a, Vector2f b);

        private:
            b2EdgeShape _shape;
    };
}