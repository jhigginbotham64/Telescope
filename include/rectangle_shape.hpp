// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/shape.hpp>

namespace ts
{
    /// \brief axis aligned rectangle shape
    class RectangleShape : public Shape
    {
        public:
            /// \brief construct, C-interface
            /// \param top_left_x: x-coordinate of the top left corner
            /// \param top_left_y: y-coordinate of the top left corner
            /// \param width: width of the rectangle
            /// \param height: height of the rectangle
            RectangleShape(float top_left_x, float top_left_y, float width, float height);

            /// \brief construct, C-interface
            /// \param top_left: x- and y-coordinate of the top left corner
            /// \param size: size of the rectangle
            RectangleShape(Vector2f top_left, Vector2f size);

            /// \brief set the centroid, moves all vertices accordingly
            /// \param new_position: new position of the centroid
            void set_centroid(Vector2f) override;

            /// \brief get the centroid
            /// \returns centroid
            Vector2f get_centroid() const override;

            /// \brief set top left corners coordinate, moves all vertices accordingly
            /// \param new_position: new position of the top left corner
            void set_top_left(Vector2f);

            /// \brief get the top left corners coordinate
            /// \returns position
            Vector2f get_top_left() const;

            /// \brief get the size of the rectangle
            /// \returns 2-vector, where .x is the width, .y the height of the rectangle
            Vector2f get_size() const;

            /// \brief set the size of the rectangle, moves all coordinates except the top left one accordingly
            /// \param new_size: new size where .x is the width, .y the height
            void set_size(Vector2f);

        private:
            Vector2f _top_left;
            Vector2f _size;

            void update();
    };
}