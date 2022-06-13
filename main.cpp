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
#include "include/collision_rectangle_shape.hpp"

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
        _shape.set_color(HSVA(rng(), std::max<float>(0.5, rng()), 1, 1).as_rgb());
        _hitbox.set_restitution(0.2);
        update();
    }

    void update()
    {
        auto pos = _hitbox.get_native_body()->GetPosition();
        _shape.set_centroid(Vector2f(pos.x, pos.y));
    }
};

struct Tri
{
    TriangleShape _shape;
    CollisionPolygon _hitbox;

    static Triangle create_triangle(Vector2f center, float radius)
    {
        auto angle_to_point = [&](float degrees) -> Vector2f
        {
            return Vector2f(
                    center.x + cos(ts::degrees(degrees).as_radians()) * radius,
                    center.y + sin(ts::degrees(degrees).as_radians()) * radius);
        };

        return Triangle{angle_to_point(0), angle_to_point(1/3.f * 360), angle_to_point(2/3.f * 360)};
    }

    Tri(PhysicsWorld* world, Vector2f center, float radius)
        : _shape(create_triangle(center, radius)), _hitbox(world, ts::DYNAMIC, create_triangle(center, radius))
    {
        _shape.set_color(HSVA(rng(), std::max<float>(0.5, rng()), 1, 1).as_rgb());
        _hitbox.set_restitution(0.2);
        update();
    }

    void update()
    {
        auto pos = _hitbox.get_native_body()->GetPosition();
        auto rotation = _hitbox.get_native_body()->GetAngle();
        _shape.set_centroid(Vector2f(pos.x, pos.y));
        _shape.rotate(ts::radians(-rotation));
    }
};

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
    std::vector<Tri> tris;
    std::vector<CollisionRectangleShape> rects;
    std::vector<Poly> polys;

    auto spawn = [&](){

        auto center = Vector2f{rng() * 800, 0};
        auto radius = std::max<float>(10, rng() * 40);

        auto val = rng() * 4;

        if (val > 0 and val < 1)
            balls.emplace_back(&world, center, radius);
        else if (val > 1 and val < 2)
            tris.emplace_back(&world, center, radius);
        else if (val > 2 and val < 3)
        {
            rects.emplace_back(&world, ts::DYNAMIC, center - Vector2f(radius, radius), Vector2f(2 * radius, 2 * radius));
            rects.back().set_color(HSVA(rng(), rng(), 1, 1));
        }
        else if (val > 3 and val < 4)
        {
            polys.emplace_back(&world, center, radius);
        }
    };

    //for (size_t i = 0; i < 10; ++i)
        //spawn();

    auto shape = ts::CollisionCircleShape(&world, ts::DYNAMIC, Vector2f(400, 300), 100);
    shape.set_density(1);

    while (window.is_open())
    {
        auto time = ts::start_frame(&window);
        window.clear();

        world.step(ts::seconds(time.as_seconds() * 2));

        window.render(&left_shape);
        window.render(&right_shape);
        window.render(&down_shape);
        window.render(&shape);

        bool pressed = false;

        if (InputHandler::is_down(KeyboardKey::RIGHT))
        {
            pressed = true;
            shape.set_linear_velocity(Vector2f(+50, 0));
        }

        if (InputHandler::is_down(KeyboardKey::LEFT))
        {
            pressed = true;
            shape.set_linear_velocity(Vector2f(-50, 0));
        }

        if (InputHandler::is_down(KeyboardKey::UP))
        {
            pressed = true;
            shape.set_linear_velocity(Vector2f(0, -50));
        }

        if (InputHandler::is_down(KeyboardKey::DOWN))
        {
            pressed = true;
            shape.set_linear_velocity(Vector2f(0, +50));
        }

        if (not pressed)
            shape.set_linear_velocity(Vector2f(0, 0));

        //if (InputHandler::was_pressed(SPACE))
            //spawn();

        for (auto& ball : balls)
        {
            ball.update();
            window.render(&ball._shape);
        }

        for (auto& tri : tris)
        {
            tri.update();
            window.render(&tri._shape);
        }

        for (auto& rect : rects)
        {
            rect.update();
            window.render(&rect);
        }

        for (auto& poly : polys)
        {
            poly.update();
            window.render(&poly._shape);
        }

        shape.update();
        ts::end_frame(&window);
    }
}
