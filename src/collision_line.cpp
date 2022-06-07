// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics/collision_line.hpp>

namespace ts
{
    CollisionLine::CollisionLine(Vector2f a, Vector2f b)
    {
        _shape = b2EdgeShape();
        _shape.SetTwoSided(b2Vec2(a.x, a.y), b2Vec2(b.x, b.y));
    }

    b2Shape* CollisionLine::get_native_shape()
    {
        return &_shape;
    }
}