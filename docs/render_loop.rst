Hello Telescope
===============

Implementing a "hello world" main, useful as a starting point for any new telescope project.

A Basic Render Loop
^^^^^^^^^^^^^^^^^^^

To start us out with, let's outline what a `main.cpp` using telescope would look like. Feel free to copy this main
as-is and extend it as you are working through this documentation. It's always nice to
start out with something already working, right?

.. code-block:: cpp
    :caption: A basic main.cpp

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

        // render loop
        while (window.is_open())
        {
            auto time = ts::start_frame(&window);
            window.clear();

            // do rendering, physics, input-handling, etc. here

            ts::end_frame(&window);
        }

        // shutdown here
        return 0;
    }

A lot of things happened in this main, don't worry about understanding every single step just yet. What's important is,
that it gives use a framework we can use to later fill-in our own things.

With :code:`fullscreen = false`, this main will open an empty window running at 60 fps. We can close the window just like
we would any other application. For fullscreen windows, the OS has to signal closing some other way, since there is no "x"
to click on.

------------------------------------

Enabling Logging
^^^^^^^^^^^^^^^^

When executing this main (and closing the window), you'll notice some console output:

.. code-block:: text
    :caption: stdout Output of "Hello World" main

    [LOG] initialization successful
    [LOG] System requested shutdown. Quitting...

Telescopes log system will keep you informed about important things such as windows closing, controllers being
connected/disconnected or warnings being thrown. Once you're ready to ship, you can turn off all logging using:

.. doxgenfunction:: ts::set_all_logging_disabled

Logging is enabled by default. I would recommend leaving it on at all times, as warnings may provide crucial
context if something goes wrong.

------------------------------------

Initialization
^^^^^^^^^^^^^^

Before any interaction with telescope can take play, we need to call:

.. doxygenfunction:: ts::initialize

This function returns a bool which you should not ignore. If one or more of telescopes subsystems fail, it will tell you
which, helping you solve any potential problems.

------------------------------------

start_frame / end_frame
^^^^^^^^^^^^^^^^^^^^^^^

All graphical applications have what is called a "render loop". This is a continous loop that exits when the window in
question closes. Each loop is called a `Frame`. In telescope, the very first thing that should happen each frame is to
call:

.. doxygenfunction:: ts::start_frame(Window *window)

Calling this function every frame is crucial because it polls the user input state, updates the window and sets up important things such
as the internal timer. No other things should interact with telescope while this function is running, which becomes
important in multi-threaded environments.

Next, we usually want to call :code:`ts::Window::clear`. This clears the window (such that it
now displays all black), so that we can queue our own render commands after this. We will learn more about this in the
section on :code:`ts::Shape`.

After we did everything else we wanted to do this frame, the very last thing we need to call is

.. doxygenfunction:: ts::end_frame(Window *window)

This synchronizes the windows state with that of the operating systems, such that it actually shows up on the monitor.
Afterwards, it simply waits until the new frame should begin. The amount of time waited depends on the target fps (set
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

In the next section, we will learn more about :code:`ts::Window` in particular, so we can use it to render our own
shapes and images.
