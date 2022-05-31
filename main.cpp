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
#include <include/polygon_shape.hpp>
#include <include/common.hpp>

#include <glm/glm.hpp>

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

    auto center = Vector2f(800, 600) / Vector2f(2, 2);

    std::vector<Vector2f> poly_v = {
        center + Vector2f(-100, -100),
        center + Vector2f(-50, -150),
        center + Vector2f(100, -100),
        center + Vector2f(70, 200),
        center + Vector2f(150, 130),
        center + Vector2f(-230, 75)
    };

    auto poly = ts::PolygonShape(poly_v);

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
        ts::start_frame(&window);

        poly.render(&window);

        for (auto& v : poly._vertices)
        {
            SDL_SetRenderDrawColor(window.get_renderer(), 255, 0, 0, 255);
            SDL_RenderDrawPoint(window.get_renderer(), v.position.x, v.position.y);
        }

        ts::end_frame(&window);
    }
}
