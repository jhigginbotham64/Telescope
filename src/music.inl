//
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#include <functional>

namespace ts
{
    namespace detail
    {
        // singleton wrapper to allow lambdas to be called as C-style function pointers
        union MusicFunctionHook
        {
            static inline std::function<void()> function = [](){};

            static void clear()
            {
                function = [](){};
            }

            static void invoke_once()
            {
                function();
                clear();
            }
        };
    }
}