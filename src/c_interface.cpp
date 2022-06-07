//
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#include <unordered_map>
#include <memory>
#include <iostream>

#include <include/common.hpp>
#include <include/window.hpp>
#include <include/key_or_button.hpp>
#include <include/input_handler.hpp>
#include <include/music.hpp>
#include <include/music_handler.hpp>
#include <include/sound.hpp>
#include <include/sound_handler.hpp>
#include <include/rectangle_shape.hpp>
#include <include/triangle_shape.hpp>
#include <include/circle_shape.hpp>
#include <include/polygon_shape.hpp>
#include <include/texture.hpp>
#include <include/static_texture.hpp>
#include <include/render_texture.hpp>
#include <include/camera.hpp>
#include <include/angle.hpp>

extern "C" {

void ts_test(float* x)
{
    *x = 1234;
}

// ### TIME ####################################################

namespace detail
{
    static inline size_t _clock_id = 0;
    static inline std::unordered_map<size_t, ts::Clock> _clocks = {};
}

double ts_ns_to_minutes(size_t n)
{
    return ts::nanoseconds(n).as_minutes();
}

double ts_ns_to_seconds(size_t n)
{
    return ts::nanoseconds(n).as_seconds();
}

double ts_ns_to_milliseconds(size_t n)
{
    return ts::nanoseconds(n).as_milliseconds();
}

double ts_ns_to_microseconds(size_t n)
{
    return ts::nanoseconds(n).as_microseconds();
}

size_t ts_ns_to_nanoseconds(size_t n)
{
    return ts::nanoseconds(n).as_nanoseconds();
}

size_t ts_minutes_to_ns(double n)
{
    return ts::minutes(n).as_nanoseconds();
}

size_t ts_seconds_to_ns(double n)
{
    return ts::seconds(n).as_nanoseconds();
}

size_t ts_milliseconds_to_ns(double n)
{
    return ts::milliseconds(n).as_nanoseconds();
}

size_t ts_microseconds_to_ns(double n)
{
    return ts::microseconds(n).as_nanoseconds();
}

size_t ts_nanoseconds_to_ns(size_t n)
{
    return ts::nanoseconds(n).as_nanoseconds();
}

size_t ts_clock_create()
{
    size_t id = detail::_clock_id++;
    detail::_clocks.emplace(id, ts::Clock());
    return id;
}

void ts_clock_destroy(size_t id)
{
    detail::_clocks.erase(id);
}

size_t ts_clock_elapsed(size_t id)
{
    return detail::_clocks.at(id).elapsed().as_nanoseconds();
}

size_t ts_clock_restart(size_t id)
{
    return detail::_clocks.at(id).restart().as_nanoseconds();
}

// ### ANGLE ###################################################

float ts_degrees_to_radians(float degrees)
{
    return ts::degrees(degrees).as_radians();
}

float ts_radians_to_degrees(float radians)
{
    return ts::radians(radians).as_degrees();
}

// ### WINDOW ##################################################

namespace detail
{
    static inline size_t _window_id = 0;
    static inline std::unordered_map<size_t, ts::Window> _windows = {};
    static inline std::unordered_map<size_t, ts::Camera> _cameras = {};
}

size_t ts_window_create(
        size_t width,
        size_t height,
        const char* title,
        uint32_t options)
{
    auto id = detail::_window_id++;
    detail::_windows.emplace(id, ts::Window());
    auto& window = detail::_windows.at(id);
    window.create(title, width, height, options);
    detail::_cameras.emplace(id, ts::Camera(&window));

    return id;
}

void ts_window_destroy(size_t id)
{
    detail::_windows.at(id).close();
    detail::_windows.erase(id);
    detail::_cameras.erase(id);
}

void ts_window_close(size_t id)
{
    detail::_windows.at(id).close();
}

bool ts_window_is_open(size_t id)
{
    return detail::_windows.at(id).is_open();
}

void ts_window_get_size(size_t window_id, size_t* out_x, size_t* out_y)
{
    SDL_GetWindowSize(detail::_windows.at(window_id).get_native(), (int*) out_x, (int*) out_y);
}

void ts_window_get_position(size_t window_id, size_t* out_x, size_t* out_y)
{
    SDL_GetWindowPosition(detail::_windows.at(window_id).get_native(), (int*) out_x, (int*) out_y);
}

void ts_window_set_hidden(size_t id, bool hidden)
{
    detail::_windows.at(id).set_hidden(hidden);
}

bool ts_window_is_hidden(size_t id)
{
    return detail::_windows.at(id).is_hidden();
}

void ts_window_minimize(size_t id)
{
    detail::_windows.at(id).minimize();
}

bool ts_window_is_minimized(size_t id)
{
    return detail::_windows.at(id).is_minimized();
}

void ts_window_maximize(size_t id)
{
    detail::_windows.at(id).maximize();
}

bool ts_window_is_maximized(size_t id)
{
    return detail::_windows.at(id).is_maximized();
}

bool ts_window_has_focus(size_t id)
{
    return detail::_windows.at(id).has_focus();
}

bool ts_window_has_mouse_focus(size_t id)
{
    return detail::_windows.at(id).has_mouse_focus();
}

void ts_window_clear(size_t id)
{
    detail::_windows.at(id).clear();
}

void ts_window_flush(size_t id)
{
    detail::_windows.at(id).flush();
}

void ts_window_render(size_t id, void* renderable_ptr, void* transform_ptr)
{
    detail::_windows.at(id).render((ts::Renderable*) renderable_ptr, *((ts::Transform*) transform_ptr));
}

// ### CAMERA ##################################################

void ts_window_camera_center_on(size_t window_id, float x, float y)
{
    detail::_cameras.at(window_id).center_on(ts::Vector2f{x, y});
}

void ts_window_camera_move(size_t window_id, float x, float y)
{
    detail::_cameras.at(window_id).move(x, y);
}

void ts_window_camera_zoom_in(size_t window_id, float factor)
{
    detail::_cameras.at(window_id).zoom_in(factor);
}

void ts_window_camera_zoom_out(size_t window_id, float factor)
{
    detail::_cameras.at(window_id).zoom_out(factor);
}

void ts_window_camera_set_zoom(size_t window_id, float factor)
{
    detail::_cameras.at(window_id).set_zoom(factor);
}

void ts_window_camera_rotate(size_t window_id, float degrees)
{
    detail::_cameras.at(window_id).rotate(ts::degrees(degrees));
}

void* ts_window_camera_get_transform(size_t window_id)
{
    return (void*) &detail::_cameras.at(window_id).get_transform();
}

void ts_window_camera_set_rotation(size_t window_id, float degrees)
{
    detail::_cameras.at(window_id).set_rotation(ts::degrees(degrees));
}

void ts_window_camera_get_center(size_t window_id, float* out_x, float* out_y)
{
    auto center = detail::_cameras.at(window_id).get_center();
    *out_x = center.x;
    *out_y = center.y;
}

void ts_window_camera_get_view_area(size_t window_id,
                                    float* out_top_left_x, float* out_top_left_y,
                                    float* out_top_right_x, float* out_top_right_y,
                                    float* out_bottom_left_x, float* out_bottom_left_y,
                                    float* out_bottom_right_x, float* out_bottom_right_y)
{
    auto rect = detail::_cameras.at(window_id).get_view_area();
    *out_top_left_x = rect.top_left.x;
    *out_top_left_y = rect.top_left.y;
    *out_top_right_x = rect.top_right.x;
    *out_top_right_y = rect.top_right.y;
    *out_bottom_left_x = rect.bottom_left.x;
    *out_bottom_left_y = rect.bottom_left.y;
    *out_bottom_right_x = rect.bottom_right.x;
    *out_bottom_right_y = rect.bottom_right.y;
}

// ### COMMON ##################################################

bool ts_initialize()
{
    return ts::initialize();
}

void ts_set_framerate_limit(size_t frames_per_second)
{
    ts::set_framerate_limit(frames_per_second);
}

void ts_start_frame(size_t window_id)
{
    ts::start_frame(&detail::_windows.at(window_id));
}

void ts_end_frame(size_t window_id)
{
    ts::end_frame(&detail::_windows.at(window_id));
}

// ### COLORS ####################################################

void ts_rgb_to_hsv(float r, float g, float b, float* out_h, float* out_s, float* out_v)
{
    auto in = ts::RGBA(r, g, b, 1);
    auto out = in.as_hsv();
    *out_h = out.hue;
    *out_s = out.saturation;
    *out_v = out.value;
}

void ts_hsv_to_rgb(float h, float s, float v, float* out_r, float* out_g, float* out_b)
{
    auto in = ts::HSVA(h, s, v, 1);
    auto out = in.as_rgb();
    *out_r = out.red;
    *out_g = out.green;
    *out_b = out.blue;
}

// ### TEXTURES ##################################################

int32_t ts_texture_filtering_mode_nearest_neighbour()
{
    return ts::TextureFilteringMode::NEAREST_NEIGHBOUR;
}

int32_t ts_texture_filtering_mode_linear()
{
    return ts::TextureFilteringMode::LINEAR;
}

int32_t ts_texture_filtering_mode_anisotropic()
{
    return ts::TextureFilteringMode::ANISOTROPIC;
}

int32_t ts_texture_blend_mode_none()
{
    return ts::TextureBlendMode::NONE;
}

int32_t ts_texture_blend_mode_alpha()
{
    return ts::TextureBlendMode::ALPHA;
}

int32_t ts_texture_blend_mode_add()
{
    return ts::TextureBlendMode::ADD;
}

int32_t ts_texture_blend_mode_multiply()
{
    return ts::TextureBlendMode::MULTIPLY;
}

void* ts_texture_create_static_texture(size_t window_id,
                                        size_t width, size_t height,
                                        float r, float g, float b, float a)
{
    auto* out = new ts::StaticTexture(&detail::_windows.at(window_id));
    out->create(width, height, ts::RGBA(r, g, b, a));
    return out;
}

void* ts_texture_load_static_texture(size_t window_id, const char* path)
{
    auto* out = new ts::StaticTexture(&detail::_windows.at(window_id));
    out->load(path);
    return out;
}

void ts_texture_get_size(void* texture, size_t* out_x, size_t* out_y)
{
    SDL_QueryTexture(((ts::Texture*) texture)->get_native(), 0, 0, (int*) out_x, (int*) out_y);
}

void ts_texture_destroy_texture(void* texture)
{
    delete ((ts::Texture*) texture);
}

void* ts_texture_create_render_texture(size_t window_id, size_t width, size_t height)
{
    auto* out = new ts::RenderTexture(&detail::_windows.at(window_id));
    out->create(width, height);
    return out;
}

void ts_texture_set_color(void* texture, float r, float g, float b, float a)
{
    ((ts::Texture*) texture)->set_color(ts::RGBA(r, g, b, a));
}

void ts_texture_get_color(void* texture, float* out_r, float* out_g, float* out_b, float* out_a)
{
    auto color = ((ts::Texture*) texture)->get_color();
    *out_r = color.red;
    *out_g = color.green;
    *out_b = color.blue;
    *out_a = color.alpha;
}


void ts_texture_set_blend_mode(void* texture, int32_t blend_mode)
{
    ((ts::Texture*) texture)->set_blend_mode(ts::TextureBlendMode(blend_mode));
}

int32_t ts_texture_get_blend_mode(void* texture)
{
    return (int32_t) ((ts::Texture*) texture)->get_blend_mode();
}

void ts_texture_set_filtering_mode(void* texture, int32_t filtering_mode)
{
    ((ts::Texture*) texture)->set_filtering_mode(ts::TextureFilteringMode(filtering_mode));
}

int32_t ts_texture_get_filtering_mode(void* texture)
{
    return (int32_t) ((ts::Texture*) texture)->get_filtering_mode();
}


// ### TRANSFORMS ##################################################

void* ts_transform_create()
{
    return new ts::Transform();
}

void ts_transform_destroy(void* transform_ptr)
{
    delete ((ts::Transform*) transform_ptr);
}

void ts_transform_set(void* transform_ptr, size_t x, size_t y, float value)
{
    ((ts::Transform*) transform_ptr)->get_native()[x][y] = value;
}

float ts_transform_get(void* transform_ptr, size_t x, size_t y)
{
    return ((ts::Transform*) transform_ptr)->get_native()[x][y];
}

void ts_transform_apply_to(void* transform_ptr, float x, float y, float* out_x, float* out_y)
{
    auto out = ((ts::Transform*) transform_ptr)->apply_to(ts::Vector2f{x, y});
    *out_x = out.x;
    *out_y = out.y;
}

void ts_transform_reset(void* transform_ptr)
{
    ((ts::Transform*) transform_ptr)->reset();
}

void ts_transform_combine(void* left, void* right)
{
    ((ts::Transform*) left)->combine(*((ts::Transform*) right));
}

void ts_transform_translate(void* transform_ptr, float x, float y)
{
    ((ts::Transform*) transform_ptr)->translate(x, y);
}

void ts_transform_rotate(void* transform_ptr, float degrees, float origin_x, float origin_y)
{
    ((ts::Transform*) transform_ptr)->rotate(ts::degrees(degrees), ts::Vector2f{origin_x, origin_y});
}

void ts_transform_scale(void* transform_ptr, float x_scale, float y_scale)
{
    ((ts::Transform*) transform_ptr)->scale(x_scale, y_scale);
}

void ts_transform_shear(void* transform_ptr, float x_scale, float y_scale)
{
    ((ts::Transform*) transform_ptr)->shear(x_scale, y_scale);
}

void ts_transform_reflect(void* transform_ptr, bool about_x_axis, bool about_y_axis, float origin_x, float origin_y)
{
    ((ts::Transform*) transform_ptr)->reflect(about_x_axis, about_y_axis, ts::Vector2f{origin_x, origin_y});
}

// ### SHAPES ##################################################

void ts_shape_render_to_window(void* shape_ptr, size_t window_id, void* transform_ptr)
{
    detail::_windows.at(window_id).render(((ts::Shape*) shape_ptr), *((ts::Transform*) transform_ptr));
}

void ts_shape_render_to_texture(void* shape_ptr, void* render_texture_ptr, void* transform_ptr)
{
    ((ts::RenderTexture*) render_texture_ptr)->render((ts::Shape*) shape_ptr, *((ts::Transform*) transform_ptr));
}

void ts_shape_move(void* shape_ptr, float x, float y)
{
    ((ts::Shape*) shape_ptr)->move(x, y);
}

void ts_shape_get_centroid(void* shape_ptr, int* out_x, int* out_y)
{
    auto centroid = ((ts::Shape*) shape_ptr)->get_centroid();
    *out_x = centroid.x;
    *out_y = centroid.y;
}

void ts_shape_set_centroid(void* shape_ptr, float x, float y)
{
    ((ts::Shape*) shape_ptr)->set_centroid(ts::Vector2f{x, y});
}

size_t ts_shape_get_n_vertices(void* shape_ptr)
{
    return ((ts::Shape*) shape_ptr)->get_n_vertices();
}

void ts_shape_set_color(void* shape_ptr, float r, float g, float b, float a)
{
    ((ts::Shape*) shape_ptr)->set_color(ts::RGBA(r, g, b, a));
}

void ts_shape_set_vertex_color(void* shape_ptr, size_t vertex_index, float r, float g, float b, float a)
{
    ((ts::Shape*) shape_ptr)->set_vertex_color(vertex_index, ts::RGBA(r, g, b, a));
}

void ts_shape_get_vertex_color(void* shape_ptr, size_t vertex_index, float* out_r, float* out_g, float* out_b, float* out_a)
{
    auto color = ((ts::Shape*) shape_ptr)->get_color(vertex_index);
    *out_r = color.red;
    *out_g = color.green;
    *out_b = color.blue;
    *out_a = color.alpha;
}

void ts_shape_set_vertex_texture_coordinates(void* shape_ptr, size_t vertex_index, float u, float v)
{
    ((ts::Shape*) shape_ptr)->set_vertex_texture_coordinates(vertex_index, ts::Vector2f{u, v});
}

void ts_shape_get_vertex_texture_coordinates(void* shape_ptr, size_t vertex_index, float* out_u, float* out_v)
{
    auto uv = ((ts::Shape*) shape_ptr)->get_vertex_texture_coordinates(vertex_index);
    *out_u = uv.x;
    *out_v = uv.y;
}

void ts_shape_set_vertex_position(void* shape_ptr, size_t vertex_index, float x, float y)
{
    ((ts::Shape*) shape_ptr)->set_vertex_position(vertex_index, ts::Vector2f{x, y});
}

void ts_shape_get_vertex_position(void* shape_ptr, size_t vertex_index, float* out_x, float* out_y)
{
    auto pos = ((ts::Shape*) shape_ptr)->get_vertex_position(vertex_index);
    *out_x = pos.x;
    *out_y = pos.y;
}

void ts_shape_set_texture_rectangle(void* shape_ptr, float top_left_x, float top_left_y, float width, float height)
{
    ((ts::Shape*) shape_ptr)->set_texture_rectangle(ts::Rectangle{{top_left_x, top_left_y}, {width, height}});
}

void ts_shape_get_texture_rectangle(void* shape_ptr, float* out_top_left_x, float* out_top_left_y, float* out_width, float* out_height)
{
    auto out = ((ts::Shape*) shape_ptr)->get_texture_rectangle();
    *out_top_left_x = out.top_left.x;
    *out_top_left_y = out.top_left.y;
    *out_width = out.size.x;
    *out_height = out.size.y;
}

void ts_shape_get_bounding_box(void* shape_ptr, float* out_top_left_x, float* out_top_left_y, float* out_width, float* out_height)
{
    auto out = ((ts::Shape*) shape_ptr)->get_bounding_box();
    *out_top_left_x = out.top_left.x;
    *out_top_left_y = out.top_left.y;
    *out_width = out.size.x;
    *out_height = out.size.y;
}

void ts_shape_set_texture(void* shape_ptr, void* texture)
{
    ((ts::Shape*) shape_ptr)->set_texture((ts::Texture*) texture);
}

void* ts_shape_get_texture(void* shape_ptr)
{
    return ((ts::Shape*) shape_ptr)->get_texture();
}

void* ts_shape_create_triangle(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y)
{
    return new ts::TriangleShape({a_x, a_y}, {b_x, b_y}, {c_x, c_y});
}

void ts_shape_triangle_get_vertices(void* triangle_ptr,
                                    float* out_a_x, float* out_a_y,
                                    float* out_b_x, float* out_b_y,
                                    float* out_c_x, float* out_c_y)
{
    auto* tri = ((ts::Triangle*) triangle_ptr);
    *out_a_x = tri->a.x;
    *out_a_y = tri->a.y;
    *out_b_x = tri->b.x;
    *out_b_y = tri->b.y;
    *out_c_x = tri->c.x;
    *out_c_y = tri->c.y;
}

void ts_shape_destroy_triangle(void* triangle_ptr)
{
    delete ((ts::TriangleShape*) triangle_ptr);
}

void* ts_shape_create_rectangle(float top_left_x, float top_left_y, float width, float height)
{
    return new ts::RectangleShape(ts::Vector2f{top_left_x, top_left_y}, ts::Vector2f{width, height});
}

void ts_shape_destroy_rectangle(void* rectangle_ptr)
{
    delete ((ts::TriangleShape*) rectangle_ptr);
}

void ts_shape_rectangle_set_top_left(void* rectangle_ptr, float top_left_x, float top_left_y)
{
    ((ts::RectangleShape*) rectangle_ptr)->set_top_left(ts::Vector2f{top_left_x, top_left_y});
}

void ts_shape_rectangle_get_top_left(void* rectangle_ptr, float* out_top_left_x, float* out_top_left_y)
{
    auto out = ((ts::RectangleShape*) rectangle_ptr)->get_top_left();
    *out_top_left_x = out.x;
    *out_top_left_y = out.y;
}

void ts_shape_rectangle_set_size(void* rectangle_ptr, float width, float height)
{
    ((ts::RectangleShape*) rectangle_ptr)->set_size(ts::Vector2f{width, height});
}

void ts_shape_rectangle_get_size(void* rectangle_ptr, float* out_width, float* out_height)
{
    auto out = ((ts::RectangleShape*) rectangle_ptr)->get_top_left();
    *out_width = out.x;
    *out_height = out.y;
}

void* ts_shape_create_circle(float center_x, float center_y, float radius, size_t n_vertices)
{
    return new ts::CircleShape(ts::Vector2f{center_x, center_y}, radius, n_vertices);
}

float ts_shape_circle_get_radius(void* circle_ptr)
{
    return ((ts::CircleShape*) circle_ptr)->get_radius();
}

void ts_shape_circle_set_radius(void* circle_ptr, float radius)
{
    ((ts::CircleShape*) circle_ptr)->set_radius(radius);
}

void ts_shape_destroy_circle(void* circle_ptr)
{
    delete ((ts::Circle*) circle_ptr);
}

void* ts_shape_create_polygon(float* vertices_x, float* vertices_y, size_t n_vertices)
{
    std::vector<ts::Vector2f> vertices;
    for (size_t i = 0; i < n_vertices; ++i)
        vertices.push_back(ts::Vector2f(vertices_x[i], vertices_y[i]));

    return new ts::PolygonShape(vertices);
}

void ts_shape_destroy_polygon(void* polygon_ptr)
{
    delete ((ts::PolygonShape*) polygon_ptr);
}

// ### INPUT ###################################################

void ts_input_update()
{
    std::vector<ts::Window*> windows;
    windows.reserve(detail::_windows.size());

    for (auto& pair : detail::_windows)
        windows.push_back(&pair.second);

    ts::InputHandler::update(windows);
}

int64_t ts_keyboard_key(const char* id)
{
    return (int64_t) ts::detail::string_to_keyboard_key(id);
}

int64_t ts_mouse_button(const char* id)
{
    return (int64_t) ts::detail::string_to_mouse_button(id);
}

int64_t ts_controller_button(const char* id)
{
    return (int64_t) ts::detail::string_to_controller_button(id);
}

bool ts_keyboard_is_down(int64_t key)
{
    return ts::InputHandler::is_down((ts::KeyboardKey) key);
}

bool ts_keyboard_has_state_changed(int64_t key)
{
    return ts::InputHandler::has_state_changed((ts::KeyboardKey) key);
}

bool ts_keyboard_was_pressed(int64_t key)
{
    return ts::InputHandler::was_pressed((ts::KeyboardKey) key);
}

bool ts_keyboard_was_released(int64_t key)
{
    return ts::InputHandler::was_released((ts::KeyboardKey) key);
}

bool ts_mouse_is_down(int64_t button)
{
    return ts::InputHandler::is_down((ts::MouseButton) button);
}

bool ts_mouse_has_state_changed(int64_t button)
{
    return ts::InputHandler::has_state_changed((ts::MouseButton) button);
}

bool ts_mouse_was_pressed(int64_t button)
{
    return ts::InputHandler::was_pressed((ts::MouseButton) button);
}

bool ts_mouse_was_released(int64_t button)
{
    return ts::InputHandler::was_released((ts::MouseButton) button);
}

void ts_mouse_cursor_position(float* out_x, float* out_y)
{
    auto pos = ts::InputHandler::get_cursor_position();
    *out_x = pos.x;
    *out_y = pos.y;
}


void ts_mouse_scrollwheel(float* out_x, float* out_y)
{
    auto pos = ts::InputHandler::get_scrollwheel();
    *out_x = pos.x;
    *out_y = pos.y;
}

bool ts_controller_is_down(int64_t button, size_t controller_id)
{
    return ts::InputHandler::is_down((ts::ControllerButton) button, controller_id);
}

bool ts_controller_has_state_changed(int64_t button, size_t controller_id)
{
    return ts::InputHandler::has_state_changed((ts::ControllerButton) button, controller_id);
}

bool ts_controller_was_pressed(int64_t button, size_t controller_id)
{
    return ts::InputHandler::was_pressed((ts::ControllerButton) button, controller_id);
}

bool ts_controller_was_released(int64_t button, size_t controller_id)
{
    return ts::InputHandler::was_released((ts::ControllerButton) button, controller_id);
}

void ts_controller_axis_left(size_t controller_id, float* out_x, float* out_y)
{
    auto state = ts::InputHandler::get_controller_axis_left(controller_id);
    *out_x = state.x;
    *out_y = state.y;
}

void ts_controller_axis_right(size_t controller_id, float* out_x, float* out_y)
{
    auto state = ts::InputHandler::get_controller_axis_right(controller_id);
    *out_x = state.x;
    *out_y = state.y;
}

float ts_controller_trigger_left(size_t controller_id)
{
    return ts::InputHandler::get_controller_trigger_left(controller_id);
}

float ts_controller_trigger_right(size_t controller_id)
{
    return ts::InputHandler::get_controller_trigger_right(controller_id);
}

// ### MUSIC ###################################################

namespace detail
{
    static inline std::unordered_map<size_t, ts::Music> _music_library = {};
}

size_t ts_music_sample_rate()
{
    return ts::MusicHandler::sample_rate;
}

size_t ts_music_load(const char* path)
{
    size_t id = std::hash<std::string>()(path);
    detail::_music_library.emplace(id, ts::Music(path));
    return id;
}

void ts_music_unload(size_t id)
{
    detail::_music_library.erase(id);
}

void ts_music_play(size_t id, bool should_loop, double fade_in_ms)
{
    ts::MusicHandler::play(detail::_music_library.at(id), should_loop, ts::milliseconds(fade_in_ms));
}

void ts_music_play_next(size_t id, bool should_loop, double fade_in_ms)
{
    ts::MusicHandler::play_next(detail::_music_library.at(id), should_loop, ts::milliseconds(fade_in_ms));
}

void ts_music_stop(double fade_out_ms)
{
    ts::MusicHandler::stop(ts::milliseconds(fade_out_ms));
}

void ts_music_next(double fade_out_ms)
{
    ts::MusicHandler::next(ts::milliseconds(fade_out_ms));
}

void ts_music_clear_next()
{
    ts::MusicHandler::clear_next();
}

size_t ts_music_get_next()
{
    return ts::MusicHandler::get_next()->get_id();
}

void ts_music_force_stop()
{
    ts::MusicHandler::force_stop();
}

void ts_music_pause()
{
    ts::MusicHandler::pause();
}

void ts_music_unpause()
{
    ts::MusicHandler::unpause();
}

void ts_music_skip_to(double ms)
{
    ts::MusicHandler::skip_to(ts::milliseconds(ms));
}

bool ts_music_is_playing()
{
    return ts::MusicHandler::is_playing();
}

bool ts_music_is_paused()
{
    return ts::MusicHandler::is_paused();
}

bool ts_music_is_stopped()
{
    return ts::MusicHandler::is_stopped();
}

void ts_music_set_volume(float zero_to_one)
{
    ts::MusicHandler::set_volume(zero_to_one);
}

float ts_music_get_volume()
{
    return ts::MusicHandler::get_volume();
}

// ### SOUNDS ##################################################

namespace detail
{
    std::unordered_map<size_t, ts::Sound> _sound_library;
}

size_t ts_sound_get_max_n_channels()
{
    return ts::SoundHandler::n_channels;
}

size_t ts_sound_load(const char* path, float volume)
{
    size_t id = std::hash<std::string>()(path);
    detail::_sound_library.emplace(id, ts::Sound(path));
    detail::_sound_library.at(id).set_volume(volume);
    return id;
}

void ts_sound_unload(size_t id)
{
    detail::_sound_library.erase(id);
}

size_t ts_sound_n_channels()
{
    return ts::SoundHandler::n_channels;
}

void ts_sound_play(size_t id, size_t channel, size_t n_loops, double fade_in_ms)
{
    ts::SoundHandler::play(channel, detail::_sound_library.at(id), n_loops, ts::milliseconds(fade_in_ms));
}

void ts_sound_stop(size_t channel, double fade_out_ms)
{
    ts::SoundHandler::stop(channel, ts::milliseconds(fade_out_ms));
}

void ts_sound_pause(size_t channel)
{
    ts::SoundHandler::pause(channel);
}

void ts_sound_unpause(size_t channel)
{
    ts::SoundHandler::unpause(channel);
}

void ts_sound_force_stop(size_t channel)
{
    ts::SoundHandler::force_stop(channel);
}

bool ts_sound_is_playing(size_t channel)
{
    return ts::SoundHandler::is_playing(channel);
}

bool ts_sound_is_paused(size_t channel)
{
    return ts::SoundHandler::is_paused(channel);
}

bool ts_sound_is_stopped(size_t channel)
{
    return ts::SoundHandler::is_stopped(channel);
}

void ts_sound_set_volume(size_t channel, float zero_to_one)
{
    ts::SoundHandler::set_volume(channel, zero_to_one);
}

float ts_sound_get_volume(size_t channel)
{
    return ts::SoundHandler::get_volume(channel);
}

void ts_sound_set_panning(size_t channel, float zero_to_360_degree)
{
    ts::SoundHandler::set_panning(channel, ts::degrees(zero_to_360_degree));
}

float ts_sound_get_panning(size_t channel)
{
    return ts::SoundHandler::get_panning(channel).as_degrees();
}

}