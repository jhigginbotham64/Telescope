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
    //
    using ControllerID = size_t;
    
    // locked while updating, any query is blocked
    class InputHandler
    {
        public:
            //
            static bool is_down(KeyboardKey keyboard_key);
            
            //
            static bool is_down(MouseButton mouse_button);
            
            //
            static bool is_down(ControllerButton controller_button, ControllerID id = 0);

            //
            static bool has_state_changed(KeyboardKey keyboard_key);
            
            //
            static bool has_state_changed(MouseButton mouse_button);
            
            //
            static bool has_state_changed(ControllerButton controller_button, ControllerID id = 0);

            //           
            static bool was_pressed(KeyboardKey keyboard_key);
            
            //
            static bool was_pressed(MouseButton mouse_button);
            
            //
            static bool was_pressed(ControllerButton controller_button, ControllerID id = 0);

            //
            static bool was_released(KeyboardKey keyboard_key);
            
            //
            static bool was_released(MouseButton mouse_button);
            
            //
            static bool was_released(ControllerButton controller_button, ControllerID id = 0);

            // relative to focused window top left, may be negative, absolute value in pixels
            static Vector2i get_cursor_position();

            // offset, 0 if no change
            // +y: up, -y: down, +x: right, -x: left
            static Vector2f get_scrollwheel();
            
            // +y: up, -y: down, +x: right, -x: left, values in [-1, 1]
            static Vector2f get_controller_axis_left(ControllerID = 0);

            // +y: up, -y: down, +x: right, -x: left, values in [-1, 1]
            static Vector2f get_controller_axis_right(ControllerID = 0);
            
            // in [0, 1], where 0: not pressed, 1: full pressed
            static float get_controller_trigger_left(ControllerID = 0);
            
            // in [0, 1], where 0: not pressed, 1: fully pressed
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