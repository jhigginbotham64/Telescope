// 
// Copyright 2022 Clemens Cords
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <thread>
#include <cassert>

#include <SDL2/SDL2_gfxPrimitives.h>

#include <include/exceptions.hpp>
#include <include/logging.hpp>
#include <include/time.hpp>
#include <include/input_handler.hpp>
#include <include/music.hpp>
#include <include/music_handler.hpp>
#include <include/sound.hpp>
#include <include/sound_handler.hpp>
#include <include/triangle_shape.hpp>
#include <include/circle_shape.hpp>
#include <include/rectangle_shape.hpp>

void initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        ts::Log::warning("Unable to initialize SDL");
        ts::forward_sdl_error();
    }

    if (SDL_VideoInit(SDL_GetVideoDriver(0)) != 0)
    {
        ts::Log::warning("Unable to initialize SDL Video");
        ts::forward_sdl_error();
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) // sic
    {
        ts::Log::warning("Unable to initialize SDL IMG");
        ts::forward_sdl_error();
    }
    
    if (SDL_AudioInit(SDL_GetAudioDriver(1)) != 0)
    {
        ts::Log::warning("Unable to initialize SDL Audio");
        ts::forward_sdl_error();
    }
    
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC) == 0)
    {
        ts::Log::warning("Unable to initialize SDL Mixer");
        ts::forward_sdl_error();
    }

    Mix_OpenAudio(ts::MusicHandler::sample_rate, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_AllocateChannels(ts::SoundHandler::n_channels);
}

using namespace ts;

int main()
{
    initialize();

    auto window = ts::Window();
    window.create("", 800, 600, ts::DEFAULT);

    auto clock = ts::Clock();
    auto target_frame_duration = ts::seconds(1 / 60.f);
    auto tri = ts::TriangleShape(Vector2f(400, 150), Vector2f(200, 450), Vector2f(600, 450));
    auto rect = ts::RectangleShape(50, 50, 400, 200);
    auto circ = ts::CircleShape({400, 300}, 100, 64);

    auto tex = ts::Texture(&window);
    tex.load("/home/clem/Workspace/backup/bckp2/bg.png");
    tri.set_texture(&tex);
    rect.set_texture(&tex);
    circ.set_texture(&tex);

    std::array<Sint16, 3> x = {400, 200, 600};
    std::array<Sint16, 3> y = {150, 450, 450};

    size_t n = 1;

    while (window.is_open())
    {
        clock.restart();
        ts::InputHandler::update(&window);
        window.clear();

        circ.render(&window);
        circ.set_position(circ.get_position() + Vector2f(sin(n * M_PI)))

        window.flush();
        auto to_wait = target_frame_duration.as_microseconds() - clock.elapsed().as_microseconds();
        std::this_thread::sleep_for(std::chrono::microseconds(size_t(to_wait)));
    }
}
