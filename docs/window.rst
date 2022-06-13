Windows
=======

Creating to a window, rendering an entity.

-----------------------

In this section, we'll go into detail on how to create and use a window, represented in telescope as :code:`ts::Window`.

Before we start, note that multiple windows are possible. Please consult the internal documentation for more information.
For now, we'll focus on having just one window.

Creating a Window
^^^^^^^^^^^^^^^^^

When creating a window, we usually first call the windows default constructor

.. doxygenfunction:: ts::Window::Window()

Then, use

.. doxygenfunction:: ts::Window::create(std::string title, size_t width, size_t height, uint32_t options = DEFAULT)

We see that this function takes a title for the window, its size and the windows options. Window options are values
of the enum :code:`ts::WindowOptions` bitwise-or'd together:

.. doxygenenum:: ts::WindowOptions

For example, if we want to have a borderless window that is resizable but no full screen, we would use
:code:`ts::DEFAULT | ts::BORDERLESS | ts::RESIZABLE` as the windows constructors last argument.

---------------------------

Rendering to a Window
^^^^^^^^^^^^^^^^^^^^^

The most common task we want to use the window for is to display graphics. To do this, we use the following members function:

.. doxygenfunction:: ts::Window::render

We see that it takes, as its argument, a pointer to a :code:`ts::Renderable`. This is a pure virtual (abstract) class:

.. doxygenclass:: ts::Renderable

Only classes that publicly inherit from this class can be rendered.

The second argument to :code:`window.render` is a transform.

.. doxygenstruct:: ts::Transform

We will learn more about transforms in the section on cameras.

For now, let's create a simple rectangle and render it to the screen:

.. code-block:: cpp

    auto shape = ts::RectangleShape(Vector2f(200, 200), Vector2f(400, 200);

    // render loop
    while (window.is_open())
    {
        ts::start_frame(&window);

        // clear the window
        window.clear();

        // render rectangle
        window.render(&shape);

        // push the window state to the monitor
        window.flush();

        ts::end_frame(&window);
    }

To render any renderable, we use :code:`window.render(&object)`, where :code:`object` is the renderable. We use a
pointer because it will be implicitly cast to the abstract type, which :code:`window.render` requires.

Before any calls to :code:`ts::Window::render` should happen this frame, we should clear the window:

.. doxygenfunction:: ts::Window::clear

This sets all pixels of the window to black. If we were to end the frame here, the window would simply show up as black.

After we did all our render calls, we need to push the windows state to the monitor. While this is usually done during
:code:`ts::end_frame(&window)`, we can also do so manually using

.. doxygenfunction:: ts::Window::flush

-------------------------------------------

ts::Window
^^^^^^^^^^

Other than basic rendering, telescope gives a lot of control over the window. We can move it on the screen,
programmatically minimize or maximize it and change its size. A full list of functions available to :code:`ts::Window`
is provided here

.. doxygenclass:: ts::Window
    :members:

-------------------------------------------

Creating our Own Renderables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For something to be rendered by a :code:`ts::Window`, all the object needs to do is to inherit publicy from
:code:`ts::Renderable`, then implement the following function

.. doxygenfunction:: ts::Renderable::render

This function takes as its first argument a :code:`ts::RenderTarget`. Using

.. doxygenfunction:: ts::RenderTarget::get_renderer

We can expose the native SDL rendering context, which we can modify as we like. For more information on how to interact
with :code:`SDL_Renderer`, see `here <https://wiki.libsdl.org/SDL_Renderer>`_

