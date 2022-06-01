// 
// Copyright 2022 Clemens Cords
// Created on 6/1/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <glm/glm.hpp>

#include <include/vector.hpp>
#include <include/angle.hpp>

namespace ts
{
    /// \brief affine transform in 2d space
    struct Transform
    {
        public:
            /// \brief initialize the transform as identity
            Transform();

            /// \brief initialize the transform from a 3x3 glm matrix
            /// \param matrix
            Transform(glm::mat3x3);

            /// \brief apply the transform to a point in 2d space
            /// \param point
            /// \returns point after transform
            Vector2f apply_to(Vector2f);

            /// \brief reset the transform such that is identity
            void reset();

            /// \brief combine the transform with another transform: this = other * this
            /// \param transform: other transform
            void combine(const Transform&);

            /// \brief translate by an offset in 2d space
            /// \param x: x-offset, negative for screen-left, positive for screen-right
            /// \param y: y-offset, negative for screen-up, positive for screen-down
            void translate(float x, float y);

            /// \brief rotate by an angle around an origin
            /// \param angle: angle, positive for clockwise, negative for counter-clockwise
            /// \param origin: origin to rotate around
            void rotate(Angle, Vector2f origin = Vector2f{0, 0});

            /// \brief set the scale to be applied to a point, multiplies each vertex coordinate by a factor
            /// \param x_scale: scale along x-dimension, 1 for no scale, 0 for invisible
            /// \param y_scale: scale along y-dimension, 1 for no scale, 0 for invisible
            void scale(float x_scale, float y_scale);

            /// \brief set the shear scale
            /// \param x_scale: shear factor along x-dimension, 0 for no shear, positive for screen-right, negative for screen-left
            /// \param x_scale: shear factor along y-dimension, 0 for no shear, positive for screen-up, negative for screen-down
            void shear(float x_scale, float y_scale);

            /// \brief reflect around point
            /// \param about_x_axis: should the transform reflect along the x-axis
            /// \param about_y_axis: should the transform reflect along the y-axis
            /// \param origin: origin to reflect about
            void reflect(bool about_x_axis, bool about_y_axis, Vector2f origin = Vector2f{0, 0});

            /// \brief get a reference to the native glm matrix
            /// \returns matrix
            glm::mat3x3& get_native();

        private:
            glm::mat3x3 _matrix;
    };
}