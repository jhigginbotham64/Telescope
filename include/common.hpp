// 
// Copyright 2022 Joshua Higginbotham
// Created on 5/31/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <include/window.hpp>
#include <include/time.hpp>

namespace ts
{
    /// \brief initialize all components of telescope
    /// \returns true if successfull, otherwise an exception will be thrown and false is returned
    bool initialize();

    /// \brief set the fps limit for all windows
    /// \param frames_per_second: integer
    void set_framerate_limit(size_t frames_per_second);

    /// \brief start the frame, updates input component and window, nothing should happen in between this and end_frame
    /// \param window
    ts::Time start_frame(Window* window);

    /// \brief start the frame, updates input component and window, nothing should happen in between this and end_frame
    /// \param windows
    ts::Time start_frame(std::vector<Window*> windows);

    /// \brief end the frame, pushes current render state to the screen, then waits to achieve target fps
    /// \param window
    void end_frame(Window* window);

    /// \brief end the frame, pushes current render state to the screen, then waits to achieve target fps
    /// \param windows
    void end_frame(std::vector<Window*> windows);
}

namespace ts::detail
{
    static inline size_t _target_fps = 60;
    static inline Clock _frame_clock = ts::Clock();
}