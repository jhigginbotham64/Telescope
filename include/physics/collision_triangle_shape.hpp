// 
// Copyright 2022 Clemens Cords
// Created on 6/9/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/triangle_shape.hpp>
#include <include/physics/collision_polygon.hpp>

namespace ts
{
    //
    class CollisionTriangleShape : public TriangleShape, public CollisionPolygon
    {
        public:
            //
            CollisionTriangleShape(PhysicsWorld*, PhysicsObjectType, Vector2f a, Vector2f b, Vector2f c);

            //
            void update();
    };
}