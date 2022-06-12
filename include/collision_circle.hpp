// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include "collision_shape.hpp"

namespace ts
{
    /// \brief collision shape: a perfect circle
    class CollisionCircle : public CollisionShape
    {
        public:
            // no docs
            virtual ~CollisionCircle() = default;

            /// \brief construct from center and radius
            /// \param center: world coordinates of the center
            /// \param radius: radius of the circle
            CollisionCircle(PhysicsWorld* world, CollisionType, Vector2f center, float radius);

            /// \brief construct from ts::Circle
            /// \param circle: provides center and radius
            CollisionCircle(PhysicsWorld* world, CollisionType, Circle);

            /// \brief construct from ts::CircleShape
            /// \param circle: provides center and radius
            /// \note unlike ts::CircleShape, ts::CollisionShape is simulated as if it had an infinite number of outer vertices
            CollisionCircle(PhysicsWorld* world, CollisionType, const CircleShape&);

        protected:
            b2Shape* get_native_shape() override;

        private:
            b2CircleShape _shape;
    };
}