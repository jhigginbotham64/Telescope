// 
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <unordered_map>

#include <include/music.hpp>
#include <include/music_handler.hpp>

#include <include/sound.hpp>
#include <include/sound_handler.hpp>

extern "C"
{

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