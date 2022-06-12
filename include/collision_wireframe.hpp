// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include "collision_shape.hpp"

namespace ts
{
    /// \brief collision shape: connected set of lines, closes in a loop
    class CollisionWireFrame : public CollisionShape
    {
        public:
            // no docs
            virtual ~CollisionWireFrame() = default;

            /// \brief create from position of vertices, the chain will automatically connect to become a loop
            /// \param vertices: vector of vertex positions
            CollisionWireFrame(PhysicsWorld* world, CollisionType type, const std::vector<Vector2f>&);

        protected:
            b2Shape* get_native_shape() override;

        private:
            b2ChainShape _shape;
    };
}