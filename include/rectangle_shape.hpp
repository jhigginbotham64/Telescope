// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/shape.hpp>

namespace ts
{
    //
    class RectangleShape : public Shape
    {
        public:
            //
            RectangleShape(float top_left_x, float top_left_y, float width, float height);

            //
            RectangleShape(Vector2f top_left, Vector2f size);

            //
            Vector2f get_size() const;

            //
            Vector2f set_size();

        private:
            Vector2f _size;
    };
}