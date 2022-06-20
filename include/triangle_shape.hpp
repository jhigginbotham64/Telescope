// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <array>

#include <include/shape.hpp>
#include <include/geometric_shapes.hpp>

namespace ts
{
    /// \brief shape with 3 vertices
    class TriangleShape : public Shape
    {
        public:
            // no docs
            virtual ~TriangleShape() = default;

            /// \brief construct from vertex positions
            /// \param a: position of the first vertex
            /// \param b: position of the second vertex
            /// \param c: position of the third vertex
            TriangleShape(Vector2f a, Vector2f b, Vector2f c);

            /// \brief construct from geometric triangle
            /// \param triangle
            TriangleShape(Triangle);

        private:
            std::array<Vector2f, 3> _coordinates;
    };
}