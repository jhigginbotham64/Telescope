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

#include <iostream>
#include <thread>
#include <cassert>

#include <include/logging.hpp>
#include <include/time.hpp>
#include <include/input_handler.hpp>
#include <include/music.hpp>
#include <include/music_handler.hpp>

//https://lazyfoo.net/tutorials/SDL/36_multiple_windows/index.php

void initialize()
{
    // TODO: log & exceptions
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_AudioInit(SDL_GetAudioDriver(0));
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC); // | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_OPS)
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
}

int main()
{
    initialize();

    auto window = ts::Window();
    window.create("window", 800, 600, ts::DEFAULT);

    std::function<void()> test = [](){
        std::cout << "test" << std::endl;
    };

    ts::detail::MusicFunctionHook::function = []() -> void {
        std::cout << "test" << std::endl;
    };

    auto okedesuka = ts::Music("/home/clem/Music/ok_desu_ka.mp3");
    auto music = ts::Music("/home/clem/Music/otherworldly_foe.mp3");
    ts::MusicHandler::play(music, false);
    ts::MusicHandler::play_next(okedesuka, false);

    auto clock = ts::Clock();
    auto target_frame_duration = ts::seconds(1 / 60.f);

    while (window.is_open())
    {
        clock.restart();
        ts::InputHandler::update(&window);
        window.clear();

        if (ts::InputHandler::was_pressed(ts::KeyboardKey::SPACE))
            ts::MusicHandler::next();

        auto to_wait = target_frame_duration.as_microseconds() - clock.elapsed().as_microseconds();
        std::this_thread::sleep_for(std::chrono::microseconds(size_t(to_wait)));
        window.flush();
    }
}

