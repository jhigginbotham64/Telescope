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

float rng()
{
    return rand() / float(RAND_MAX);
}

struct Ball
{
    CircleShape _shape;
    CollisionCircle _hitbox;

    Ball(PhysicsWorld* world, Vector2f center, float radius)
        : _shape(center, radius, 16), _hitbox(world, DYNAMIC, center, radius)
    {
        _shape.set_color(HSVA(rng(), 1, 1, 1).as_rgb());
        _hitbox.set_restitution(10);
        update();
    }

    void update()
    {
        auto pos = _hitbox.get_native_body()->GetPosition();
        _shape.set_centroid(Vector2f(pos.x, pos.y));
    }
};

int main()
{
    initialize();

    size_t ll = 5;
    size_t h = 600;
    size_t w = 800;

    auto window = ts::Window();
    window.create("", 800, 600, ts::DEFAULT);

    auto world = PhysicsWorld();
    world.set_gravity({0, 100});

    auto camera = Camera(&window);

    auto down_shape = RectangleShape(0, h - ll, w, ll);
    down_shape.set_color(RGBA(1, 1, 1, 1));
    auto down_hb = CollisionPolygon(&world, ts::STATIC, down_shape);

    std::cout << down_hb.get_native_body()->GetPosition().x << " " << down_hb.get_native_body()->GetPosition().y  << std::endl;

    auto left_shape = RectangleShape(0, 0, ll, h);
    left_shape.set_color(RGBA(1, 1, 1, 1));
    auto left_hb = CollisionPolygon(&world, ts::STATIC, left_shape);

    auto right_shape = RectangleShape(w - ll, 0, ll, h);
    right_shape.set_color(RGBA(1, 1, 1, 1));
    auto right_hb = CollisionPolygon(&world, ts::STATIC, right_shape);

    std::vector<Ball> balls;
    auto spawn_ball = [&](){
        balls.emplace_back(&world, Vector2f{rng() * 800, 0}, std::max<float>(10, rng() * 50));
    };

    spawn_ball();

    while (window.is_open())
    {
        auto time = ts::start_frame(&window);
        window.clear();

        world.step(time);

        window.render(&left_shape);
        window.render(&right_shape);
        window.render(&down_shape);

        if (InputHandler::was_pressed(SPACE))
            spawn_ball();

        for (auto& ball : balls)
        {
            ball.update();
            window.render(&ball._shape);
        }

        ts::end_frame(&window);
    }
}
