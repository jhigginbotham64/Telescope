// 
// Copyright 2022 Clemens Cords
// Created on 21.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <chrono>

namespace ts
{
    // duration
    class Time
    {
        public:
            //
            Time(int64_t n_nanoseconds);

            //
            double as_seconds() const;

            //
            double as_milliseconds() const;

            //
            double as_microseconds() const;

            //
            size_t as_nanoseconds() const;

        private:
            std::chrono::duration<int64_t, std::nano> _ns; // in nanoseconds
    };

    //
    Time seconds(double);

    //
    Time milliseconds(double);

    //
    Time microseconds(double);

    //
    Time nanoseconds(size_t);

    // for measuring durations only, use std::chrono for current date/time
    class Clock
    {
        public:
            //
            Clock();

            //
            Time restart();

            //
            Time elapsed();

        private:
            std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<int64_t, std::nano>> _start;
    };
}