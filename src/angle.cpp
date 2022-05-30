//
// Created by clem on 5/30/22.
//

#include <include/angle.hpp>
#include <cmath>

namespace ts
{
    Angle::Angle(float dg)
        : _degrees(fmod(dg, 360))
    {}

    float Angle::as_radians() const
    {
        return _degrees * M_PI / 180;
    }

    float Angle::as_degrees() const
    {
        return _degrees;
    }

    Angle radians(float value)
    {
        return Angle(value * 180 / M_PI);
    }

    Angle degrees(float value)
    {
        return Angle(value);
    }
}
