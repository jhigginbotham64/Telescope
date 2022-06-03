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

extern "C" {

void ts_test()
{
    std::cout << "test" << std::endl;
}

// ### TIME ####################################################

namespace detail
{
    static inline size_t _clock_id = 0;
    static inline std::unordered_map<size_t, ts::Clock> _clocks = {};
}

double ts_minutes(double n)
{
    return ts::minutes(n).as_milliseconds();
}

double ts_seconds(double n)
{
    return ts::seconds(n).as_milliseconds();
}

double ts_milliseconds(double n)
{
    return n;
}

double ts_microseconds(double n)
{
    return ts::microseconds(n).as_milliseconds();
}

double ts_nanoseconds(double n)
{
    return ts::nanoseconds(n).as_milliseconds();
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

double ts_clock_elapsed(size_t id)
{
    return detail::_clocks.at(id).elapsed().as_milliseconds();
}

double ts_clock_restart(size_t id)
{
    return detail::_clocks.at(id).restart().as_milliseconds();
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
        bool fullscreen,
        bool borderless,
        bool resizable)
{
    auto id = detail::_window_id++;
    detail::_windows.emplace(id, ts::Window());

    uint32_t options = ts::WindowOptions::DEFAULT;

    if (fullscreen)
        options |= ts::FULLSCREEN;

    if (borderless)
        options |= ts::BORDERLESS;

    if (resizable)
        options |= ts::RESIZABLE;

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

// ### TEXTURES ##################################################

namespace detail
{
    size_t _texture_id = 0;
    std::unordered_map<size_t, std::unique_ptr<ts::Texture>> _textures;
}

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

size_t ts_texture_create_static_texture(size_t window_id,
                                        size_t width, size_t height,
                                        float r, float g, float b, float a)
{
    size_t id = detail::_texture_id++;
    detail::_textures.emplace(id, std::unique_ptr<ts::Texture>(new ts::StaticTexture(&detail::_windows.at(window_id))));
    ((ts::StaticTexture*) detail::_textures.at(id).get())->create(width, height, ts::RGBA(r, g, b, a));
    return id;
}

size_t ts_texture_load_static_texture(size_t window_id, const char* path)
{
    size_t id = detail::_texture_id++;
    detail::_textures.emplace(id, std::unique_ptr<ts::Texture>(new ts::StaticTexture(&detail::_windows.at(window_id))));
    ((ts::StaticTexture*) detail::_textures.at(id).get())->load(path);
    return id;
}

void ts_texture_get_size(size_t texture_id, size_t* out_x, size_t* out_y)
{
    SDL_QueryTexture(detail::_textures.at(texture_id)->get_native(), 0, 0, (int*) out_x, (int*) out_y);
}

void ts_texture_destroy_texture(size_t texture_id)
{
    detail::_textures.erase(texture_id);
}

size_t ts_texture_create_render_texture(size_t window_id, size_t width, size_t height)
{
    size_t id = detail::_texture_id++;
    detail::_textures.emplace(id, std::unique_ptr<ts::Texture>(new ts::RenderTexture(&detail::_windows.at(window_id))));
    ((ts::RenderTexture*) detail::_textures.at(id).get())->create(width, height);
    return id;
}

// ### SHAPES ##################################################

void ts_shape_render(void* shape_ptr, size_t window_id,
                     float t_00, float t_01, float t_02,
                     float t_10, float t_11, float t_12,
                     float t_20, float t_21, float t_22) // transform elements
{
    detail::_windows.at(window_id).render(((ts::Shape*) shape_ptr), ts::Transform({t_00, t_01, t_02, t_10, t_11, t_12, t_20, t_21, t_22}));
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

void ts_shape_get_vertex(void* shape_ptr, size_t vertex_index,
                         float* out_pos_x, float* out_pos_y,
                         float* out_tex_coord_x, float * out_tex_coord_y,
                         float* out_r, float* out_g, float* out_b, float* out_a)
{
    auto* shape = (ts::Shape*) shape_ptr;

    *out_pos_x = shape->get_vertex_position(vertex_index).x;
    *out_pos_y = shape->get_vertex_position(vertex_index).y;
    *out_tex_coord_x = shape->get_vertex_texture_coordinates(vertex_index).x;
    *out_tex_coord_y = shape->get_vertex_texture_coordinates(vertex_index).y;

    auto color = shape->get_vertex_color(vertex_index);
    *out_r = color.red;
    *out_g = color.green;
    *out_b = color.blue;
    *out_a = color.alpha;
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

void ts_shape_set_texture(void* shape_ptr, size_t texture_id)
{
    ((ts::Shape*) shape_ptr)->set_texture(detail::_textures.at(texture_id).get());
}

void* ts_shape_new_triangle(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y)
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

void* ts_shape_new_rectangle(float top_left_x, float top_left_y, float width, float height)
{
    return new ts::RectangleShape(ts::Vector2f{top_left_x, top_left_y}, ts::Vector2f{width, height});
}

void ts_shape_destroy_rectangle(void* rectangle_ptr)
{
    delete ((ts::TriangleShape*) rectangle_ptr);
}

void* ts_shape_new_circle(float center_x, float center_y, float radius, size_t n_vertices)
{
    return new ts::CircleShape(ts::Vector2f{center_x, center_y}, radius, n_vertices);
}

float ts_shape_circle_get_radius(void* circle_ptr)
{
    return ((ts::CircleShape*) circle_ptr)->get_radius();
}

void ts_shape_destroy_circle(void* circle_ptr)
{
    delete ((ts::Circle*) circle_ptr);
}

void* ts_shape_new_polygon(float* vertices_x, float* vertices_y, size_t n_vertices)
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

float ts_mouse_cursor_position_x()
{
    return ts::InputHandler::get_cursor_position().x;
}

float ts_mouse_cursor_position_y()
{
    return ts::InputHandler::get_cursor_position().y;
}

float ts_mouse_scrollwheel_x()
{
    return ts::InputHandler::get_scrollwheel().x;
}

float ts_mouse_scrollwheel_y()
{
    return ts::InputHandler::get_scrollwheel().y;
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

float ts_controller_axis_left_x(size_t controller_id)
{
    return ts::InputHandler::get_controller_axis_left(controller_id).x;
}

float ts_controller_axis_left_y(size_t controller_id)
{
    return ts::InputHandler::get_controller_axis_left(controller_id).y;
}

float ts_controller_axis_right_x(size_t controller_id)
{
    return ts::InputHandler::get_controller_axis_right(controller_id).x;
}

float ts_controller_axis_right_y(size_t controller_id)
{
    return ts::InputHandler::get_controller_axis_right(controller_id).y;
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

void ts_music_skip_to(int ms)
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
    ts::SoundHandler::play(detail::_sound_library.at(id), channel, n_loops, ts::milliseconds(fade_in_ms));
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

void ts_sound_set_panning(size_t channel, size_t zero_to_360_degree)
{
    ts::SoundHandler::set_panning(channel, zero_to_360_degree);
}

size_t ts_sound_get_panning(size_t channel)
{
    return ts::SoundHandler::get_panning(channel);
}

}