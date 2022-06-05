// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_shape.hpp>

namespace ts
{
    //
    class CollisionPolygon : public CollisionShape
    {
        public:
            //
            CollisionPolygon(const std::vector<Vector2f>&);

            //
            CollisionPolygon(const TriangleShape&);

            //
            CollisionPolygon(const RectangleShape&);

            //
            CollisionPolygon(const PolygonShape&);

            //
            CollisionPolygon(Triangle);

            //
            CollisionPolygon(Rectangle);

        private:
            b2PolygonShape _shape;
    };
}