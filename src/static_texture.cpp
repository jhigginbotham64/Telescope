// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <SDL2/SDL_image.h>

#include <include/logging.hpp>
#include <include/static_texture.hpp>
#include <include/window.hpp>

namespace ts
{
    StaticTexture::StaticTexture(Window* context)
        : Texture(context)
    {}

    void StaticTexture::create(size_t width, size_t height, RGBA color)
    {
        SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", std::to_string((size_t) get_filtering_mode()).c_str());

        auto* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, (uint32_t) PIXEL_FORMAT);
        _texture = SDL_CreateTextureFromSurface(get_window()->get_renderer(), surface);
        Texture::update();

        SDL_FreeSurface(surface);
        SDL_ClearHints();
    }

    bool StaticTexture::load(const std::string& path)
    {
        SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", std::to_string((size_t) get_filtering_mode()).c_str());
        _texture = IMG_LoadTexture(get_window()->get_renderer(), path.c_str());

        if (_texture == nullptr)
        {
            ts::Log::warning("In ts::Texture.load: unable to load texture from file \"", path, "\"");
            _texture = nullptr;
            return false;
        }

        Texture::update();
        SDL_ClearHints();
        return true;
    }

    void StaticTexture::unload()
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

