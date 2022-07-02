// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <iostream>

namespace ts
{
    inline void set_all_logging_disabled(bool disabled)
    {
        Log::disabled = disabled;
    }

    template<typename... Args_t>
    void Log::print(Args_t... args)
    {
        if (disabled)
            return;

        _lock.lock();
        std::cout << GLOBAL_PREFIX << LOG_PREFIX << " ";

        static auto add = [](auto in) -> void {
            std::cout << in;
        };

        (add(args), ...);
        std::cout << std::endl;
        _lock.unlock();
    }

    template<typename... Args_t>
    void Log::warning(Args_t... args)
    {
        if (disabled)
            return;

        _lock.lock();
        std::cerr << GLOBAL_PREFIX << WARNING_PREFIX << " ";

        static auto add = [](auto in) -> void {
            std::cerr << in;
        };

        (add(args), ...);
        std::cerr << std::endl;
        _lock.unlock();
    }

    template<typename... Args_t>
    void Log::debug(Args_t... args)
    {
        if (disabled)
            return;

        if (not _debug_enabled)
            return;

        _lock.lock();
        std::cerr << GLOBAL_PREFIX << DEBUG_PREFIX << " ";

        static auto add = [](auto in) -> void {
            std::cerr << in;
        };

        (add(args), ...);
        std::cerr << std::endl;
        _lock.unlock();
    }
}