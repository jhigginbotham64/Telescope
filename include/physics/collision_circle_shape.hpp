// 
// Copyright 2022 Clemens Cords
// Created on 6/9/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/circle_shape.hpp>
#include <include/physics/collision_circle.hpp>

namespace ts
{
    //
    class CollisionCircleShape : public CircleShape, public CollisionCircle
    {
        public:
            //
            CollisionCircleShape(PhysicsWorld*, PhysicsObjectType, Vector2f center, float radius);

            //
            void update();
    };
}