// 
// Copyright 2022 Clemens Cords
// Created on 22.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_render.h>

#include <include/vector.hpp>

namespace ts
{
    enum WindowOptions
    {
        FULLSCREEN = SDL_WINDOW_FULLSCREEN,
        BORDERLESS = SDL_WINDOW_BORDERLESS,
        RESIZABLE = SDL_WINDOW_RESIZABLE
    };

    class Window
    {
        friend class InputHandler;

        public:
            Window();

            void create(size_t width, size_t height, WindowOptions options);
            void close();

            void set_hidden(bool);
            bool is_hidden() const;

            void minimize();
            bool is_minimized() const;

            void maximize();
            bool is_maximized() const;

            void has_focus() const;
            void has_mouse_focus() const;

            void resize(size_t width, size_t height);

            size_t get_id() const;

        private:
            SDL_Window* _window;

            size_t _id;
            Vector2ui _size;

            bool _is_borderless;
            bool _is_resizable;

            bool _is_minimized;
            bool _is_maximized;
            bool _is_hidden;

            // updated by InputHandler:
            bool _has_focus;
            bool _has_mouse_focus;
    };
}