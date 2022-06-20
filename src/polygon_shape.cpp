//
// Copyright 2022 Joshua Higginbotham
// Created on 5/30/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <stdexcept>
#include <algorithm>

#include <include/polygon_shape.hpp>
#include <include/angle.hpp>

namespace ts
{
    PolygonShape::PolygonShape(const std::vector<Vector2f>& positions)
    {
        if (positions.size() < 3)
            throw std::invalid_argument("In PolygonShape::PolygonShape: polygons have to have to have at least 3 vertices.");

        auto center = Vector2f(0, 0);
        for (auto& pos : positions)
            center += pos;

        size_t n = positions.size();
        center /= Vector2f(n, n);

        float min_x = std::numeric_limits<float>::max();
        float min_y = std::numeric_limits<float>::max();
        float max_x = std::numeric_limits<float>::min();
        float max_y = std::numeric_limits<float>::min();

        std::vector<std::pair<Vector2f, Angle>> by_angle;
        for (auto& pos : positions)
        {
            min_x = std::min(min_x, pos.x);
            min_y = std::min(min_y, pos.y);
            max_x = std::max(max_x, pos.x);
            max_y = std::max(max_y, pos.y);

            by_angle.emplace_back(pos, radians(std::atan2(pos.x - center.x, pos.y - center.y)));
        }

        std::sort(by_angle.begin(), by_angle.end(), [](const std::pair<Vector2f, Angle>& a, const std::pair<Vector2f, Angle>& b)
        {
            return a.second.as_degrees() < b.second.as_degrees();
        });

        _vertices.clear();
        _vertices.reserve(positions.size() + 1);

        _vertices.emplace_back();
        _vertices.back().position.x = center.x;
        _vertices.back().position.y = center.y;

        for (auto& pair : by_angle)
        {
            _vertices.emplace_back();
            _vertices.back().position.x = pair.first.x;
            _vertices.back().position.y = pair.first.y;
        }

        for (auto& v : _vertices)
        {
            v.color.r = 255;
            v.color.g = 255;
            v.color.b = 255;
            v.color.a = 255;
        }

        _vertex_indices.clear();

        for (size_t i = 1; i < _vertices.size()-1; ++i)
        {
            _vertex_indices.push_back(0);
            _vertex_indices.push_back(i);
            _vertex_indices.push_back(i+1);
        }

        _vertex_indices.push_back(0);
        _vertex_indices.push_back(1);
        _vertex_indices.push_back(_vertices.size() - 1);

        signal_vertices_updated();
    }
}
