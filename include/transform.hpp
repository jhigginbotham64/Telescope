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
    /// \brief affine transform
    struct Transform
    {
        public:
            //
            Transform();

            //
            Transform(glm::mat3x3);

            //
            Vector2f apply_to(Vector2f);

            //
            void reset();

            //
            void combine(const Transform&);

            //
            void translate(float x, float y);

            //
            void rotate(Angle, Vector2f origin = Vector2f{0, 0});

            //
            void scale(float x, float y);

            //
            void shear(float x_offset, float y_offset);

            //
            void reflect(bool about_x_axis, bool about_y_axis);

            //
            glm::mat3x3& get_native();

        private:
            glm::mat3x3 _matrix;
    };
}