// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#include <include/render_target.hpp>
#include <include/window.hpp>
#include <include/renderable.hpp>

namespace ts
{
    RenderTarget::RenderTarget()
        : _window(nullptr), _renderer(nullptr)
    {}

    RenderTarget::~RenderTarget()
    {
        SDL_DestroyRenderer(_renderer);
    }

    SDL_Renderer * RenderTarget::get_renderer() const
    {
        return _renderer;
    }

    void RenderTarget::create(Window* window)
    {
        _window = window;
        _renderer = SDL_CreateRenderer(window->get_native(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    }

    void RenderTarget::render(Renderable* renderable) const
    {
        renderable->render(this);
    }
}

