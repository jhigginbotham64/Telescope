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

#include <include/logging.hpp>
#include <include/time.hpp>
#include <include/input_handler.hpp>
#include <include/music.hpp>
#include <include/music_handler.hpp>
#include <include/sound.hpp>
#include <include/sound_handler.hpp>

void initialize()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_VideoInit(SDL_GetVideoDriver(1));

    SDL_Vulkan_LoadLibrary(NULL);

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    SDL_AudioInit(SDL_GetAudioDriver(1));
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC); // | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_OPS)
    Mix_OpenAudio(ts::MusicHandler::sample_rate, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_AllocateChannels(ts::SoundHandler::n_channels);
}

int main()
{
    initialize();

    auto window = ts::Window();
    window.create("window", 800, 600, ts::DEFAULT);

    auto clock = ts::Clock();
    auto target_frame_duration = ts::seconds(1 / 60.f);

    auto* instance = SDL_Vulkan_GetVkGetInstanceProcAddr()

    while (window.is_open())
    {
        clock.restart();
        ts::InputHandler::update(&window);
        window.clear();



        auto to_wait = target_frame_duration.as_microseconds() - clock.elapsed().as_microseconds();
        std::this_thread::sleep_for(std::chrono::microseconds(size_t(to_wait)));
        window.flush();
    }
}
