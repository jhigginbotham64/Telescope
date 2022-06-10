// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <include/shape.hpp>

namespace ts
{
    /// \brief circle shape with variable number of vertices
    class CircleShape : public Shape
    {
        public:
            virtual ~CircleShape() = default;

            /// \brief construct
            /// \param center: centroid
            /// \param radius: radius
            /// \param n_outer_vertices: number of vertices along the circumference of the circle
            CircleShape(Vector2f center, float radius, size_t n_outer_vertices);

            /// \brief construct C-interface
            /// \param center_x: x-coordinate of the centroid
            /// \param center_y: y-coordinate of the centroid
            /// \param radius: radius
            /// \param n_outer_vertices: number of vertices along the circumference of the circle
            CircleShape(float center_x, float center_y, float radius, size_t n_outer_vertices);

            /// \brief get centroid
            /// \returns center
            Vector2f get_centroid() const override;

            /// \brief set centroid
            /// \param centroid coordinates
            void set_centroid(Vector2f) override;

            /// \param get radius
            /// \returns float
            float get_radius() const;

            /// \param set radius
            /// \param radius
            void set_radius(float);

        private:
            Vector2f _center;
            float _radius;
            size_t _n_vertices;
                // number of outer vertices before tri decomposition

            void update();
    };
}