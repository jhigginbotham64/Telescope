// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#include <SDL2/SDL_image.h>

#include <include/logging.hpp>
#include <include/texture.hpp>
#include <include/window.hpp>

namespace ts
{
    Texture::Texture(Window* context)
        : _renderer(context->get_renderer()), _texture(nullptr)
    {}

    Texture::~Texture()
    {
        unload();
    }

    void Texture::update()
    {
        if (_texture == nullptr)
            return;

        SDL_SetTextureBlendMode(_texture, (SDL_BlendMode) _blend_mode);
        SDL_SetTextureColorMod(_texture, _color.red * 255, _color.blue * 255, _color.green * 255);
        SDL_SetTextureAlphaMod(_texture, _color.alpha * 255);
    }

    void Texture::create(size_t width, size_t height, RGBA color)
    {
        SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", std::to_string((size_t) _filtering_mode).c_str());

        auto* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, (uint32_t) PIXEL_FORMAT);
        _texture = SDL_CreateTextureFromSurface(_renderer, surface);

        update();

        SDL_FreeSurface(surface);
        SDL_ClearHints();
    }

    void Texture::load(const std::string& path)
    {
        SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", std::to_string((size_t) _filtering_mode).c_str());
        _texture = IMG_LoadTexture(_renderer, path.c_str());

        if (_texture == nullptr)
            ts::Log::warning("In Texture.load: unable to load texture from file \"", path, "\"");

        update();

        SDL_ClearHints();
    }

    void Texture::unload()
    {
        SDL_DestroyTexture(_texture);
    }

    void Texture::set_color(RGBA color)
    {
        _color = color;
    }

    RGBA Texture::get_color() const
    {
        return _color;
    }

    void Texture::set_blend_mode(TextureBlendMode mode)
    {
        _blend_mode = mode;
        update();
    }

    TextureBlendMode Texture::get_blend_mode() const
    {
        return _blend_mode;
    }

    void Texture::set_filtering_mode(TextureFilteringMode mode)
    {
        _filtering_mode = mode;

        if (_texture != nullptr)
            Log::warning("In Texture.set_filtering_mode: Cannot change texture filtering mode if the texture is already GPU-side. Instead, `unload` the texture, set the filtering mode, then `load` the texture again.");
    }

    TextureFilteringMode Texture::get_filtering_mode() const
    {
        return _filtering_mode;
    }

    SDL_Texture * Texture::get_native()
    {
        return _texture;
    }
}

