Hello Telescope
===============

Writing a "hello world" main, basic architecture of a real-time rendering application.

------------------------------

A Basic Render Loop
^^^^^^^^^^^^^^^^^^^

It's always good to start out with something that is already working. The following is a :code:`main.cpp` we can use
and extend as we'll learn more about telescope. Feel free to copy-paste this main as-is.

.. code-block:: cpp
    :caption: A basic main.cpp
    :linenos:

    #include <telescope.hpp>

    using namespace ts;

    int main()
    {
        // initialize everything
        if (not ts::initialize())
        {
            std::cerr << "FATAL: initialization failed. Exiting..." << std::endl;
            exit(1);
        }

        // create a window
        const size_t window_resolution_width = 800;
        const size_t window_resolution_height = 600;
        uint32_t window_options = ts::DEFAULT;
        const size_t frames_per_second = 60;

        const bool is_fullscreen = false;
        if (is_fullscreen)
            window_options = window_options | FULLSCREEN | BORDERLESS;

        ts::set_framerate_limit(frames_per_second);

        auto window = ts::Window();
        window.create(
            "Hello Telescope",          // window title
            window_resolution_width,    // x-dimension
            window_resolution_height,   // y-dimension
            window_options              // options
        );

        // create renderable entity: a circle
        auto shape = CircleShape(Vector2f(400, 300), 200, 32);
        for (size_t i = 0; i < shape.get_n_vertices(); ++i)
            shape.set_vertex_color(i, HSVA(float(i) / shape.get_n_vertices(), 1, 1, 1));

        // render loop
        while (window.is_open())
        {
            auto time = ts::start_frame(&window);
            window.clear();

            if (ts::InputHandler::was_pressed(ts::ESCAPE))
                window.close();

            // do rendering, physics, input-handling, etc. here
            window.render(&shape);

            ts::end_frame(&window);
        }

        // shutdown here
        return 0;
    }

A lot of things happen in this main, don't worry about understanding every single step just yet. The important thing is
that it gives use a framework, such that we won't have to start from scratch.

With :code:`fullscreen = false`, this main will open an empty window running at 60 fps. We can close the window just like
we would in any other application: by clicking the x button.

For borderless windows (including native fullscreen), we need some other way to request a shutdown, since there is no "x"
to click on. This is done in line #45, where we check if the user has pressed the escape key, closing the window if
they did.

------------------------------------

Enabling Logging
^^^^^^^^^^^^^^^^

Running the above main (and closing the window), we notice some console output:

.. code-block:: text
    :caption: stdout Output of "Hello World" main

    [LOG] initialization successful
    [LOG] System requested shutdown. Quitting...

Telescopes log system will keep us informed about important things such as windows closing, controllers being
connected/disconnected or warnings being thrown. Once we're ready to ship, we can turn off all logging using:

.. doxygenfunction:: ts::set_all_logging_disabled

Logging is enabled by default. It's recommended to leave it on at all times. Warnings may provide crucial
context if something goes wrong to both the developer and their users.

------------------------------------

Initialization
^^^^^^^^^^^^^^

Before any interaction with telescope can take place, we need to call:

.. doxygenfunction:: ts::initialize

This function returns a bool which should not be ignored. If one or more of telescopes subsystems fail the user will
be notified which and why, which is of great help when trying to diagnose potential problems.

------------------------------------

start_frame / end_frame
^^^^^^^^^^^^^^^^^^^^^^^

All graphical applications have what is often called a "render loop" or "render cycle". This is a continuous loop
that exits when the window which provides the rendering context closes. Each loop is called a `Frame`.

In telescope, the very first thing that should happen each frame is to call:

.. doxygenfunction:: ts::start_frame(Window *window)

Calling this function every frame is crucial because it polls the user input state, updates the window and sets up
important things like the internal timer. Nothing else should interact with telescope while this function is running,
though this only becomes important in multi-threaded environments.

Next, we usually want to call :code:`ts::Window::clear`. This clears the window (such that it
now displays all black). Afterwards we can queue our own render commands. We will learn more about this in the
section on :code:`ts::Window`.

At the end of each frame, the very last thing we need to call is

.. doxygenfunction:: ts::end_frame(Window *window)

This synchronizes the windows render state with that of the operating systems, which makes it so the graphics actually
show up on the monitor.

Additionally, the function performs a non-busy wait. The amount of time waited depends on the target fps (set
via :code:`ts::set_target_fps`) and the systems vsync. If you do not want to wait at all, simply set the target fps to a
really high number (though this is not recommended).

------------------------------------

In Summary
^^^^^^^^^^

We learned that a typical telescope applications :code:`main.cpp` has the following form:

.. code-block:: cpp

    int main()
    {
        // initialize telescope
        if (not ts::initialize())
            // exit

        // create a window
        auto window = ts::Window()
        window.create(// ...

        // initialize user-defined things here

        // render loop
        while (window.is_open())
        {
            ts::start_frame(&window);
            window.clear();

            // user render and simulation for this frame here

            ts::end_frame(&window);
        }

        return 0;
    }

We will learn more about :code:`ts::Window` in particular soon, which provides us with
a canvas we can use it to render our own shapes and images.
