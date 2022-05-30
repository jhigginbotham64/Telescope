// 
// Copyright 2022 Clemens Cords
// Created on 5/30/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/shape.hpp>

namespace ts
{
    class PolygonShape : public Shape
    {
        public:
            //
            PolygonShape(std::vector<Triangle> tris);

            // automatic triangle decomposition
            //PolygonShape(std::vector<Vector2f> positions);
    };
}