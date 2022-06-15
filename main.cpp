// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
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
    auto window_id = ts_window_create("test", 500, 500, ts::DEFAULT);
    auto world_id = ts_physics_world_create();

    while (ts_window_is_open(window_id))
    {
        float time = ts_start_frame(window_id);

        if (ts_keyboard_was_pressed(ts::SPACE))
    }
}

/*

float rng()
{
    return rand() / float(RAND_MAX);
}

struct Poly
{
    PolygonShape _shape;
    CollisionPolygon _hitbox;

    static std::vector<Vector2f> create_polygon(Vector2f center, float radius)
    {
        std::vector<Vector2f> out;

        for (size_t i = 0; i < 8; ++i)
            out.push_back(Vector2f(
                center.x + cos(ts::degrees(i / 8.f * 360).as_radians() * radius),
                center.y + sin(ts::degrees(i / 8.f * 360).as_radians() * radius)
        ));

        return out;
    }

    Poly(PhysicsWorld* world, Vector2f center, float radius)
        : _shape(create_polygon(center, radius)), _hitbox(world, ts::DYNAMIC, create_polygon(center, radius))
    {
        _shape.set_color(HSVA(rng(), rng(), 1, 1));
    }

    void update()
    {
        auto pos = _hitbox.get_native_body()->GetPosition();
        auto rotation = _hitbox.get_native_body()->GetAngle();
        _shape.set_centroid(Vector2f(pos.x, pos.y));
        _shape.rotate(ts::radians(-rotation));

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

    auto collision_handler = CollisionHandler(&world);

    auto camera = Camera(&window);

    auto down_shape = RectangleShape(0, h - ll, w, ll);
    down_shape.set_color(RGBA(1, 1, 1, 1));
    auto down_hb = CollisionPolygon(&world, ts::STATIC, down_shape);

    auto left_shape = RectangleShape(0, 0, ll, h);
    left_shape.set_color(RGBA(1, 1, 1, 1));
    auto left_hb = CollisionPolygon(&world, ts::STATIC, left_shape);

    auto right_shape = RectangleShape(w - ll, 0, ll, h);
    right_shape.set_color(RGBA(1, 1, 1, 1));
    auto right_hb = CollisionPolygon(&world, ts::STATIC, right_shape);

    auto top_shape = RectangleShape(0, 0, w, ll);
    top_shape.set_color(RGBA(1, 1, 1, 1));
    auto top_hb = CollisionPolygon(&world, ts::STATIC, top_shape);

    std::vector<CollisionCircleShape> circles;
    std::vector<CollisionTriangleShape> triangles;
    std::vector<CollisionRectangleShape> rectangles;
    std::vector<CollisionLineShape> lines;
    //std::vector<CollisionWireframeShape> wireframes;

    auto spawn = [&](){

        auto center = Vector2f{rng() * 800, 50 + rng() * 550};
        auto radius = std::max<float>(10, rng() * 40);
        auto color = HSVA(rng(), rng(), 1, 1);

        auto val = rng() * 4;

        if (val > 0 and val < 1)
        {
            circles.emplace_back(&world, ts::DYNAMIC, center, radius);
            circles.back().set_color(color);
            circles.back().set_density(0.1);
        }
        else if (val > 1 and val < 2)
        {
            auto angle_to_point = [&](float degrees) -> Vector2f
            {
                return Vector2f(
                        center.x + cos(ts::degrees(degrees).as_radians()) * radius,
                        center.y + sin(ts::degrees(degrees).as_radians()) * radius);
            };
            triangles.emplace_back(&world, ts::DYNAMIC, angle_to_point(0), angle_to_point(1 / 3.f * 360), angle_to_point(2 / 3.f * 360));
            triangles.back().set_color(color);
            triangles.back().set_density(0.1);
        }
        else if (val > 2 and val < 3)
        {
            rectangles.emplace_back(&world, ts::DYNAMIC, center - Vector2f(radius, radius), Vector2f(2 * radius, 2 * radius));
            rectangles.back().set_color(color);
            rectangles.back().set_density(0.1);
        }
        else if (val > 3 and val < 4)
        {
            lines.emplace_back(&world, ts::DYNAMIC, center - Vector2f(radius, 0), center + Vector2f(radius, 0));
            lines.back().set_color(color);
            lines.back().set_density(1);
        }
    };

    for (size_t i = 0; i < 5; ++i)
        spawn();

    auto player = ts::CollisionCircleShape(&world, ts::DYNAMIC, Vector2f(400, 300), 25);
    player.set_density(1);

    while (window.is_open())
    {
        auto time = ts::start_frame(&window);
        window.clear();

        world.step(ts::seconds(time.as_seconds() * 2));

        window.render(&left_shape);
        window.render(&right_shape);
        window.render(&down_shape);
        window.render(&top_shape);

        window.render(&player);

        bool pressed = false;

        const float v = 100;
        Vector2f velocity = {0, 0};
        if (InputHandler::is_down(KeyboardKey::RIGHT))
        {
            velocity.x += v;
        }

        if (InputHandler::is_down(KeyboardKey::LEFT))
        {
            velocity.x -= v;
        }

        if (InputHandler::is_down(KeyboardKey::UP))
        {
            velocity.y -= v;
        }

        if (InputHandler::is_down(KeyboardKey::DOWN))
        {
            velocity.y += v;
        }

        player.set_linear_velocity(velocity);

        for (auto& line : lines)
            line.ts::CollisionLine::set_angular_velocity(rng());

        if (InputHandler::was_pressed(SPACE))
        {
        }

        for (auto& ball : circles)
        {
            ball.update();
            window.render(&ball);
        }

        for (auto& tri : triangles)
        {
            tri.update();
            window.render(&tri);
        }

        for (auto& rect : rectangles)
        {
            rect.update();
            window.render(&rect);
        }

        for (auto& line : lines)
        {
            line.update();
            window.render(&line);
        }

        player.update();
        ts::end_frame(&window);
    }
}

 */
