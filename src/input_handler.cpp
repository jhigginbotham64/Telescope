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

    bool InputHandler::is_down_aux(KeyOrButton key)
    {
        return _this_frame_state.find(key_to_sdl_key(key)) == _this_frame_state.end();
    }

    bool InputHandler::has_state_changed_aux(KeyOrButton key)
    {
        bool this_frame = _this_frame_state.find(key_to_sdl_key(key)) == _this_frame_state.end();
        bool last_frame = _last_frame_state.find(key_to_sdl_key(key)) == _last_frame_state.end();
        return this_frame != last_frame;
    }

    bool InputHandler::is_down(KeyOrButton key)
    {
        wait_if_locked();
        return is_down_aux(key);
    }

    bool InputHandler::has_state_changed(KeyOrButton key)
    {
        wait_if_locked();
        return has_state_changed_aux(key);
    }

    bool InputHandler::was_pressed(KeyOrButton key)
    {
        wait_if_locked();
        return is_down_aux(key) and has_state_changed_aux(key);
    }

    bool InputHandler::was_released(KeyOrButton key)
    {
        wait_if_locked();
        return not is_down_aux(key) and has_state_changed_aux(key);
    }
}

