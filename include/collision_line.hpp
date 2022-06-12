// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include "collision_shape.hpp"

namespace ts
{
    /// \brief collision shape: a 2-point line
    class CollisionLine : public CollisionShape
    {
        public:
            // no docs
            virtual ~CollisionLine() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type of object
            /// \param a: first point of the line
            /// \param b: second point of the line
            /// \param two_sided: should collision happend from both sides of the line. If false, collision only happens on the outer, clockwise side of the line
            CollisionLine(PhysicsWorld* world, CollisionType type, Vector2f a, Vector2f b, bool two_sided = true);

        protected:
            b2Shape* get_native_shape() override;

        private:
            b2EdgeShape _shape;
    };
}