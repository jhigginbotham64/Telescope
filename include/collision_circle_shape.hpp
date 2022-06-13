// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/9/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <include/circle_shape.hpp>
#include <include/collision_circle.hpp>

namespace ts
{
    /// \brief debug shape, a renderable circle with a hitbox the same size as the render shape
    class CollisionCircleShape : public CircleShape, public CollisionCircle
    {
        public:
            // no docs
            virtual ~CollisionCircleShape() = default;

            /// \brief create
            /// \param world: physics world
            /// \param center: position of the centroid of the circle
            /// \param radius: radius of the circle
            CollisionCircleShape(PhysicsWorld*, CollisionType, Vector2f center, float radius);

            /// \brief synchronize the shapes position with the hitbox' position
            void update();
    };
}