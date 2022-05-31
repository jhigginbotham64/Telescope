//
// Created by clem on 5/31/22.
//

#include <include/logging.hpp>
#include <include/texture.hpp>

namespace ts
{
    Texture::Texture(Window* window)
        : _window(window), _texture(nullptr)
    {}

    Texture::~Texture()
    {
        if (_texture != nullptr)
            SDL_DestroyTexture(_texture);
    }

    void Texture::set_color(RGBA color)
    {
        _color = color;
        update();
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
            Log::warning("In ts::Texture.set_filtering_mode: Cannot change texture filtering mode if the texture is already GPU-side. Instead, `unload` the texture, set the filtering mode, then `load` the texture again.");
    }

    TextureFilteringMode Texture::get_filtering_mode() const
    {
        return _filtering_mode;
    }

    SDL_Texture * Texture::get_native()
    {
        return _texture;
    }

    Window* Texture::get_window() const
    {
        return _window;
    }

    void Texture::update()
    {
        if (_texture == nullptr)
            return;

        SDL_SetTextureBlendMode(_texture, (SDL_BlendMode) _blend_mode);
        SDL_SetTextureColorMod(_texture, _color.red * 255, _color.blue * 255, _color.green * 255);
        SDL_SetTextureAlphaMod(_texture, _color.alpha * 255);
    }
}
