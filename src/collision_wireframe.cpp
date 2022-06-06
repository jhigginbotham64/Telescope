// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_wireframe.hpp>

namespace ts
{
    b2Shape* CollisionWireFrame::get_shape()
    {
        return &_shape;
    }

    CollisionWireFrame::CollisionWireFrame(const std::vector<Vector2f>& vec)
    {
        _shape = b2ChainShape();

        std::vector<b2Vec2> points;
        points.reserve(vec.size());

        for (auto& v : vec)
            points.emplace_back(v.x, v.y);

        _shape.CreateLoop(points.data(), points.size());
    }
}