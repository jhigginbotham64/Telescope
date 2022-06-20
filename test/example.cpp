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
    window.create("Telescope Example", window_size.x, window_size.y);

    // create the physics world
    auto world = PhysicsWorld();
    world.set_gravity(Vector2f(0, 100)); // gravity pulls screen-down

    // level geometry:;
    std::vector<CollisionLineSequenceShape> boundaries;

    // outer boundaries of the screen
    boundaries.emplace_back(&world, ts::STATIC, std::vector<Vector2f>{
        Vector2f(0+1, 0),
        Vector2f(window_size.x, 0),
        Vector2f(window_size.x, window_size.y-1),
        Vector2f(0+1, window_size.y-1),
        Vector2f(0+1, 0)
    });

    const auto frame = 100;
    const auto outer_radius = (std::min(window_size.x, window_size.y) / 2.f) - frame;
    const auto screen_center = Vector2f(window_size.x / 2.f, window_size.y / 2.f);

    // horizontal line, -x: left, +y : right
    auto line = std::vector<CollisionLineShape>{CollisionLineShape(
        &world,         // world
        ts::KINEMATIC,  // kinematic: can be moved and rotated but does not repond forces
        Vector2f(0, screen_center.y), // left vertex
        Vector2f(window_size.x, screen_center.y)  // right vertex
    )};

    auto spike_vertices = {
        Vector2f(screen_center + Vector2f(-frame, 0)),
        Vector2f(screen_center + Vector2f(0, -2 * frame)),
        Vector2f(screen_center + Vector2f(+frame, 0)),
        Vector2f(screen_center + Vector2f(0, +2 * frame)),
        Vector2f(screen_center + Vector2f(-frame, 0))   // duplicate first to close the loop
    };
    auto spike = std::vector<CollisionLineSequenceShape>{CollisionLineSequenceShape(
        &world,
        ts::KINEMATIC,
        spike_vertices
    )};

    // fully dynamic entities
    std::vector<CollisionPolygonShape> polygons;

    // function to randomly spawn an entity inside the level arena
    auto spawn = [&](){

        // decide number of vertices
        size_t n_vertices = std::max<size_t>(3, round(rng() * 6));

        // decide the shapes radius
        float radius = std::max<float>(rng(), 0.5) * 20;

        // decide the shapes initial position
        auto center = Vector2f(0, 0);
        if (rng() > 0.5)
            center.y = 0 + rng() * frame;
        else
            center.y = window_size.y - frame + rng() * frame;

        center.x = rng() * window_size.x;

        // decide the shapes color
        auto color = HSVA(
            rng(),  // hue
            std::max<float>(rng(), 0.25),  // saturation
            1,      // value
            1     // transparency
        );

        // create n-vertex polygon
        auto vertices = generate_polygon_vertices(center, radius, n_vertices);
        polygons.emplace_back(&world, ts::DYNAMIC, vertices);
        polygons.back().set_color(color);
        polygons.back().set_density(10);
    };

    // start out with a few entities already in the wheel
    const size_t n_entities = 200;
    for (size_t i = 0; i < n_entities; ++i)
        spawn();

    // player character: a perfect circle
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
    auto player_texture = StaticTexture(&window);
    player_texture.load("./docs/_static/favicon.png");
    player.set_texture(&player_texture);

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
              << "\t" << "ARROWS: move player sprite" << "\n" \
              << "\t" << "A: rotate wheel left" << "\n" \
              << "\t" << "D: rotate wheel right" << "\n" \
              << "\t" << "ESCAPE: press twice to quit" << std::endl;

    std::cout << "Goal: \n" \
              << "\t" << "Try to get all polygons below the line. If you succeed, something cool may happen!" \
              << std::endl;

    bool escape_pressed = false;

    // win conditions: all polygons are on one side of the line
    auto sound = Sound();
    sound.load("./test/ok_desu_ka.mp3");
    std::vector<CollisionCircleShape> win_condition_snow;

    auto check_win_condition = [&]()
    {
        static bool active = false;

        if (not active) // check for in condition
        {
            float screen_cutoff = window_size.y * 0.7; // bottom 25% of the screen
            auto line_aabb = line.front().ts::Shape::get_bounding_box();
            float line_cutoff = line_aabb.top_left.y  + line_aabb.size.y; // below line

            for (auto &polygon: polygons)
            {
                auto y = polygon.ts::CollisionShape::get_centroid().y;
                if (y < screen_cutoff or y < line_cutoff)
                    return;
            }

            // success:
            MusicHandler::stop(seconds(0.1));
            SoundHandler::play(
                    SoundHandler::next_free_channel(), // sound channel id
                    sound,  // sound
                    0       // number of loops
            );

            for (auto& polygon : polygons)
                polygon.destroy();
            polygons.clear();

            line.front().destroy();
            line.clear();

            spike.front().destroy();
            spike.clear();

            active = true;
        }

        if (win_condition_snow.size() > 200)
            return;

        win_condition_snow.emplace_back(
                &world,
                ts::DYNAMIC,
                Vector2f(rng() * window_size.x, 25),
                2);

        win_condition_snow.back().set_restitution(1);
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

        // ESCAPE: quit application
        if (InputHandler::was_pressed(KeyboardKey::ESCAPE))
        {
            if (not escape_pressed)
            {
                escape_pressed = true;
                std::cout << "Press the escape key again to give up" << std::endl;
            }
            else
            {
                std::cout << "Quitting..." << std::endl;
                window.close();
            }
        }

        // step the physics simulation, synced to frame duration
        world.step(time);

        // update and render the player sprite
        update_player();
        window.render(&player);

        // render level geometry, does not need update because it is static
        for (auto& boundary : boundaries)
            window.render(&boundary);

        // rotate the line and spike, update them, then render
        for (auto& l : line)
        {
            l.set_angular_velocity(rotation); // depends on player input
            l.update();
            window.render(&l);
        }

        for (auto& s : spike)
        {
            s.set_angular_velocity(rotation);
            s.update();
            window.render(&s);
        }

        // update & render all dynamic entities
        for (auto& polygon : polygons)
        {
            // sync position with that of the hitbox
            polygon.update();

            // cycle through colors for flair
            auto color = polygon.get_vertex_color(0).as_hsv();
            auto new_color = HSVA(std::fmod(time.as_seconds() / 2.f, 1), 1, 1, 0.9);
            polygon.set_color(HSVA(fmod(color.hue + new_color.hue, 1), color.saturation, 1, color.value));

            // render
            window.render(&polygon);
        }

        // test whether the player has won
        check_win_condition();

        for (auto& snow : win_condition_snow)
        {
            snow.update();
            window.render(&snow);
        }

        // push the render state and wait for vsync
        end_frame(&window);
    }
    return 0; // everything is shut down safely automatically
}

