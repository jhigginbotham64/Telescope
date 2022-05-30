//
// Copyright 2022 Clemens Cords
// Created on 5/30/22 by clem (mail@clemens-cords.com)
//

#include <stdexcept>

#include <include/polygon_shape.hpp>

namespace ts
{
    PolygonShape::PolygonShape(std::vector<Vector2f> positions)
    {
        throw std::invalid_argument("TODO: implement tri decomposition for non-convex polygons");
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
