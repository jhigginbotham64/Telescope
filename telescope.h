// 
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <unordered_map>

#include <include/window.hpp>

#include <include/input_handler.hpp>

#include <include/music.hpp>
#include <include/music_handler.hpp>

#include <include/sound.hpp>
#include <include/sound_handler.hpp>

extern "C"
{

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
    auto id = _window_id++;
    detail::_windows.emplace(id, ts::Window());

    uint32_t options = ts::WindowOptions::DEFAULT;

    if (fullscreen)
        option |= ts::FULLSCREEN;

    if (borderless)
        option |= ts::BORDERLESS;

    if (resizable)
        option |= ts::RESIZABLE;

    detail::_windows[id].create(title, width, height, options);
    return id;
}

void ts_window_destroy(size_t id)
{
    detail::_windows[id].close();
    detail::_windows.erase(id);
}

void ts_window_close(size_t id)
{
    detail::_windows[id].close();
}

bool ts_window_is_open(size_t id)
{
    return detail::_windows[id].is_open();
}

size_t ts_window_get_size_x(size_t id)
{
    return detail::_windows[id].get_size().x;
}

size_t ts_window_get_size_y(size_t id)
{
    return detail::_windows[id].get_size().y;
}

size_t ts_window_get_position_x(size_t id)
{
    return detail::_windows[id].get_position().x;
}

size_t ts_window_get_position_y(size_t id)
{
    return detail::_windows[id].get_position().y;
}

void ts_window_set_hidden(size_t id, bool hidden)
{
    detail::_windows[id].set_hidden(hidden);
}

bool ts_window_is_hidden(size_t id)
{
    return detail::_windows[id].is_hidden();
}

bool ts_window_minimize(size_t id)
{
    detail::_windows[id].minimize();
}

bool ts_window_is_minimized(size_t id)
{
    return detail::_windows[id].is_minimized();
}

bool ts_window_maximize(size_t id)
{
    detail::_windows[id].maximize();
}

bool ts_window_is_maximized(size_t id)
{
    return detail::_windows[id].is_maximized();
}

bool ts_window_has_focus(size_t id)
{
    return detail::_windows[id].has_focus();
}

bool ts_window_has_mouse_focus(size_t id)
{
    return detail::_windows[id].has_mouse_focus();
}

void ts_window_clear(size_t id)
{
    detail::_windows[id].clear();
}

void ts_window_flush(size_t id)
{
    detail::_windows[id].flush();
}

// ### INPUT ###################################################

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
    return ts::InputHandler::get_controller_axis_left(controller_id).x
}

float ts_controller_axis_left_y(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_axis_left(controller_id).y
}

float ts_controller_axis_right_x(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_axis_right(controller_id).x
}

float ts_controller_axis_right_y(size_t controller_id = 0)
{
    return ts::InputHandler::get_controller_axis_right(controller_id).y
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
    size_t out = std::hash<std::string>()(path);
    detail::_music_library.emplace(out, ts::Music(path));
    return out;
}

void ts_music_unload(size_t id)
{
    detail::_music_library.erase(id);
}

void ts_music_play(size_t id, bool should_loop = true, int fade_in_ms = 0)
{
    ts::MusicHandler::play(detail::_music_library[id], should_loop, ts::milliseconds(fade_in_ms));
}

void ts_music_play_next(size_t id, bool should_loop = true, int fade_in_ms = 0)
{
    ts::MusicHandler::play_next(id, should_loop, fade_in_ms);
}

void ts_music_stop(int fade_out_ms = 0)
{
    ts::MusicHandler::stop(ts::milliseconds(fade_in_ms));
}

void ts_music_next(int fade_out_ms = 0)
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

void ts_music_get_volume()
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
    size_t out = std::hash<std::string>()(path);
    detail::_sound_library.emplace(out, ts::Sound(path));
    detail::_sound_library[out].set_volume(volume);
    return out;
}

size_t ts_sound_unload(size_t id)
{
    detail::_sound_library.erase(id);
}

size_t ts_sound_n_channels()
{
    return ts::SoundHandler::n_channels;
}

void ts_sound_play(size_t id, size_t channel, size_t n_loops = 0, int fade_in_ms = 0)
{
    ts::SoundHandler::play(detail::_sound_library[id], channel, n_loops, ts::milliseconds(fade_in_ms));
}

void ts_sound_stop(size_t id, int fade_out_ms = 0)
{
    ts::SoundHandler::stop(detail::_sound_library[id], ts::milliseconds(fade_out_ms));
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
    ts::SoundHandler::get_panning(channel);
}

}