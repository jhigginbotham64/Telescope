//
// Created by clem on 6/1/22.
//

#include <iostream> // TODO

#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>

#include <include/transform.hpp>

namespace ts
{
    Transform::Transform()
        : _matrix()
    {
        reset();
    }

    Transform::Transform(glm::mat3x3 m)
    {
        for (size_t x = 0; x < 3; ++x)
            for (size_t y = 0; y < 3; ++y)
                _matrix[x][y] = m[x][y];
    }

    Vector2f Transform::apply_to(Vector2f in)
    {
        auto result = (_matrix * glm::vec3(in.x, in.y, 1));
        return Vector2f{result.x, result.y};
    }

    void Transform::combine(const Transform & other)
    {
        this->_matrix = other._matrix * this->_matrix;
    }

    void Transform::reset()
    {
        _matrix[0][0] = 1;
        _matrix[0][1] = 0;
        _matrix[0][2] = 0;

        _matrix[1][0] = 0;
        _matrix[1][1] = 1;
        _matrix[1][2] = 0;

        _matrix[2][0] = 0;
        _matrix[2][1] = 0;
        _matrix[2][2] = 1;
    }

    void Transform::translate(float x, float y)
    {
        _matrix[2][0] += x;
        _matrix[2][1] += y;
    }

    void Transform::scale(float x, float y)
    {
        combine(Transform({
            x, 0, 0,
            0, y, 0,
            0, 0, 1
        }));
    }

    void Transform::shear(float x_scale, float y_scale)
    {
        combine(Transform({
            1, y_scale, 0,
            x_scale, 1, 0,
            0, 0, 1
        }));
    }

    void Transform::rotate(Angle angle, Vector2f origin)
    {
        float rad = angle.as_radians();
        float cos = std::cos(rad);
        float sin = std::sin(rad);

        combine(Transform({
            cos, -sin, 0,
            sin,  cos, 0,
            origin.x * (1 - cos) - origin.y * sin,    origin.y * (1 - cos) + origin.x * sin,   1
        }));
    }

    void Transform::reflect(bool about_x_axis, bool about_y_axis, Vector2f origin)
    {
        auto x = about_x_axis ? -1 : 1;
        auto y = about_y_axis ? -1 : 1;

        translate(-origin.x, -origin.y);
        combine(Transform({
            x, 0, 0,
            0, y, 0,
            0, 0, 1
        }));
        translate(origin.x, origin.y);
    }

    glm::mat3x3 &Transform::get_native()
    {
        return _matrix;
    }
}
