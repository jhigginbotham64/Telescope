// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/9/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include "rectangle_shape.hpp"
#include "collision_polygon.hpp"

namespace ts
{
    //
    class CollisionRectangleShape : public RectangleShape, public CollisionPolygon
    {
        public:
            // no docs
            virtual ~CollisionRectangleShape() = default;

            //
            CollisionRectangleShape(PhysicsWorld*, CollisionType, Vector2f top_left, Vector2f size);

            //
            void update();
    };
}