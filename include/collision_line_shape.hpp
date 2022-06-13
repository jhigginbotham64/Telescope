// 
// Copyright 2022 Clemens Cords
// Created on 6/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/collision_line.hpp>
#include <include/physics_world.hpp>

namespace ts
{
    //
    class CollisionLineShape : public CollisionLine, public RectangleShape
    {
        public:
            //
            CollisionLineShape(PhysicsWorld*, CollisionType, Vector2f a, Vector2f b);

            //
            void update();

        protected:
    };
}