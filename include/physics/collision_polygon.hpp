// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_shape.hpp>

namespace ts
{
    /// \param collision shape: convex polygon
    class CollisionPolygon : public CollisionShape
    {
        public:
            /// \brief virtual destructor
            virtual ~CollisionPolygon() = default;

            /// \brief construct from list of vertices
            /// \param vertices: vector of world vertex positions
            CollisionPolygon(PhysicsWorld*, CollisionType, const std::vector<Vector2f>&);

            /// \brief construct from ts::TriangleShape
            /// \param triangle: supplies 3 vertices
            CollisionPolygon(PhysicsWorld*, CollisionType, const TriangleShape&);

            /// \brief construct from ts::RectangleShape
            /// \param rectangle: supplies 4 vertices
            CollisionPolygon(PhysicsWorld*, CollisionType, const RectangleShape&);

            /// \brief constructo from ts::PolygonShape
            /// \param polygon: supplies vertices, outer convex hull will be computed independently of the polygons total number of vertices
            CollisionPolygon(PhysicsWorld*, CollisionType, const PolygonShape&);

            /// \brief construct from ts::Triangle
            /// \param triangle: supplies 3 vertex positions
            CollisionPolygon(PhysicsWorld*, CollisionType, Triangle);

            /// \brief construct from ts::Rectangle
            /// \param rectangle: supplies 4 vertex positions
            CollisionPolygon(PhysicsWorld*, CollisionType, Rectangle);

        protected:
            b2Shape* get_native_shape() override;

        private:
            b2PolygonShape _shape;
    };
}