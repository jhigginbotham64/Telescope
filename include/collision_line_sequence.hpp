// 
//
// Created on 6/19/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/collision_shape.hpp>

namespace ts
{
    /// \brief collision shape: a sequence of lines
    class CollisionLineSequence : public CollisionShape
    {
        public:
            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type of object
            /// \param a: first point of the line
            /// \param b: second point of the line
            /// \param two_sided: should collision happend from both sides of the line. If false, collision only happens on the outer, clockwise side of the line
            /// \note Lines can only be ts::STATIC or ts::KINEMATIC. If ts::DYNAMIC is provided as the ts::CollisionType, a weak warning is issued
            CollisionLineSequence(PhysicsWorld* world, CollisionType type, const std::vector<Vector2f>&, bool two_sided = true);

        protected:
            b2Shape* get_native_shape() override;

            std::vector<b2Fixture*> _subsequent_fixtures;
        private:
            b2EdgeShape _shape;
    };
}