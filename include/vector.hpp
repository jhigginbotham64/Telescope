// 
// Copyright 2022 Clemens Cords
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <cstdint>

namespace ts
{
    //
    template<typename T>
    struct Vector2
    {
        //
        T x, y;
    };

    //
    template<typename T>
    struct Vector3
    {
        //
        T x, y, z;
    };

    //
    template<typename T>
    struct Vector4
    {
        //
        T x, y, z, u;
    }

    // typedefs:

    using Vector2f = Vector2<float>;
    using Vector2i = Vector2<int64_t>;
    using Vector2ui = Vector2<uint64_t>;

    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int64_t>;
    using Vector3ui = Vector3<uint64_t>;

    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int64_t>;
    using Vector3ui = Vector3<uint64_t>;
}