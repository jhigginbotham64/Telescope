// 
// Copyright 2022 Clemens Cords
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_events.h>

#include <iostream>
#include <thread>

#include <include/logging.hpp>
#include <include/time.hpp>
#include <include/input_handler.hpp>

//https://lazyfoo.net/tutorials/SDL/36_multiple_windows/index.php

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        ts::Log::warning("SDL initialization failed");
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    else
        ts::Log::print("SDL initialized");

    auto window = ts::Window();
    window.create("window", 800, 600, ts::BORDERLESS);

    bool current = false;

    auto clock = ts::Clock();
    auto target_frame_duration = ts::seconds(1 / 60.f);

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

