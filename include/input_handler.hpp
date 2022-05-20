//
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_joystick.h>

#include <mutex>
#include <condition_variable>
#include <set>

#include <include/vector.hpp>
#include <include/key_or_button.hpp>

namespace ts
{
    // locked while updating, any query is blocked
    class InputHandler
    {
        public:
            /// \brief is key or button currently pressed
            static bool is_down(KeyOrButton key);

            /// \brief is key or buttons current state different from last frame
            static bool has_state_changed(KeyOrButton key);

            /// \brief is button pressed this frame and not pressed last frame
            static bool was_pressed(KeyOrButton key);

            /// \brief is button not pressed this frame and pressed last frame
            static bool was_released(KeyOrButton key);

            /// \brief get cursor position, returns coordinates (in pixels), where (0, 0) is the top left of the window
            static Vector2i get_cursor_position();
            
            /// \brief get current position of left joystick axis, vector2 where each component is a float in [-1, 1]
            static Vector2f get_controller_axis_left(size_t controller_index = 0);

            /// \brief get current position of right joystick axis, vector2 where each component is a float in [-1, 1]
            static Vector2f get_controller_axis_right(size_t controller_index = 0);

            /// \brief get state of left trigger
            /// \returns float in [0, 1], where 0 is not pressed, 1 is full depressed
            static Vector2f get_controller_trigger_left(size_t controller_index = 0);

            /// \brief get state of right trigger
            /// \returns float in [0, 1], where 0 is not pressed, 1 is full depressed
            static Vector2f get_controller_trigger_right(size_t controller_index = 0);

        protected:
            //
            static void initialize();

            //
            static void update();

        private:
            static std::atomic<bool> _locked;
            static inline std::unique_lock<std::mutex> _cv_lock;
            static inline std::condition_variable _cv;

            static std::mutex _init_lock;
            static std::set<SDL_KeyCode> _last_frame_state;
            static std::set<SDL_KeyCode> _this_frame_state;

            // non-thread-safe access:
            static bool is_down_aux(KeyOrButton key);
            static bool has_state_changed_aux(KeyOrButton key);

            static SDL_KeyCode key_to_sdl_key(KeyOrButton key);
            static void wait_if_locked();
    };
}