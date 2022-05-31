//
// Created by clem on 5/31/22.
//

#include <SDL2/SDL_image.h>

#include <include/texture.hpp>
#include <include/static_texture.hpp>
#include <include/render_texture.hpp>

namespace ts
{
    RenderTexture::RenderTexture(Window *window)
        : Texture(window)
    {
        RenderTarget::create(window);
    }

    void RenderTexture::create(size_t width, size_t height)
    {
        SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", std::to_string((size_t) get_filtering_mode()).c_str());

        _texture = SDL_CreateTexture(get_renderer(), PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, width, height);
        SDL_ClearHints();
    }

    void RenderTexture::unload()
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}
