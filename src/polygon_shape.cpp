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
        // compute convex hull
        // sort by angle around centroid, then decompose into tris

        if (positions.size() < 3)
            throw std::invalid_argument("In PolygonShape::PolygonShape: polygons have to have to have at least 3 vertices.");





        /*
        if (positions.size() < 3)
            throw std::invalid_argument("In PolygonShape::PolygonShape: polygons have to have to have at least 3 vertices.");

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

        _vertices.clear();
        _vertices.reserve(per_angle.size() + 1);
        for (int i = -1; i < per_angle.size(); ++i)
        {
            Vector2f position;
            if (i == -1)
                position = centroid;
            else
                position = per_angle.at(i).first;

            _vertices.push_back(SDL_Vertex());
            _vertices.back().position.x = position.x;
            _vertices.back().position.y = position.y;

            _vertices.back().color.r = 255;
            _vertices.back().color.g = 255;
            _vertices.back().color.b = 255;
            _vertices.back().color.a = 255;

            _vertices.back().tex_coord.x = position.x - top_left.x;
            _vertices.back().tex_coord.y = position.y - top_left.y;
        }

        _vertex_indices.clear();
        for (size_t i = 2; i < _vertices.size(); ++i)
        {
            _vertex_indices.push_back(0);
            _vertex_indices.push_back(i-1);
            _vertex_indices.push_back(i);
        }

        _vertex_indices.push_back(0);
        _vertex_indices.push_back(_vertices.size() - 1);
        _vertex_indices.push_back(1);

        Shape::signal_vertices_updated();
         */
    }

    void PolygonShape::set_centroid(Vector2f new_centroid)
    {
        auto offset = new_centroid - get_centroid();

        for (auto& v : _vertices)
        {
            v.position.x += offset.x;
            v.position.y += offset.y;
        }
    }
}
