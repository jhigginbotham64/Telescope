// 
// Copyright 2022 Clemens Cords
// Created on 6/9/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/rectangle_shape.hpp>
#include <include/physics/collision_polygon.hpp>

namespace ts
{
    //
    class CollisionRectangleShape : public RectangleShape, public CollisionPolygon
    {
        public:
            //
            CollisionRectangleShape(PhysicsWorld*, PhysicsObjectType, Vector2f top_left, Vector2f size);

            //
            void update();
    };
}