// 
// Copyright 2022 Clemens Cords
// Created on 25.05.22 by clem (mail@clemens-cords.com)
//

#include <string>

#include <include/logging.hpp>
#include <include/color.hpp>

namespace ts
{
    RGBA::RGBA()
        : RGBA(Vector4f(0, 0, 0, 1))
    {}

    RGBA::RGBA(float r, float g, float b, float a)
        : RGBA(Vector4f(r, g, b, a))
    {}

    RGBA::RGBA(Vector4f in)
        : red(glm::clamp<float>(in.r, 0, 1)),
          green(glm::clamp<float>(in.g, 0, 1)),
          blue(glm::clamp<float>(in.b, 0, 1)),
          alpha(glm::clamp<float>(in.a, 0, 1))
    {}

    RGBA::operator Vector4f()
    {
        return Vector4f(red, green, blue, alpha);
    }

    HSVA::HSVA()
        : HSVA(Vector4f(0, 0, 0, 1))
    {}

    HSVA::HSVA(float h, float s, float v, float a)
        : HSVA(Vector4f(h, s, v, a))
    {}

    HSVA::HSVA(Vector4f in)
        : hue(glm::clamp<float>(in.x, 0, 1)),
          saturation(glm::clamp<float>(in.y, 0, 1)),
          value(glm::clamp<float>(in.z, 0, 1)),
          alpha(glm::clamp<float>(in.a, 0, 1))
    {}

    HSVA::operator Vector4f()
    {
        return Vector4f(hue, saturation, value, alpha);
    }

    // source: https://github.com/Clemapfel/crisp/blob/main/.src/color.inl

    HSVA RGBA::as_hsv() const
    {
        float h, s, v;

        auto min = red < green ? red : green;
        min = min  < blue ? min  : blue;

        auto max = red > green ? red : green;
        max = max  > blue ? max  : blue;

        auto delta = max - min;

        if (delta > 0)
        {
            if (max == red)
                h = 60 * (fmod(((green - blue) / delta), 6));

            else if (max == green)
                h = 60 * (((blue - red) / delta) + 2);

            else if (max == blue)
                h = 60 * (((red - green) / delta) + 4);

            if (max > 0)
                s = delta / max;
            else
                s = 0;

            v = max;
        }
        else
        {
            h = 0;
            s = 0;
            v = max;
        }

        if (h < 0)
            h += 360;

        auto out = HSVA();
        out.hue = h / 360.f;
        out.saturation = s;
        out.value = v;
        out.alpha = alpha;
        return out;
    }

    RGBA HSVA::as_rgb() const
    {
        auto h = hue;

        h *= 360;
        float c = value * saturation;
        float h_2 = h / 60;
        float x = c * (1 - std::fabs(std::fmod(h_2, 2) - 1));

        Vector3f rgb;

        if (0 <= h_2 and h_2 < 1)
        {
            rgb = Vector3f(c, x, 0);
        }
        else if (1 <= h_2 and h_2 < 2)
        {
            rgb = Vector3f(x, c, 0);
        }
        else if (2 <= h_2 and h_2 < 3)
        {
            rgb = Vector3f(0, c, x);
        }
        else if (3 <= h_2 and h_2 < 4)
        {
            rgb = Vector3f(0, x, c);
        }
        else if (4 <= h_2 and h_2 < 5)
        {
            rgb = Vector3f(x, 0, c);
        }
        else if (5 <= h_2 and h_2 <= 6)
        {
            rgb = Vector3f(c, 0, x);
        }

        auto m = value - c;

        rgb.r += m;
        rgb.g += m;
        rgb.b += m;

        return RGBA(Vector4f(rgb.r, rgb.g, rgb.b, alpha));
    }
}

