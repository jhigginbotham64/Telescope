// 
// Copyright 2022 Clemens Cords
// Created on 6/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/collision_line.hpp>
#include <include/physics_world.hpp>

namespace ts
{
    /// \brief renderable 1-pixel wide line
    class CollisionLineShape : public CollisionLine, public RectangleShape
    {
        public:
            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param a: start of the line
            /// \param b: end of the line
            CollisionLineShape(PhysicsWorld*, CollisionType, Vector2f a, Vector2f b);

            /// \brief synchronize the shapes position with that of its hitbox
            void update();
    };
}