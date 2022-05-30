// 
// Copyright 2022 Clemens Cords
// Created on 5/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/window.hpp>
#include <include/time.hpp>

namespace ts
{
    //
    bool initialize();

    //
    void set_framerate_limit(size_t frames_per_second);

    //
    ts::Time start_frame(Window* window);

    //
    ts::Time start_frame(std::vector<Window*> windows);

    //
    void end_frame(Window* window);

    //
    void end_frame(std::vector<Window*> windows);
}

namespace ts::detail
{
    static inline size_t _target_fps = 60;
    static inline Clock _frame_clock = ts::Clock();
}