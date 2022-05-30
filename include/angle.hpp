//
// Copyright 2022 Clemens Cords
// Created on 29.05.22 by clem (mail@clemens-cords.com)
//

#include <cinttypes>

namespace ts
{
    //
    struct Angle
    {
        public:
            //
            float as_degrees() const;

            //
            float as_radians() const;

        private:
            Angle(float dg);
            float _degrees;

            friend Angle degrees(int);
            friend Angle radians(float);
    };

    //
    Angle degrees(int);

    //
    Angle radians(float);
}