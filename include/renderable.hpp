// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

namespace ts
{
    class RenderTarget;

    //
    struct Renderable
    {
        public:
            //
            virtual void render(const RenderTarget* target) const = 0;
    };
}