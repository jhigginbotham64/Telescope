// 
// Copyright 2022 Clemens Cords
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#include <include/input_handler.hpp>

namespace ts
{
    void initialize()
    {
        _init_lock.lock();



        _init_lock.unlock();
    }

    void InputHandler::wait_if_locked()
    {
        if (_locked)
            _cv.wait(_cv_lock, [locked_ptr = &_locked]{return locked_ptr;});
    }

    bool InputHandler::is_key_down_aux(KeyboardKey key)
    {
        return _this_frame_state.find(key_to_sdl_key(key)) == _this_frame_state.end();
    }

    bool InputHandler::has_state_changed_aux(KeyboardKey key)
    {
        bool this_frame = _this_frame_state.find(key_to_sdl_key(key)) == _this_frame_state.end();
        bool last_frame = _last_frame_state.find(key_to_sdl_key(key)) == _last_frame_state.end();
        return this_frame != last_frame;
    }

    bool InputHandler::is_key_down(KeyboardKey key)
    {
        wait_if_locked();
        return is_key_down(key);
    }

    bool InputHandler::has_state_changed(KeyboardKey key)
    {
        wait_if_locked();
        return has_state_changed_aux(key);
    }

    bool InputHandler::was_key_pressed(KeyboardKey key)
    {
        wait_if_locked();
        return is_key_down(key) and has_state_changed(key);
    }

    bool InputHandler::was_key_released(KeyboardKey key)
    {
        wait_if_locked();
        return not is_key_down(key) and has_state_changed(key);
    }
}

