//
// Copyright 2022 Clemens Cords
// Created on 6/2/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/vector.hpp>
#include <include/angle.hpp>
#include <include/transform.hpp>
#include <include/geometric_shapes.hpp>

namespace ts
{
    class Window;

    //
    struct Camera
    {
        public:
            //
            Camera(Window*);

            //
            void center_on(Vector2f);

            //
            void move(float x, float y);

            //
            void zoom_in(float factor);

            //
            void zoom_out(float factor);

            //
            void set_zoom(float factor);

            //
            void rotate(Angle);

            //
            void set_rotation(Angle);

            //
            Transform& get_transform();

            //
            Vector2f get_center() const;

            //
            Trapezoid get_view_area() const;

        private:
            Window* _window;
    };
}