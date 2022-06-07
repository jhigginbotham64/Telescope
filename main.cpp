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
#include <include/render_texture.hpp>
#include <include/camera.hpp>
#include <glm/glm.hpp>
#include <include/c_interface.hpp>

#include <glm/gtx/transform.hpp>

#include <include/physics.hpp>

using namespace ts;

int main()
{
    initialize();

    auto window = ts::Window();
    window.create("", 0, 0, ts::DEFAULT);

    auto render_texture = ts::RenderTexture(&window);
    render_texture.create(800, 600);

    auto clock = ts::Clock();
    auto target_frame_duration = ts::seconds(1 / 60.f);
    auto tri = ts::TriangleShape(Vector2f(400, 150), Vector2f(200, 450), Vector2f(600, 450));
    auto rect = ts::RectangleShape(50, 50, 400, 200);
    auto circ = ts::CircleShape({400, 300}, 100, 64);

    SDL_Rect port;
    port.x = -400;//-800;
    port.y = -300;//-600;
    port.h = 800;
    port.w = 600;

    SDL_RenderSetViewport(window.get_renderer(), &port);
    auto center = Vector2f(800, 600) / Vector2f(2, 2);

    std::vector<Vector2f> poly_v =
    {
        center + Vector2f(-100, -100),
        center + Vector2f(-50, -150),
        center + Vector2f(100, -100),
        center + Vector2f(70, 200),
        center + Vector2f(150, 130),
        center + Vector2f(-230, 75)
    };
    auto poly = ts::PolygonShape(poly_v);
    render_texture.set_color(RGBA(1, 0, 0, 1));

    assert(render_texture.get_native() != nullptr);
    render_texture.render(&poly);

    rect.set_texture(&render_texture);

    auto transform = ts::Transform();
    auto centroid = poly.get_centroid();

    auto camera = Camera(&window);

    while (window.is_open())
    {
        ts::start_frame(&window);
        window.clear();

        if (InputHandler::was_pressed(KeyboardKey::SPACE))
            std::cout << "space" << std::endl;

        if (InputHandler::is_down(KeyboardKey::RIGHT))
            camera.move(+10, 0);

        if (InputHandler::is_down(KeyboardKey::LEFT))
            camera.move(-10, 0);

        if (InputHandler::is_down(KeyboardKey::UP))
            camera.move(0, -10);

        if (InputHandler::is_down(KeyboardKey::DOWN))
            camera.move(0, +10);

        if (InputHandler::is_down(V))
            camera.rotate(ts::degrees(-5));

        if (InputHandler::was_pressed(B))
            camera.rotate(ts::degrees(+5));

        if (InputHandler::was_pressed(SPACE))
        {
            camera.center_on(poly.get_centroid());
        }

        window.render(&poly, transform);

        ts::end_frame(&window);
    }
}
