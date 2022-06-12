// 
// Copyright 2022 Joshua Higginbotham
// Created on 22.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <SDL2/SDL_render.h>

#include <string>

#include <include/vector.hpp>
#include <include/render_target.hpp>
#include <include/transform.hpp>

extern "C" {

namespace ts
{
    /// \brief window options
    enum WindowOptions : uint32_t
    {
        /// \brief default: not fullscreen, with border
        DEFAULT = 0,

        /// \brief make window native fullscreen
        FULLSCREEN = 1 << 1,

        /// \brief make window borderless
        BORDERLESS = 1 << 2,

        /// \brief allow resizing by the user
        RESIZABLE  = 1 << 3
    };

    using WindowID = int32_t;

    /// \brief window, creates render context and allows for displaying shapes on the monitor
    class Window : public RenderTarget
    {
        friend class InputHandler;
        friend class Camera;

        public:
            /// \brief default ctor
            Window();

            // no docs
            ~Window();

            /// \brief render a renderable
            /// \param renderable: object to be rendered, usually a ts::Shape
            /// \param transform: transform to be applied before rendering
            void render(const Renderable*, Transform = ts::Transform()) override;

            /// \brief create the window
            /// \param width: number of pixels in x-dimension
            /// \param height: number of pixels in y-dimension
            void create(size_t width, size_t height, uint32_t options = DEFAULT);

            /// \brief create the window with a title
            /// \param title: title
            /// \param width: number of pixels in x-dimension
            /// \param height: number of pixels in y-dimension
            void create(std::string title, size_t width, size_t height, uint32_t options = DEFAULT);

            /// \brief close the window
            void close();

            /// \brief is the window close
            /// \returns false if window is closed, true otherwise
            bool is_open() const;

            /// \brief get the windows size
            /// \returns size, in pixels
            Vector2ui get_size() const;

            /// \brief set the windows size, even if it is not resizable
            /// \param width: number of pixels in x-dimension
            /// \param height: number of pixels in y-dimension
            void set_size(size_t width, size_t height);

            /// \brief set the windows position on the monitor
            /// \param x: x-coordinate
            /// \param y: y-coordinate
            /// \note (0, 0) is the top left corner of the monitor
            void set_position(int32_t x, int32_t y);

            /// \brief get the position of the window on the monitor
            /// \returns position
            Vector2i get_position() const;

            /// \brief set whether the window is hidden
            /// \param value: true if window should be hidden, false otherwise
            void set_hidden(bool);

            /// \brief is the window hidden
            /// \returns true if the window is hidden, false otherwise
            bool is_hidden() const;

            /// \brief minimize the window
            void minimize();

            /// \brief is the window minimized
            /// \returns true if minimized, false otherwise
            bool is_minimized() const;

            /// \brief maximize the window
            void maximize();

            /// \brief is the window maximized
            /// \returns true if maximized, false otherwise
            bool is_maximized() const;

            /// \brief is the window focused
            /// \returns true if focused, false otherwise
            bool has_focus() const;

            /// \brief is the window focused and is the mouse cursor currently inside the window
            /// \returns bool
            bool has_mouse_focus() const;

            // no docs
            WindowID get_id() const;

            /// \brief set the window icon
            /// \param path: path to image
            /// \note supported formats include: .bmp, .png, .jpg, .jpeg
            void set_icon(const std::string& path);

            /// \brief clear the windows render state
            void clear();

            /// \brief push the current render state to the monitor
            void flush();

            /// \brief get the native SDL window
            /// \returns pointer to SDL_Window
            SDL_Window* get_native();

            /// \brief get the native SDL renderer
            /// \returns pointer to SDL_Renderer
            SDL_Renderer* get_renderer() override;

        private:
            SDL_Window* _window = nullptr;
            SDL_Renderer* _renderer;

            SDL_Surface* _icon = nullptr;

            Transform _global_transform; // camera state

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

}