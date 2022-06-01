// 
// Copyright 2022 Clemens Cords
// Created on 6/1/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/renderable.hpp>

namespace ts
{
    //
    struct RenderTarget
    {
        //
        virtual void render(const Renderable* object, Transform transform = ts::Transform()) = 0;

        //
        virtual SDL_Renderer* get_renderer() = 0;
    };
}