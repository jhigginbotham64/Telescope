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

    /// \brief geometric rectangle
    struct Rectangle
    {
        Vector2f top_left,
                 top_right,
                 bottom_left,
                 bottom_right;
    };
}