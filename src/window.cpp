// 
// Copyright 2022 Joshua Higginbotham
// Created on 22.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/window.hpp>
#include <include/logging.hpp>

#include <SDL2/SDL_image.h>

namespace ts
{
    Window::Window()
        : _is_borderless(false),
          _is_resizable(false),
          _is_minimized(false),
          _is_maximized(false),
          _is_fullscreen(false),
          _is_hidden(false),
          _has_focus(false),
          _has_mouse_focus(false)
    {}

    Window::~Window()
    {
        close();

        if (_icon != nullptr)
            SDL_FreeSurface(_icon);
    }

    void Window::render(const Renderable * object, Transform transform)
    {
        transform.combine(_global_transform);
        detail::forward_render(this, object, transform);
    }

    void Window::create(size_t width, size_t height, uint32_t options)
    {
        create("", width, height, options);
    }

    void Window::create(std::string title, size_t width, size_t height, uint32_t options)
    {
        if (width == 0 or height == 0)
        {
            Log::warning("In ts::Window::create: Trying to create a window of size ", width, "x", height,
                         ". Instead, resizing window to ", std::max<size_t>(width, 1), "x",
                         std::max<size_t>(height, 1));

            width = std::max<size_t>(width, 1);
            height = std::max<size_t>(height, 1);
        }

        if (options & (1 << 1))
            _is_fullscreen = true;

        if (options & (1 << 2))
            _is_borderless = true;

        if (options & (1 << 3))
            _is_resizable = true;

        uint32_t sdl_options = SDL_WINDOW_SHOWN
            | SDL_WINDOW_VULKAN
            | SDL_WINDOW_ALLOW_HIGHDPI;

        if (_is_resizable)
            sdl_options |= SDL_WINDOW_RESIZABLE;

        if (_window != nullptr)
            SDL_DestroyWindow(_window);

        _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdl_options);

        if (_is_borderless)
            SDL_SetWindowBordered(_window, SDL_FALSE);

        if (_is_fullscreen)
            SDL_SetWindowFullscreen(_window, SDL_TRUE);

        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
        SDL_RenderSetVSync(_renderer, true);
        _is_open = true;
    }

    SDL_Window * Window::get_native()
    {
        return _window;
    }

    void Window::close()
    {
        _is_borderless = false;
        _is_resizable = false;
        _is_minimized = false;
        _is_maximized = false;
        _is_fullscreen = false;
        _is_hidden = false;
        _has_focus = false;
        _has_mouse_focus = false;

        SDL_DestroyWindow(_window);
        _is_open = false;
    }

    bool Window::is_open() const
    {
        return _is_open;
    }

    Vector2ui Window::get_size() const
    {
        int width, height;
        SDL_GetRendererOutputSize(_renderer, &width, &height);
        return Vector2ui(width, height);
    }

    Vector2i Window::get_position() const
    {
        int width, height;
        SDL_GetWindowPosition(_window, &width, &height);
        return Vector2i(width, height);
    }

    void Window::set_position(int32_t x, int32_t y)
    {
        SDL_SetWindowPosition(_window, x, y);
    }

    void Window::set_hidden(bool b)
    {
        if (_is_hidden == b)
            return;

        if (b)
        {
            SDL_HideWindow(_window);
            _is_hidden = true;
        }
        else
        {
            SDL_ShowWindow(_window);
            _is_hidden = false;
        }
    }

    bool Window::is_hidden() const
    {
        return _is_hidden;
    }

    void Window::minimize()
    {
        SDL_MinimizeWindow(_window);
        _is_minimized = true;
    }

    bool Window::is_minimized() const
    {
        return _is_minimized;
    }

    void Window::maximize()
    {
        SDL_MaximizeWindow(_window);
        _is_maximized = true;
    }

    bool Window::is_maximized() const
    {
        return _is_maximized;
    }

    bool Window::has_focus() const
    {
        return _has_focus;
    }

    bool Window::has_mouse_focus() const
    {
        return _has_mouse_focus;
    }

    void Window::set_size(size_t width, size_t height)
    {
        SDL_SetWindowSize(_window, width, height);
    }

    WindowID Window::get_id() const
    {
        return SDL_GetWindowID(_window);
    }

    void Window::set_icon(const std::string& path)
    {
        if (_icon != nullptr)
            SDL_FreeSurface(_icon);

        _icon = IMG_Load(path.c_str());
        if (_icon == nullptr)
        {
            Log::warning("In ts::Window::set_icon: Unable to load icon from file ", path);
            return;
        }

        if (_icon->w != _icon->h)
            Log::warning("In ts::Window::set_icon: Icon image should be square. Visual corruption may occur because icon \"", path, "\" is of size ", _icon->w, "x", _icon->h, ".");

        SDL_SetWindowIcon(_window, _icon);
    }

    void Window::clear()
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
    }

    void Window::flush()
    {
        SDL_RenderFlush(_renderer);
        SDL_RenderPresent(_renderer);
    }

    SDL_Renderer* Window::get_renderer()
    {
        return _renderer;
    }
}

