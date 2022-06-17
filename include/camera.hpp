//
// Copyright 2022 Joshua Higginbotham
// Created on 6/2/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <include/vector.hpp>
#include <include/angle.hpp>
#include <include/transform.hpp>
#include <include/geometric_shapes.hpp>

namespace ts
{
    class Window;

    /// \brief controls the view area without affecting local object geometry
    class Camera
    {
        public:
            /// \brief construct from window
            /// \param window: window to control
            Camera(Window*);

            /// \brief reset transform to identity
            void reset();

            /// \brief move the camera such that it is centered on a given point
            /// \param point: true coordinates
            void center_on(Vector2f);

            /// \brief move the camera
            /// \param x_offset: offset, true coordinates
            /// \param y_offset: offset, true coordinates
            void move(float x_offset, float y_offset);

            /// \brief zoom the camera in without changing its center coordinates
            /// \param factor: zoom offset, relative
            /// \note an offset of 0 will have no change, an offset of 0.5 will double an objects size, an offset of 2 will half an objects size
            void zoom_in(float factor);

            /// \brief zoom the camera out without changing its center coordinates
            /// \param factor: zoom offset, relative
            /// \note an offset of 0 will have no change, an offset of 0.5 will half an objects size, an offset of 2 will double an objects size
            void zoom_out(float factor);

            /// \brief set the camera zoom
            /// \param factor: zoom factor to override, relative
            /// \note setting the zoom to 1 will give no visual change, a factor of 0.5 will double an objects size, a factor 2 will half an objects size
            void set_zoom(float factor);

            /// \brief rotate the camera around its center by `angle` degrees
            /// \param angle: angle offset
            /// \note +angle: clockwise rotation, -angle: counter-clockwise rotation
            void rotate(Angle);

            /// \brief set the rotation of the camera
            /// \param angle: angle to override
            /// \note 0° is no rotation, > 0° is clockwise rotation
            void set_rotation(Angle);

            /// \brief expose the transform, can be modified to affect the camera state
            /// \returns reference to transform
            Transform& get_transform();

            /// \brief replace the transform
            /// \param transform: to replace with
            void set_transform(Transform);

            /// \param get the true coordinates of the cameras center
            /// \returns center
            Vector2f get_center() const;

            /// \param get the true coordinates and size of the cameras view area
            /// \returns trapezoid, not necessarily a rectangle because the transform may shear the image
            Trapezoid get_view_area() const;

        private:
            Window* _window;
    };
}