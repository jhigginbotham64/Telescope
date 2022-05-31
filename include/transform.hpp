// 
// Copyright 2022 Clemens Cords
// Created on 6/1/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <glm/glm.hpp>

#include <include/angle.hpp>

namespace ts
{
    //
    struct Transform : public glm::mat3x3
    {
        public:
            // https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fwww.alanzucconi.com%2Fwp-content%2Fuploads%2F2016%2F02%2F2D_affine_transformation_matrix.svg_.png&f=1&nofb=1

            //
            Transform();

            //
            void reset();

            //
            void combine(Transform&);

            //
            void translate(float x, float y);

            //
            void rotate(Angle);

            //
            void scale(float x, float y);

            //
            void shear(float x_offset, float y_offset);

            //
            void reflect(bool about_x_axis, bool about_y_axis);
    };
}