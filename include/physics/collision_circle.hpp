// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_shape.hpp>

namespace ts
{
    /// \brief collision shape: a perfect circle
    class CollisionCircle : public CollisionShape
    {
        public:
            /// \brief construct from center and radius
            /// \param center: world coordinates of the center
            /// \param radius: radius of the circle
            CollisionCircle(Vector2f center, float radius);

            /// \brief construct from ts::Circle
            /// \param circle: provides center and radius
            CollisionCircle(Circle);

            /// \brief construct from ts::CircleShape
            /// \param circle: provides center and radius
            /// \note unlike ts::CircleShape, ts::CollisionShape is simulated as if it had an infinite number of outer vertices
            CollisionCircle(const CircleShape&);

        protected:
            b2Shape* get_native_shape() override;

        private:
            b2CircleShape _shape;
    };
}