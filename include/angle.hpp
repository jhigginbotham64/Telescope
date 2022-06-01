//
// Copyright 2022 Clemens Cords
// Created on 29.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <cinttypes>

namespace ts
{
    /// \brief class representing a geometric angle
    struct Angle
    {
        public:
            /// \brief convert to degrees
            /// \returns float, in [0, 360]
            float as_degrees() const;

            /// \brief convert to radians
            /// \returns float, in [pi, 2pi]
            float as_radians() const;

        private:
            Angle(float dg);
            float _degrees;

            friend Angle degrees(float);
            friend Angle radians(float);
    };

    /// \brief construct angle in degree
    Angle degrees(float);

    /// \brief construct angle in radian
    Angle radians(float);
}