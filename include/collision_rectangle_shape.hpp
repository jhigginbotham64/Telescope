// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/9/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <include/rectangle_shape.hpp>
#include <include/collision_polygon.hpp>

namespace ts
{
    /// \brief debug shape that is a rectangle with a hitbox of the same size
    class CollisionRectangleShape : public RectangleShape, public CollisionPolygon
    {
        public:
            // no docs
            virtual ~CollisionRectangleShape() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param top_left: top left coordinate of the shape
            /// \param size: size of the shape
            CollisionRectangleShape(PhysicsWorld*, CollisionType, Vector2f top_left, Vector2f size);

            /// \brief synchronize the shapes position with that of its hitbox
            void update();
    };
}