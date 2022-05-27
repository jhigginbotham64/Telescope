// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/shape.hpp>

namespace ts
{
    //
    class TriangleShape : public Shape
    {
        public:
            //
            TriangleShape(Vector2f a, Vector2f b, Vector2f c);
    };
}