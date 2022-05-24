// 
// Copyright 2022 Clemens Cords
// Created on 21.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <chrono>

namespace ts
{
    /// \brief object representing a time interval, nanoseconds accuracy
    class Time
    {
        public:
            /// \brief convert the time interval into minutes
            /// \returns number of minutes, double
            double as_minutes() const;

            /// \brief convert the time interval into seconds
            /// \returns number of seconds, double
            double as_seconds() const;

            /// \brief convert the time interval into milliseconds
            /// \returns number of milliseconds, double
            double as_milliseconds() const;

            /// \brief convert the time interval into microseconds
            /// \returns number of microseconds, double
            double as_microseconds() const;

            /// \brief convert the time interval into nanoseconds
            /// \returns number of nanoseconds, size_t
            size_t as_nanoseconds() const;

        private:
            friend Time minutes(double);
            friend Time seconds(double);
            friend Time milliseconds(double);
            friend Time microseconds(double);
            friend Time nanoseconds(size_t);

            Time(int64_t n_nanoseconds);

            std::chrono::duration<int64_t, std::nano> _ns; // in nanoseconds
    };

    /// \brief construct time interval from number of minutes
    /// \param minutes: number of minutes, double
    /// \returns time
    Time minutes(double);

    /// \brief construct time interval from number of seconds
    /// \param seconds: number of seconds, double
    /// \returns time
    Time seconds(double);

    /// \brief construct time interval from number of milliseconds
    /// \param minutes: number of milliseconds, double
    /// \returns time
    Time milliseconds(double);

    /// \brief construct time interval from number of microseconds
    /// \param minutes: number of microseconds, double
    /// \returns time
    Time microseconds(double);

    /// \brief construct time interval from number of nanoseconds
    /// \param minutes: number of nanoseconds
    /// \returns time
    Time nanoseconds(size_t);

    /// \brief clock used for measuring time deltas, steady clock, nanosecond precision
    class Clock
    {
        public:
            /// \brief construct the clock and set the time to 0
            Clock();

            /// \brief restart the clock, returns number of seconds elapsed
            /// \returns elapsed time until restart
            Time restart();

            /// \brief get the number of elapsed seconds, does not restart the clock
            /// \returns elapsed time
            Time elapsed();

        private:
            std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<int64_t, std::nano>> _start;
    };
}