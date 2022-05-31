// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <array>

#include <include/shape.hpp>

namespace ts
{
    //
    class TriangleShape : public Shape
    {
        public:
            //
            TriangleShape(Vector2f a, Vector2f b, Vector2f c);

            //
            void set_centroid(Vector2f) override;

            //
            Vector2f get_centroid() const override;

        private:
            void update();

            Vector2f _centroid;
            std::array<Vector2f, 3> _coordinates;
    };
}