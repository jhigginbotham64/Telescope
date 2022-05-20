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

class InputHandler
{
    public:
        bool is_key_down();
        bool has_state_changed();

};

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        ts::Log::warning("SDL initialization failed");
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    else
        ts::Log::print("SDL initialized");

    SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, 0);
    auto event = SDL_Event();

    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            auto sym = event.key.keysym.sym;

            if (sym == SDLK_ESCAPE)
                exit(0);

            std::cout << (char) event.key.keysym.sym << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds (1/60 * 1000));
    }
}

