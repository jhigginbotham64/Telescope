//
// Copyright 2022 Clemens Cords
// Created on 5/30/22 by clem (mail@clemens-cords.com)
//

#include <stdexcept>
#include <algorithm>
#include <iostream> //TODO

#include <include/polygon_shape.hpp>
#include <include/angle.hpp>

namespace ts
{
    PolygonShape::PolygonShape(const std::vector<Vector2f>& positions)
    {
        // compute convex hull
        // sort by angle around centroid, then decompose into tris

        Vector2f centroid = {0, 0};
        Vector2f top_left = Vector2f(std::numeric_limits<float>::max());

        for (auto& v : positions)
        {
            centroid += v;
            top_left.x = std::min(top_left.x, v.x);
            top_left.y = std::min(top_left.y, v.y);
        }

        centroid = centroid / Vector2f{positions.size(), positions.size()};

        std::vector<std::pair<Vector2f, Angle>> per_angle;

        for (auto v : positions)
        {
            auto at_origin = centroid - v;
            per_angle.push_back({v, ts::radians(std::atan2(at_origin.x, at_origin.y))});
        }

        std::sort(per_angle.begin(), per_angle.end(), [](auto a, auto b) -> bool {
            return a.second.as_degrees() < b.second.as_degrees();
        });

        HSVA temp_color = HSVA(0, 1, 1, 0.33); //TODO

        static auto push_vertex = [&](Vector2f position) -> void
        {
            std::cout << position.x << " " << position.y << std::endl;

            _vertices.emplace_back();

            _vertices.back().position.x = position.x;
            _vertices.back().position.y = position.y;

            _vertices.back().color.r = 255;
            _vertices.back().color.g = 255;
            _vertices.back().color.b = 255;
            _vertices.back().color.a = 255;

            _vertices.back().tex_coord.x = position.x - top_left.x;
            _vertices.back().tex_coord.y = position.y - top_left.y;
        };

        _vertices.clear();
        push_vertex(centroid);
        push_vertex(per_angle.at(0).first);
        push_vertex(per_angle.at(1).first);

        for (size_t i = 1; i < per_angle.size(); ++i)
        {
            push_vertex(centroid);
            push_vertex(per_angle.at(i-1).first);
            push_vertex(per_angle.at(i).first);
        }

        push_vertex(centroid);
        push_vertex(per_angle.back().first);
        push_vertex(per_angle.front().first);

        // TODO
        float step = 1.0 / _vertices.size();
        for (auto& v : _vertices)
        {
            v.color = temp_color.as_rgb().operator SDL_Color();
            temp_color.hue += step;
        }
    }

    Vector2f PolygonShape::get_centroid() const
    {
        auto aabb = get_bounding_box();
        return aabb.top_left + (aabb.size / Vector2f{2, 2});
    }

    void PolygonShape::set_centroid(Vector2f new_centroid)
    {
        auto offset = get_centroid() - new_centroid;

        for (auto& v : _vertices)
        {
            v.position.x += offset.x;
            v.position.y += offset.y;
        }
    }
}
