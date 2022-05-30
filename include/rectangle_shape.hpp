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
            void set_centroid(Vector2f) override;

            //
            Vector2f get_centroid() const override;

            //
            void set_top_left(Vector2f);

            //
            Vector2f get_top_left() const;

            //
            Vector2f get_size() const;

            //
            void set_size(Vector2f);

        private:
            Vector2f _top_left;
            Vector2f _size;

            void update();
    };
}