// 
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <unordered_map>

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

extern "C"
{

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
}

size_t ts_window_create(
    size_t width,
    size_t height,
    const char* title = "",
    bool fullscreen = false,
    bool borderless = false,
    bool resizable = false)
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

    detail::_windows.at(id).create(title, width, height, options);
    return id;
}

void ts_window_destroy(size_t id)
{
    detail::_windows.at(id).close();
    detail::_windows.erase(id);
}

void ts_window_close(size_t id)
{
    detail::_windows.at(id).close();
}

bool ts_window_is_open(size_t id)
{
    return detail::_windows.at(id).is_open();
}

size_t ts_window_get_size_x(size_t id)
{
    return detail::_windows.at(id).get_size().x;
}

size_t ts_window_get_size_y(size_t id)
{
    return detail::_windows.at(id).get_size().y;
}

size_t ts_window_get_position_x(size_t id)
{
    return detail::_windows.at(id).get_position().x;
}

size_t ts_window_get_position_y(size_t id)
{
    return detail::_windows.at(id).get_position().y;
}

void ts_window_set_hidden(size_t id, bool hidden)
{
    detail::_windows.at(id).set_hidden(hidden);
}

bool ts_window_is_hidden(size_t id)
{
    return detail::_windows.at(id).is_hidden();
}

bool ts_window_minimize(size_t id)
{
    detail::_windows.at(id).minimize();
}

bool ts_window_is_minimized(size_t id)
{
    return detail::_windows.at(id).is_minimized();
}

bool ts_window_maximize(size_t id)
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
    size_t _texture_id;
    std::unordered_map<size_t, ts::Texture> _textures;
}

size_t ts_texture_load(size_t window_id, const char* path)
{
    auto id = detail::_texture_id++;
    detail::_textures.emplace(id, ts::Texture(&detail::_windows.at(window_id)));
    detail::_textures.at(id).load(path);
    return id;
}

void ts_texture_unload(size_t texture_id)
{
    detail::_textures.erase(texture_id);
}

void ts_texture_set_color(size_t texture_id, float r, float g, float b, float a)
{
    detail::_textures.at(texture_id).set_color(ts::RGBA(r, g, b, a));
}

// ### SHAPES ##################################################

void ts_shape_render(void* shape_ptr, size_t window_id)
{
    detail::_windows.at(window_id).render(((ts::Shape*) shape_ptr));
}

float ts_shape_get_centroid_x(void* shape_ptr)
{
    return ((ts::Shape*) shape_ptr)->get_centroid().x;
}

float ts_shape_get_centroid_y(void* shape_ptr)
{
    return ((ts::Shape*) shape_ptr)->get_centroid().y;
}

void ts_shape_set_centroid(void* shape_ptr, float x, float y)
{
    ((ts::Shape*) shape_ptr)->set_centroid(ts::Vector2f{x, y});
}

void ts_shape_set_color(void* shape_ptr, float r, float g, float b, float a)
{
    ((ts::Shape*) shape_ptr)->set_color(ts::RGBA(r, g, b, a));
}

void ts_shape_set_texture(void* shape_ptr, size_t texture_id)
{
    ((ts::Shape*) shape_ptr)->set_texture(&detail::_textures.at(texture_id));
}

float ts_shape_get_aabb_x(void* shape_ptr)
{
    ((ts::Shape*) shape_ptr)->get_bounding_box().top_left.x;
}

float ts_shape_get_aabb_y(void* shape_ptr)
{
    ((ts::Shape*) shape_ptr)->get_bounding_box().top_left.y;
}

float ts_shape_get_aabb_width(void* shape_ptr)
{
    ((ts::Shape*) shape_ptr)->get_bounding_box().size.x;
}

float ts_shape_get_aabb_height(void* shape_ptr)
{
    ((ts::Shape*) shape_ptr)->get_bounding_box().size.y;
}

void* ts_shape_new_triangle(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y)
{
    return new ts::TriangleShape({a_x, a_y}, {b_x, b_y}, {c_x, c_y});
}

void ts_shape_destroy_triangle(void* triangle_pointer)
{
    delete ((ts::TriangleShape*) triangle_pointer);
}

void* ts_shape_new_rectangle(float top_left_x, float top_left_y, float width, float height)
{
    return new ts::RectangleShape(ts::Vector2f{top_left_x, top_left_y}, ts::Vector2f{width, height});
}

void* ts_shape_destroy_rectangle(void* rectangle_pointer)
{
    delete ((ts::TriangleShape*) rectangle_pointer);
}

void* ts_shape_new_circle(float center_x, float center_y, float radius)
{
    return new ts::CircleShape(ts::Vector2f{center_x, center_y}, radius);
}

void ts_shape_destroy_circle(void* circle_pointer)
{
    delete ((ts::Circle*) circle_pointer);
}

void* ts_shape_new_polygon(float* vertices_x, float* vertices_y, size_t n_vertices)
{
    std::vector<ts::Vector2f> vertices;
    for (size_t i = 0; i < n_vertices; ++i)
        vertices.push_back(ts::Vector2f(vertices_x[i], vertices_y[i]));

    return new ts::PolygonShape(vertices);
}

void ts_shape_destroy_polygon(void* polygon_pointer)
{
    delete ((ts::PolygonShape*) polygon_pointer);
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

float ts_controller_axis_left_x(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_axis_left(controller_id).x;
}

float ts_controller_axis_left_y(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_axis_left(controller_id).y;
}

float ts_controller_axis_right_x(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_axis_right(controller_id).x;
}

float ts_controller_axis_right_y(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_axis_right(controller_id).y;
}

float ts_controller_trigger_left(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_trigger_left(controller_id);
}

float ts_controller_trigger_right(size_t controller_id = 0)
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

void ts_music_play(size_t id, bool should_loop = true, double fade_in_ms = 0)
{
    ts::MusicHandler::play(detail::_music_library.at(id), should_loop, ts::milliseconds(fade_in_ms));
}

void ts_music_play_next(size_t id, bool should_loop = true, double fade_in_ms = 0)
{
    ts::MusicHandler::play_next(detail::_music_library.at(id), should_loop, ts::milliseconds(fade_in_ms));
}

void ts_music_stop(double fade_out_ms = 0)
{
    ts::MusicHandler::stop(ts::milliseconds(fade_out_ms));
}

void ts_music_next(double fade_out_ms = 0)
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

size_t ts_sound_load(const char* path, float volume = 1)
{
    size_t id = std::hash<std::string>()(path);
    detail::_sound_library.emplace(id, ts::Sound(path));
    detail::_sound_library.at(id).set_volume(volume);
    return id;
}

size_t ts_sound_unload(size_t id)
{
    detail::_sound_library.erase(id);
}

size_t ts_sound_n_channels()
{
    return ts::SoundHandler::n_channels;
}

void ts_sound_play(size_t id, size_t channel, size_t n_loops = 0, double fade_in_ms = 0)
{
    ts::SoundHandler::play(detail::_sound_library.at(id), channel, n_loops, ts::milliseconds(fade_in_ms));
}

void ts_sound_stop(size_t channel, double fade_out_ms = 0)
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