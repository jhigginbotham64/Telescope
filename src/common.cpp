//
// Copyright 2022 Joshua Higginbotham
// Created on 5/31/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <thread>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include <include/window.hpp>
#include <include/input_handler.hpp>
#include <include/logging.hpp>
#include <include/exceptions.hpp>
#include <include/music_handler.hpp>
#include <include/sound_handler.hpp>

namespace ts
{
    bool initialize()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            ts::Log::warning("In ts::initialize: Unable to initialize SDL");
            ts::forward_sdl_error();
            return false;
        }

        if (SDL_VideoInit(SDL_GetVideoDriver(0)) != 0)
        {
            ts::Log::warning("In ts::initialize: Unable to initialize SDL Video");
            ts::forward_sdl_error();
            return false;
        }

        if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) // sic
        {
            ts::Log::warning("In ts::initialize: Unable to initialize SDL IMG");
            ts::forward_sdl_error();
            return false;
        }

        if (SDL_AudioInit(SDL_GetAudioDriver(1)) != 0)
        {
            ts::Log::warning("In ts::initialize: Unable to initialize SDL Audio");
            ts::forward_sdl_error();
            return false;
        }

        if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC) == 0)
        {
            ts::Log::warning("In ts::initialize: Unable to initialize SDL Mixer");
            ts::forward_sdl_error();
            return false;
        }

        if (Mix_OpenAudio(ts::MusicHandler::sample_rate, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
        {
            ts::Log::warning("In ts::initialize: Unable to initialize Music Handler");
            ts::forward_sdl_error();
            return false;
        }

        if (Mix_AllocateChannels(ts::SoundHandler::n_channels) != ts::SoundHandler::n_channels)
        {
            ts::Log::warning("In ts::initialize: Unable to initialize Audio Handler");
            ts::forward_sdl_error();
            return false;
        }

        ts::Log::print("initialization successful");
        return true;
    }

    namespace detail
    {
        static inline size_t _target_fps = 60;
        static inline Clock _frame_clock = ts::Clock();
    }

    void set_framerate_limit(size_t frames_per_second)
    {
        detail::_target_fps = frames_per_second;
    }

    ts::Time start_frame(std::vector<Window*> windows)
    {
        ts::InputHandler::update(windows);
        return detail::_frame_clock.restart();
    }

    ts::Time start_frame(Window* window)
    {
        return start_frame(std::vector<Window*>{window});
    }

    void end_frame(std::vector<Window*> windows)
    {
        for (auto* w : windows)
            w->flush();

        auto to_wait = ts::seconds(1.f / detail::_target_fps).as_microseconds() - detail::_frame_clock.elapsed().as_microseconds();
        std::this_thread::sleep_for(std::chrono::microseconds(size_t(to_wait)));
    }

    void end_frame(Window* window)
    {
        end_frame(std::vector<Window*>{window});
    }
}