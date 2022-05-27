// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/render_target.hpp>

namespace ts
{
    //
    struct Renderable
    {
        public:
            //
            virtual void render(RenderTarget& target) = 0;
    };
}