// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_circle.hpp>

namespace ts
{
    CollisionCircle::CollisionCircle(Vector2f center, float radius)
    {
        _shape = b2CircleShape();
        _shape.m_p.Set(center.x, center.y);
        _shape.m_radius = radius;
    }

    CollisionCircle::CollisionCircle(ts::Circle circle)
    {
        _shape = b2CircleShape();
        _shape.m_p.Set(circle.center.x, circle.center.y);
        _shape.m_radius = circle.radius;
    }

    CollisionCircle::CollisionCircle(const CircleShape& shape)
    {
        _shape = b2CircleShape();
        _shape.m_p.Set(shape.get_centroid().x, shape.get_centroid().y);
        _shape.m_radius = shape.get_radius();
    }

    b2Shape* CollisionCircle::get_shape()
    {
        return &_shape;
    }
}