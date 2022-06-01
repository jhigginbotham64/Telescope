// 
// Copyright 2022 Clemens Cords
// Created on 22.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <SDL2/SDL_render.h>

#include <string>

#include <include/vector.hpp>
#include <include/render_target.hpp>

namespace ts
{
    enum WindowOptions : uint32_t
    {
        DEFAULT = 0,
        FULLSCREEN = 1 << 1,
        BORDERLESS = 1 << 2,
        RESIZABLE  = 1 << 3
    };

    using WindowID = int32_t;

    class Window : public RenderTarget
    {
        friend class InputHandler;

        public:
            //
            Window();

            //
            ~Window();

            //
            void render(const Renderable*, Transform = ts::Transform()) override;

            //
            void create(size_t width, size_t height, uint32_t options = DEFAULT);

            //
            void create(std::string title, size_t width, size_t height, uint32_t options = DEFAULT);

            //
            void close();

            //
            bool is_open() const;

            //
            Vector2ui get_size() const;

            //
            void set_size(size_t width, size_t height);

            //
            void set_position(int32_t x, int32_t y);

            //
            Vector2i get_position() const;

            //
            void set_hidden(bool);

            //
            bool is_hidden() const;

            //
            void minimize();

            //
            bool is_minimized() const;

            //
            void maximize();

            //
            bool is_maximized() const;

            //
            bool has_focus() const;

            //
            bool has_mouse_focus() const;

            //
            WindowID get_id() const;

            //
            void clear();

            //
            void flush();

            //
            SDL_Window* get_native();

            //
            SDL_Renderer* get_renderer() override;

        private:
            SDL_Window* _window = nullptr;
            SDL_Renderer* _renderer;
            bool _is_open = false;

            bool _is_borderless;
            bool _is_resizable;

            bool _is_minimized;
            bool _is_maximized;
            bool _is_fullscreen;
            bool _is_hidden;

            // updated by InputHandler:
            bool _has_focus;
            bool _has_mouse_focus;
    };
}