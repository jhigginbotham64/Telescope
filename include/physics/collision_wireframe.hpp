// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_shape.hpp>

namespace ts
{
    //
    class CollisionWireFrame : public CollisionShape
    {
        public:
            // chain line with loop
            CollisionWireFrame(const std::vector<Vector2f>&);

        private:
            b2ChainShape _shape;
    };
}