// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <box2d/b2_fixture.h>

namespace ts
{
    // for collision filtering
    enum struct FilterGroup : uint16_t
    {
        NONE = uint16_t(0),
        GROUP_01 = uint16_t(1) << 0,
        GROUP_02 = uint16_t(1) << 1,
        GROUP_03 = uint16_t(1) << 2,
        GROUP_04 = uint16_t(1) << 3,
        GROUP_05 = uint16_t(1) << 4,
        GROUP_06 = uint16_t(1) << 5,
        GROUP_07 = uint16_t(1) << 6,
        GROUP_08 = uint16_t(1) << 7,
        GROUP_09 = uint16_t(1) << 8,
        GROUP_10 = uint16_t(1) << 9,
        GROUP_11 = uint16_t(1) << 10,
        GROUP_12 = uint16_t(1) << 11,
        GROUP_13 = uint16_t(1) << 12,
        GROUP_14 = uint16_t(1) << 13,
        GROUP_15 = uint16_t(1) << 14,
        GROUP_16 = uint16_t(1) << 15,
        ALL = !uint16_t(0)
    };
}