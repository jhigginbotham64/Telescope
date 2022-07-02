//
// Created by clem on 5/31/22.
//

#include <SDL2/SDL_image.h>

#include <include/window.hpp>
#include <include/texture.hpp>
#include <include/static_texture.hpp>
#include <include/render_texture.hpp>

namespace ts
{
    RenderTexture::RenderTexture(Window *window)
        : Texture(window), _window(window)
    {}

    void RenderTexture::create(size_t width, size_t height)
    {
        _texture = SDL_CreateTexture(_window->get_renderer(), PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, width, height);
        set_blend_mode(TextureBlendMode::ALPHA);
        set_filtering_mode(TextureFilteringMode::ANISOTROPIC);
    }

    void RenderTexture::unload()
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }

    void RenderTexture::render(const Renderable * object, Transform transform)
    {
        SDL_SetRenderTarget(_window->get_renderer(), _texture);
        detail::forward_render(this, object, transform);
        SDL_SetRenderTarget(_window->get_renderer(), nullptr);
    }

    SDL_Renderer* RenderTexture::get_renderer()
    {
        return _window->get_renderer();
    }
}
