// 
// Copyright 2022 Joshua Higginbotham
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
    };

    //
    template<typename T>
    struct Vector5
    {
        //
        T x, y, z, u, v;
    };

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

    using Vector4f = Vector4<float>;
    using Vector4i = Vector4<int64_t>;
    using Vector4ui = Vector4<uint64_t>;

    using Vector5f = Vector5<float>;
    using Vector5i = Vector5<int64_t>;
    using Vector5ui = Vector5<uint64_t>;
}