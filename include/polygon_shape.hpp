// 
// Copyright 2022 Joshua Higginbotham
// Created on 5/30/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <include/shape.hpp>

namespace ts
{
    /// \brief convex polygon
    class PolygonShape : public Shape
    {
        public:
            // no docs
            virtual ~PolygonShape() = default;

            /// \brief construct from number of points, will construct the convex hull of the points
            /// \param points
            PolygonShape(const std::vector<Vector2f>& positions);

            /// \brief get centroid, average of all vertices
            /// \returns centroid
            Vector2f get_centroid() const override;

            /// \brief set the centroid, moves all vertices accordingly
            /// \param new_position: new position of the centroid
            void set_centroid(Vector2f) override;
    };
}