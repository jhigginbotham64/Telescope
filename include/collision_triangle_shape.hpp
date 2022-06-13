// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/9/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <include/triangle_shape.hpp>
#include <include/collision_polygon.hpp>

namespace ts
{
    /// \brief triangle shape with identically sized hitbox
    class CollisionTriangleShape : public TriangleShape, public CollisionPolygon
    {
        public:
            // no docs
            virtual ~CollisionTriangleShape() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param a: first vertex position
            /// \param b: second vertex position
            /// \param c: thrid vertex position
            CollisionTriangleShape(PhysicsWorld*, CollisionType, Vector2f a, Vector2f b, Vector2f c);

            /// \brief synchronize the position of the shape with that of the hitbox
            void update();
    };
}