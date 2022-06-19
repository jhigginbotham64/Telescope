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

float rng()
{
    return rand() / float(RAND_MAX);
}

std::vector<Vector2f> generate_polygon(Vector2f center, float radius, size_t n = 8)
{
    std::vector<Vector2f> out;
    for (size_t i = 0; i < n; ++i)
    {
        out.push_back(Vector2f(
            center.x + cos(ts::degrees(i / 8.f * 360).as_radians()) * radius * std::max<float>(0.25, rng()),
            center.y + sin(ts::degrees(i / 8.f * 360).as_radians()) * radius * std::max<float>(0.25, rng())
        ));
    }

    return out;
}

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
    std::vector<CollisionPolygonShape> polys;
    std::vector<CollisionLineSequenceShape> line_seqs;

    auto spawn = [&](){

        auto center = Vector2f{rng() * 800, 50 + rng() * 550};
        auto radius = std::max<float>(10, rng() * 40);
        auto color = HSVA(rng(), rng(), 1, 1);

        auto val = rng() * 6;

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
            /*
            lines.emplace_back(&world, ts::DYNAMIC, center - Vector2f(4 * radius, 0), center + Vector2f(4 * radius, 0));
            lines.back().set_color(color);
            lines.back().set_density(1);
             */
        }
        else if (val > 4 and val < 5)
        {
            auto vertices = generate_polygon(center, radius);
            vertices.push_back(vertices.front());
            line_seqs.emplace_back(&world, ts::DYNAMIC, vertices, false);

            for (auto& e : line_seqs.back().get_shapes())
                e.set_color(color);
        }
        else if (val > 5)
        {
            /*
            auto vertices = generate_polygon(center, radius);
            polys.emplace_back(&world, ts::DYNAMIC, vertices);
            polys.back().set_color(color);
            polys.back().set_density(1);
             */
        }
    };

    for (size_t i = 0; i < 50; ++i)
    {
        //spawn();
    }

    auto test = ts::CollisionLineSequenceShape(&world, ts::DYNAMIC, {
       Vector2f(250, 250),
       Vector2f(250 + 100, 250),
       Vector2f(250 + 100, 250 + 100),
       Vector2f(250, 250 + 100)
    }, true);

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

        //TODO
        window.render(&test);

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

        for (auto& poly : polys)
        {
            poly.update();
            window.render(&poly);
        }

        for (auto& seq : line_seqs)
        {
            seq.update();
            window.render(&seq);
        }

        player.update();
        ts::end_frame(&window);
    }
}
