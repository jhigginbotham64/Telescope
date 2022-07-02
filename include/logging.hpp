// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <mutex>
#include <deque>

namespace ts
{
    /// \brief suppress all log output from this point on
    /// \param disabled: should the output be disabled?
    void set_all_logging_disabled(bool disabled);

    /// \brief basic thread-safe log
    class Log
    {
        public:
            static inline bool disabled = false;

            /// \brief print as general info
            /// \param args: arguments, converted to strings
            template<typename... Args_t>
            static void print(Args_t... args);

            /// \brief print as warning to stderr
            /// \param args: arguments, converted to strings
            template<typename... Args_t>
            static void warning(Args_t... args);

            /// \brief print only if debug output was enabled
            /// \param args: arguments, converted to strings
            template<typename... Args_t>
            static void debug(Args_t... args);

        private:
            Log() = delete;

            static inline constexpr const char* LOG_PREFIX = "[LOG]";
            static inline constexpr const char* WARNING_PREFIX = "[WARNING]";
            static inline constexpr const char* DEBUG_PREFIX = "[DEBUG]";
            static inline constexpr const char* GLOBAL_PREFIX = "";//"[C++]";
            static inline std::mutex _lock;

            static inline bool _debug_enabled = false;
    };
}

#include <src/logging.inl>