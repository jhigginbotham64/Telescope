// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/input_handler.hpp>
#include <include/logging.hpp>

namespace ts
{
    void InputHandler::update(ts::Window* window)
    {
        update(std::vector<ts::Window*>{window});
    }

    void InputHandler::update(std::vector<ts::Window*> windows)
    {
        _init_lock.lock();
        _locked = true;

        _keyboard_state[0] = _keyboard_state[1];
        _mouse_state[0] = _mouse_state[1];
        _mouse_state[1].scroll_delta = Vector2f(0, 0);

        for (auto& pair : _controller_states)
            pair.second[0] = pair.second[1];

        auto event = SDL_Event();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                _keyboard_state[1].pressed.insert((KeyboardKey) event.key.keysym.sym);
            }
            else if (event.type == SDL_KEYUP)
            {
                _keyboard_state[1].pressed.erase((KeyboardKey) event.key.keysym.sym);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                _mouse_state[1].pressed.insert((MouseButton) event.button.button);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                _mouse_state[1].pressed.insert((MouseButton) event.button.button);
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                _mouse_state[1].position = Vector2i(event.motion.x, event.motion.x);
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                bool flipped = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED;
                _mouse_state[1].scroll_delta.x = event.wheel.x * (flipped ? -1 : 1);
                _mouse_state[1].scroll_delta.y = event.wheel.y * (flipped ? -1 : 1);
            }
            else if (event.type == SDL_CONTROLLERDEVICEADDED)
            {
                auto id = event.cdevice.which;
                Log::print("Controller ", id, " connected");

                _controller_states.insert_or_assign(id, std::array<ControllerState, 2>());
            }
            else if (event.type == SDL_CONTROLLERDEVICEREMOVED)
            {
                auto id = event.cdevice.which;
                Log::print("Controller ", id, " disconnected");

                _controller_states.erase(id);
            }
            else if (event.type == SDL_CONTROLLERBUTTONDOWN)
            {
                _controller_states[event.cbutton.which][1].pressed.insert((ControllerButton) event.cbutton.button);
            }
            else if (event.type == SDL_CONTROLLERBUTTONUP)
            {
                _controller_states[event.cbutton.which][1].pressed.erase((ControllerButton) event.cbutton.button);
            }
            else if (event.type == SDL_CONTROLLERAXISMOTION)
            {
                auto& state = _controller_states[event.caxis.which][1];

                if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
                    state.axis_left.x = event.caxis.value / 32767.f;

                else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
                    state.axis_left.y = event.caxis.value / 32767.f;

                else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
                    state.axis_right.x = event.caxis.value / 32767.f;

                else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
                    state.axis_right.y = event.caxis.value / 32767.f;

                else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
                    state.trigger_left = event.caxis.value / 32767.f;

                else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
                    state.trigger_right = event.caxis.value / 32767.f;

                else
                {
                    static bool already_printed = false;
                    if (not already_printed)
                    {
                        Log::warning("In ts::InputHandler.update: controller axis ", event.caxis.axis, " unsupported");
                        already_printed = true;
                    }
                }
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    for (auto* w : windows)
                    {
                        if (uint32_t(w->get_id()) == event.window.windowID)
                            w->close();
                    }
                }
                else if (event.window.event == SDL_WINDOWEVENT_SHOWN)
                {
                    // noop
                }
                else if (event.window.event == SDL_WINDOWEVENT_HIDDEN)
                {
                    // noop
                }
                else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    // noop, resized by OS
                }
                else if (event.window.event == SDL_WINDOWEVENT_MINIMIZED)
                {
                    for (auto* w : windows)
                    {
                        if (uint32_t(w->get_id()) == event.window.windowID)
                            w->minimize();
                    }
                }
                else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED)
                {
                    for (auto* w : windows)
                    {
                        if (uint32_t(w->get_id()) == event.window.windowID)
                            w->maximize();
                    }
                }
                else if (event.window.event == SDL_WINDOWEVENT_ENTER)
                {
                    for (auto* w : windows)
                    {
                        if (uint32_t(w->get_id()) == event.window.windowID)
                            w->_has_mouse_focus = true;
                    }
                }
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE)
                {
                    for (auto* w : windows)
                    {
                        if (uint32_t(w->get_id()) == event.window.windowID)
                            w->_has_mouse_focus = false;
                    }
                }
                else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                {
                    for (auto* w : windows)
                    {
                        if (uint32_t(w->get_id()) == event.window.windowID)
                            w->_has_focus = true;
                    }
                }
                else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                {
                    for (auto* w : windows)
                    {
                        if (uint32_t(w->get_id()) == event.window.windowID)
                            w->_has_focus = false;
                    }
                }
            }
            else if (event.type == SDL_QUIT)
            {
                Log::print("System requested shutdown. Quitting...");
                for (auto* w : windows)
                    w->close();
            }
            else
            {
                // noop
            }
        }

        _locked = false;
        _init_lock.unlock();
        _cv.notify_all();
    }

    void InputHandler::wait_if_locked()
    {
        if (_locked)
            _cv.wait(_cv_lock, [&]() -> bool {return _locked;});
    }

    bool InputHandler::is_down(KeyboardKey keyboard_key)
    {
        wait_if_locked();
        return _keyboard_state[1].pressed.find(keyboard_key) != _keyboard_state[1].pressed.end();
    }

    bool InputHandler::is_down(MouseButton mouse_button)
    {
        wait_if_locked();
        return _mouse_state[1].pressed.find(mouse_button) != _mouse_state[1].pressed.end();
    }

    bool InputHandler::is_down(ControllerButton controller_button, ControllerID id)
    {
        wait_if_locked();
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::is_down: No controller with id ", id, " connected, returning false");
                already_printed.insert(id);
            }
            return false;
        }

        return it->second[1].pressed.find(controller_button) != it->second[1].pressed.end();
    }

    bool InputHandler::has_state_changed(KeyboardKey keyboard_key)
    {
        wait_if_locked();

        bool before = _keyboard_state[0].pressed.find(keyboard_key) != _keyboard_state[0].pressed.end();
        bool after = _keyboard_state[1].pressed.find(keyboard_key) != _keyboard_state[1].pressed.end();


        return before != after;
    }

    bool InputHandler::has_state_changed(MouseButton mouse_button)
    {
        wait_if_locked();
        bool before = _mouse_state[0].pressed.find(mouse_button) != _mouse_state[0].pressed.end();
        bool after = _mouse_state[1].pressed.find(mouse_button) != _mouse_state[1].pressed.end();

        return before != after;
    }

    bool InputHandler::has_state_changed(ControllerButton controller_button, ControllerID id)
    {
        wait_if_locked();
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::has_state_changed: No controller with id ", id, " connected, returning false");
                already_printed.insert(id);
            }
            return false;
        }

        bool before = it->second[0].pressed.find(controller_button) != it->second[0].pressed.end();
        bool after = it->second[1].pressed.find(controller_button) != it->second[1].pressed.end();

        return before != after;
    }

    bool InputHandler::was_pressed(KeyboardKey keyboard_key)
    {
        wait_if_locked();
        return is_down(keyboard_key) and has_state_changed(keyboard_key);
    }

    bool InputHandler::was_pressed(MouseButton mouse_button)
    {
        wait_if_locked();
        return is_down(mouse_button) and has_state_changed(mouse_button);
    }

    bool InputHandler::was_pressed(ControllerButton controller_button, ControllerID id)
    {
        wait_if_locked();
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::was_pressed: No controller with id ", id, " connected, returning false");
                already_printed.insert(id);
            }
            return false;
        }

        return is_down(controller_button, id) and has_state_changed(controller_button, id);
    }

    bool InputHandler::was_released(KeyboardKey keyboard_key)
    {
        wait_if_locked();
        return not was_pressed(keyboard_key);
    }

    bool InputHandler::was_released(MouseButton mouse_button)
    {
        wait_if_locked();
        return not was_pressed(mouse_button);
    }

    bool InputHandler::was_released(ControllerButton controller_button, ControllerID id)
    {
        wait_if_locked();
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::was_released: No controller with id ", id, " connected, returning false");
                already_printed.insert(id);
            }
            return false;
        }

        return not was_pressed(controller_button, id);
    }

    Vector2i InputHandler::get_cursor_position()
    {
        return _mouse_state[1].position;
    }

    Vector2f InputHandler::get_scrollwheel()
    {
        return _mouse_state[1].scroll_delta;
    }

    Vector2f InputHandler::get_controller_axis_left(ControllerID id)
    {
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::get_controller_axis_left: No controller with id ", id, " connected, returning (0, 0)");
                already_printed.insert(id);
            }
            return Vector2f(0, 0);
        }
        return it->second[1].axis_left;
    }

    Vector2f InputHandler::get_controller_axis_right(ControllerID id)
    {
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::get_controller_axis_right: No controller with id ", id, " connected, returning (0, 0)");
                already_printed.insert(id);
            }
            return Vector2f(0, 0);
        }
        return it->second[1].axis_right;
    }

    float InputHandler::get_controller_trigger_left(ControllerID id)
    {
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::get_controller_trigger_left: No controller with id ", id, " connected, returning 0");
                already_printed.insert(id);
            }
            return 0;
        }
        return it->second[1].trigger_left;
    }

    float InputHandler::get_controller_trigger_right(ControllerID id)
    {
        auto it = _controller_states.find(id);
        if (it == _controller_states.end())
        {
            static auto already_printed = std::set<size_t>();
            if (already_printed.find(id) == already_printed.end())
            {
                Log::warning("In ts::InputHandler::get_controller_trigger_right: No controller with id ", id, " connected, returning 0");
                already_printed.insert(id);
            }
            return 0;
        }
        return it->second[1].trigger_right;
    }
}

