// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/shape.hpp>

namespace ts
{
    //
    class CircleShape : public Shape
    {
        public:
            //
            CircleShape(Vector2f center, float radius, size_t n_outer_vertices);

            //
            CircleShape(float center_x, float center_y, float radius, size_t n_outer_vertices);

            //
            Vector2f get_center() const;


            //
            void set_center(Vector2f);

            //
            float get_radius() const;

            //
            void set_radius(float);

        private:
            float _radius;
            size_t _n_vertices;
                // number of outer vertices before tri decomposition

            void update(Vector2f center);
    };
}