// 
// Copyright 2022 Clemens Cords
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#include <iostream>

namespace ts
{
    template<typename... Args_t>
    void Log::print(Args_t... args)
    {
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