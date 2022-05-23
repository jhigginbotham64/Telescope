// 
// Copyright 2022 Clemens Cords
// Created on 23.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <string>
#include <functional>

namespace ts
{
    // human-readable ID, hash on construction
    class ID
    {
        public:
            //
            ID(const std::string&);

            //
            size_t hash();

        private:
            size_t _hash;
    };
}