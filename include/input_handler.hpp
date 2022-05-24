//
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_events.h>

#include <mutex>
#include <condition_variable>
#include <set>
#include <vector>
#include <map>
#include <atomic>

#include <include/vector.hpp>
#include <include/key_or_button.hpp>
#include <include/window.hpp>

namespace ts
{
    /// \brief controller id: 0, 1, ...
    using ControllerID = size_t;
    
    /// \brief handle user input, thread-safe
    class InputHandler
    {
        public:
            /// \brief is keyboard key currently down
            /// \param keyboard_key: key
            /// \returns bool
            static bool is_down(KeyboardKey keyboard_key);
            
            /// \brief is mouse button currently down
            /// \param mouse_button: button
            /// \returns bool
            static bool is_down(MouseButton mouse_button);
            
            /// \brief is controller button currently down
            /// \param controller_button: button
            /// \param controller_id: id of controller: 0, 1, ...
            /// \returns bool
            static bool is_down(ControllerButton controller_button, ControllerID id = 0);

            /// \brief is the state of a keyboard key this frame different from last frame
            /// \param keyboard_key: key
            /// \returns bool
            static bool has_state_changed(KeyboardKey keyboard_key);
            
            /// \brief is the state of a mouse button this frame different from last frame
            /// \param mouse_button: button
            /// \returns bool
            static bool has_state_changed(MouseButton mouse_button);
            
            /// \brief is the state of a controller button this frame different from last frame
            /// \param controller_button: button
            /// \param controller_id: id of controller: 0, 1, ...
            /// \returns bool
            static bool has_state_changed(ControllerButton controller_button, ControllerID id = 0);

            /// \brief was the keyboard key up last frame and down this frame
            /// \param keyboard_key: key
            /// \returns bool
            static bool was_pressed(KeyboardKey keyboard_key);
            
            /// \brief was the mouse button up last frame and down this frame
            /// \param mouse_button: button
            /// \returns bool
            static bool was_pressed(MouseButton mouse_button);

            /// \brief was the controller button up last frame and down this frame
            /// \param controller_button: button
            /// \param controller_id: id of controller: 0, 1, ...
            /// \returns bool
            static bool was_pressed(ControllerButton controller_button, ControllerID id = 0);

            /// \brief was the keyboard key down last frame and up this frame
            /// \param keyboard_key: key
            /// \returns bool
            static bool was_released(KeyboardKey keyboard_key);

            /// \brief was the mouse button down last frame and up this frame
            /// \param mouse_button: button
            /// \returns bool
            static bool was_released(MouseButton mouse_button);

            /// \brief was the controller button down last frame and up this frame
            /// \param controller_button: button
            /// \param controller_id: id of controller: 0, 1, ...
            /// \returns bool
            static bool was_released(ControllerButton controller_button, ControllerID id = 0);

            /// \brief get the current position of the mouse cursor on the screen
            /// \returns vector of integers, where [0, 0] is the top left position of the currently focused window
            static Vector2i get_cursor_position();

            /// \brief get the scrollwheel offset inputted last frame
            /// \returns vector of floats, where +y: up, -y: down, +x: right, -x: left
            static Vector2f get_scrollwheel();

            /// \brief get the current state of the left controller joystick
            /// \param controller_id: id of controller: 0, 1, ...
            /// \returns vector of floats, where +y: up, -y: down, +x: right, -x: left, values in [-1, 1]
            static Vector2f get_controller_axis_left(ControllerID = 0);

            /// \brief get the current state of the right controller joystick
            /// \param controller_id: id of controller: 0, 1, ...
            /// \returns vector of floats, where +y: up, -y: down, +x: right, -x: left, values in [-1, 1]
            static Vector2f get_controller_axis_right(ControllerID = 0);

            /// \brief get the state of the left controller trigger
            /// \returns float in [0, 1], where 0: not pressed, 1: full pressed
            static float get_controller_trigger_left(ControllerID = 0);

            /// \brief get the state of the left controller trigger
            /// \returns float in [0, 1], where 0: not pressed, 1: fully pressed
            static float get_controller_trigger_right(ControllerID = 0);

        //protected:
            static void update(ts::Window*);
            static void update(std::vector<ts::Window*>);

        private:
            static inline std::atomic<bool> _locked = false;
            static inline std::unique_lock<std::mutex> _cv_lock;
            static inline std::condition_variable _cv;

            static inline std::mutex _init_lock;
            
            struct KeyboardState
            {
                std::set<KeyboardKey> pressed;
            };
            
            struct MouseState
            {
                std::set<MouseButton> pressed;

                Vector2i position; // always to window in focus
                Vector2f scroll_delta;
            };
            
            struct ControllerState
            {
                std::set<ControllerButton> pressed;
                
                Vector2f axis_left;
                Vector2f axis_right;

                float trigger_left;
                float trigger_right;
            };

            // states: 0 is last frame, 1 is this frame
            static inline std::array<KeyboardState, 2> _keyboard_state;
            static inline std::array<MouseState, 2> _mouse_state;

            // one state per controller
            static inline std::map<ControllerID, std::array<ControllerState, 2>> _controller_states;

            static void wait_if_locked();
    };
}