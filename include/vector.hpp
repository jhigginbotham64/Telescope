// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <glm/glm.hpp>

namespace ts
{
    template<typename T>
    using Vector2 = glm::vec<2, T>;

    //
    using Vector2f = glm::vec<2, float>;

    //
    using Vector2i = glm::vec<2, int>;

    //
    using Vector2ui = glm::vec<2, size_t>;

    //
    template<typename T>
    using Vector3 = glm::vec<3, T>;

    //
    using Vector3f = glm::vec<3, float>;

    //
    using Vector3i = glm::vec<3, int>;

    //
    using Vector3ui = glm::vec<3, size_t>;

    //
    template<typename T>
    using Vector4 = glm::vec<4, T>;

    //
    using Vector4f = glm::vec<4, float>;

    //
    using Vector4i = glm::vec<4, int>;

    //
    using Vector4ui = glm::vec<4, size_t>;
}