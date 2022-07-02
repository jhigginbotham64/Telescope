//
// Created by clem on 6/2/22.
//

#include <include/camera.hpp>
#include <include/window.hpp>

namespace ts
{
    Camera::Camera(Window* window)
        : _window(window)
    {}

    Transform& Camera::get_transform()
    {
        return _window->_global_transform;
    }

    void Camera::set_transform(Transform in)
    {
        _window->_global_transform = in;
    }

    void Camera::reset()
    {
        _window->_global_transform.reset();
    }

    void Camera::center_on(Vector2f pos)
    {
        int width, height;
        SDL_GetWindowSize(_window->get_native(), &width, &height);
        _window->_global_transform.translate(pos.x - width / 2.f, pos.y - width / 2.f);
    }

    void Camera::move(float x, float y)
    {
        auto to_combine = Transform();
        to_combine.translate(x, y);

        _window->_global_transform.combine(to_combine);
    }

    void Camera::zoom_in(float factor)
    {
        auto to_combine = Transform();
        to_combine.scale(factor, factor);

        _window->_global_transform.combine(to_combine);
    }

    void Camera::zoom_out(float factor)
    {
        zoom_in(1.f / factor);
    }

    void Camera::set_zoom(float factor)
    {
        _window->_global_transform.scale(factor, factor);
    }

    Vector2f Camera::get_center() const
    {
        auto size = _window->get_size();
        Vector2f true_center = {size.x / 2.f, size.y / 2.f};

        return _window->_global_transform.apply_to(true_center);
    }

    void Camera::rotate(Angle angle)
    {
        auto center = get_center();
        auto to_combine = Transform();
        to_combine.rotate(angle, center);
        _window->_global_transform.combine(to_combine);
    }

    void Camera::set_rotation(Angle angle)
    {
        _window->_global_transform.rotate(angle, get_center());
    }

    Trapezoid Camera::get_view_area() const
    {
        auto& t = _window->_global_transform;
        auto size = _window->get_size();

        auto top_left = t.apply_to({0, 0});
        auto top_right = t.apply_to({size.x, 0});
        auto bottom_left = t.apply_to({0, size.y});
        auto bottom_right = t.apply_to({size.x, size.y});

        return Trapezoid{top_left, top_right, bottom_left, bottom_right};
    }
}
