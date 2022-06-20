//
// Created on 20.06.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <telescope.hpp>

#include <iostream>
#include <random>

using namespace ts;

// returns pseudo-random number in [0, 1]
float rng()
{
    static auto distribution = std::uniform_real_distribution<float>(0, 1);
    static auto device = std::random_device();
    static auto engine = std::mt19937();
    engine.seed(device());

    return distribution(engine);
}

// generate vertices of a circular polygon, 5 will generate a pentagon, 6 a hexagon, etc.
std::vector<Vector2f> generate_polygon_vertices(Vector2f center, float radius, size_t n_vertices)
{
    auto vertices_out = std::vector<Vector2f>();
    for (size_t i = 0; i < n_vertices; ++i)
    {
        Vector2f to_push;
        auto angle = ts::degrees(i * (360.f / n_vertices));
        to_push.x = center.x + cos(angle.as_radians()) * radius;
        to_push.y = center.y + sin(angle.as_radians()) * radius;

        vertices_out.push_back(to_push);
    }
    return vertices_out;
}

int main()
{
    // initialize all subsystems
    if (not initialize())
    {
        std::cerr << "[FATAL] Initialization failed." << std::endl;
        exit(1);
    }

    // create the window
    auto window = Window();
    const auto window_size = Vector2ui(800, 600);
    window.create("Hello Telescope", window_size.x, window_size.y);

    // create the physics world
    auto world = PhysicsWorld();

    // level geometry:
    std::vector<CollisionLineSequenceShape> level_geometry;

    const auto frame = 50;
    const auto outer_radius = (std::min(window_size.x, window_size.y) / 2.f) - frame;
    const auto screen_center = Vector2f(window_size.x / 2.f, window_size.y / 2.f);

    // movable lines
    std::vector<CollisionLineShape> lines;

    // horizontal line, -x: left, +y : right
    lines.emplace_back(
        &world,         // world
        ts::KINEMATIC,  // kinematic: can be moved and rotated but does not repond forces
        Vector2f(screen_center.x - outer_radius, screen_center.y), // left vertex
        Vector2f(screen_center.x + outer_radius, screen_center.y)  // right vertex
    );

    // vertical line, -y: up, +y :down
    lines.emplace_back(
        &world, ts::KINEMATIC,
        Vector2f(screen_center.x, screen_center.y - outer_radius), // upper vertex
        Vector2f(screen_center.x, screen_center.y + outer_radius)  // lower vertex
    );

    // fully dynamic entities
    std::vector<PolygonShape> polygons;

    // function to randomly spawn an entity inside the level arena
    auto spawn = [&](){

        // decide number of vertices
        size_t n_vertices = std::max<size_t>(3, round(rng() * 6));

        // decide the shapes radius
        float radius = std::max<float>(rng(), 0.5) * 20;

        // decide the shapes position
        const static auto center_of_screen = Vector2f(
            window_size.x / 2,
            window_size.y / 2
        );

        Vector2f center = center_of_screen;
        float distance_from_center = rng() * (window_size.x / 2) * 0.75;
        auto angle = ts::degrees(rng() * 360);

        center.x += cos(angle.as_radians()) * distance_from_center;
        center.y += sin(angle.as_radians()) * distance_from_center;

        // decide the shapes color
        auto color = HSVA(
            rng(),  // hue
            rng(),  // saturation
            1,      // value
            0.9     // transparency
        );

        std::cout << n_vertices << std::endl;

        // create n-vertex polygon
        auto vertices = generate_polygon_vertices(center, radius, n_vertices);
        polygons.emplace_back(vertices);
        polygons.back().set_color(color);
    };

    // start out with a few entities already in the wheel
    const size_t n_entities = 100;
    for (size_t i = 0; i < n_entities; ++i)
        spawn();

    // play music
    auto music = Music();
    system("echo $(pwd)");
    music.load("./test/otherworldly_foe.mp3");
    MusicHandler::play(
        music,          // music
        true,           // should music loop
        seconds(0.25)   // fade in duration
    );

    // print controls
    std::cout << "Controls: \n" \
              << "\t" << "LEFT ARROW: rotate wheel left" << "\n" \
              << "\t" << "RIGHT ARROW: rotate wheel right" << "\n" \
              << "\t" << "SPACE: spawn a new object" << "\n" \
              << "\t" << "ESCAPE: press twice to quit" << std::endl;
    bool escape_pressed = false;

    // TODO
    auto player = CollisionCircleShape(&world, ts::DYNAMIC, Vector2f(100, 100), 20);
    auto update_player = [&](){

        auto velocity = Vector2f(0, 0);
        auto delta = 150;
        if (InputHandler::is_down(KeyboardKey::UP))
            velocity.y -= delta;

        if (InputHandler::is_down(KeyboardKey::LEFT))
            velocity.x -= delta;

        if (InputHandler::is_down(KeyboardKey::DOWN))
            velocity.y += delta;

        if (InputHandler::is_down(KeyboardKey::RIGHT))
            velocity.x += delta;

        player.set_linear_velocity(velocity);
        player.set_color(RGBA(1, 1, 1, 1));
        player.update();
    };

    // render loop
    while (window.is_open())
    {
        // update input handler
        auto time = start_frame(&window);

        // clear window with black
        window.clear();

        // handle player input:
        float rotation = 0;

        // LEFT: rotate left
        if (InputHandler::is_down(KeyboardKey::A))
            rotation -= 1;

        // RIGHT: rotate right
        if (InputHandler::is_down(KeyboardKey::D))
            rotation += 1;

        // SPACE: spawn new entity
        if (InputHandler::was_pressed(KeyboardKey::SPACE))
            spawn();

        // ESCAPE: quit application
        if (InputHandler::was_pressed(KeyboardKey::ESCAPE))
        {
            if (not escape_pressed)
            {
                escape_pressed = true;
                std::cout << "Press the escape key again to quit.x" << std::endl;
            }
            else
            {
                std::cout << "Quitting..." << std::endl;
                window.close();
            }
        }

        // TODO
        update_player();
        window.render(&player);

        // step the physics simulation, synced to frame duration
        world.step(time);

        // render level geometry, does not need update because it is static
        for (auto& shape : level_geometry)
            window.render(&shape);

        // rotate the lines, update them, then render
        for (auto& line : lines)
        {
            line.set_angular_velocity(rotation); // depends on player input
            line.update();
            window.render(&line);
        }

        // update & render all dynamic entities
        // the physics simulation will move these, we only need to update their shape
        for (auto& polygon : polygons)
        {
            //polygon.update();
            window.render(&polygon);
        }

        // push the render state and wait for vsync
        end_frame(&window);
    }

    return 0; // everything is shut down safely automatically
}

