// 
// Copyright 2022 Clemens Cords
// Created on 5/30/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/vector.hpp>

namespace ts
{
    /// \brief triangle, basic building block of 2d shapes
    struct Triangle
    {
        /// \brief vertex positions
        Vector2f a, b, c;
    };

    /// \brief trapezoid, arbitrary non-overlapping shape with 4 vertices
    struct Trapezoid
    {
        /// \brief vertex positions
        Vector2f top_left,
                 top_right,
                 bottom_left,
                 bottom_right;
    };

    /// \brief axis-aligned rectangle
    struct Rectangle
    {
        /// \brief upper left vertex
        Vector2f top_left;

        /// \brief width, height
        Vector2f size;
    };

    /// \brief circle
    struct Circle
    {
        /// \brief centroid
        Vector2f center;

        /// \brief radius
        float radius;
    };
}