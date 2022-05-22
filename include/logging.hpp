// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <mutex>
#include <deque>

namespace ts
{
    // basic thread-safe log, used for debugging
    class Log
    {
        public:
            //
            template<typename... Args_t>
            static void print(Args_t... args);

            //
            template<typename... Args_t>
            static void warning(Args_t... args);

            //
            template<typename... Args_t>
            static void debug(Args_t... args);

        private:
            Log() = delete;

            static inline constexpr const char* LOG_PREFIX = "[LOG]";
            static inline constexpr const char* WARNING_PREFIX = "[WARNING]";
            static inline constexpr const char* DEBUG_PREFIX = "[DEBUG]";
            static inline constexpr const char* GLOBAL_PREFIX = "";//"[C++]";
            static inline std::mutex _lock;

            static inline bool _debug_enabled = true; //TODO: false for release
    };
}

#include <src/logging.inl>