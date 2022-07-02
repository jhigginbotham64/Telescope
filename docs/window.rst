Windows
=======

Creating a window, rendering an entity.

-----------------------

In telescope, a operating system window and its accompanying render context is managed by an instance of :code:`ts::Window`.

It's possible to have multiple windows active at the same time. Things such as the mouse position will be relative to
the window that currently has focus. For more information, please consult the internal documentation. From this
point onward, we'll assume that an application only has a single window.

Creating a Window
^^^^^^^^^^^^^^^^^

First, we call :code:`ts::Window`s default constructor

.. doxygenfunction:: ts::Window::Window()

Then, to actually open a window such that it shows up on screen, we call:

.. doxygenfunction:: ts::Window::create(std::string title, size_t width, size_t height, uint32_t options = DEFAULT)

We see that this function takes, as its arguments, the title for the window, its size and the **windows options**.
Window options are values of the enum :code:`ts::WindowOptions` bitwise-or'd together:

.. doxygenenum:: ts::WindowOptions

For example, if we want to have a borderless window that is resizable but not full screen, we would use
:code:`ts::DEFAULT | ts::BORDERLESS | ts::RESIZABLE` as the windows constructors last argument.

---------------------------

Rendering to a Window
^^^^^^^^^^^^^^^^^^^^^

The most common task we want to use the window for is to display graphics. To render an object, we use:

.. doxygenfunction:: ts::Window::render

We see that the object is identified by pointer to a :code:`ts::Renderable`. This is a pure virtual (abstract) class.

.. doxygenclass:: ts::Renderable
    :members:

Only classes that publicly inherit from this class can be rendered.

The second argument to :code:`window.render` is a transform.

.. doxygenstruct:: ts::Transform

We will learn more about transforms in the section on cameras.

For now, let's create a simple rectangle and render it to the screen:

.. code-block:: cpp

    // create rectangle shape
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

To render any renderable, we use :code:`window.render(&object)`, where :code:`object` is the renderable. By using a
pointer, the argument will be implicitly cast to :code:`ts::Renderable`, which :code:`window.render` requires.

We should clear the window at the start of the frame, this essentially clears the window with the color black (:code:`RGBA(0, 0, 0, 1)`)
.. doxygenfunction:: ts::Window::clear

 If we were to end the frame directly after this, the actual window on the monitor would not be updated, however. To
force the OS to synchronize the monitor with the windows render state, we need to call

.. doxygenfunction:: ts::Window::flush

each frame. This is done automatically during :code:`ts::end_frame`, though it is sometimes necessary to manually
flush the window.

-------------------------------------------

ts::Window
^^^^^^^^^^

Other than basic rendering, telescope gives a lot of additional control over the window. We can move it around
the screen space, programmatically minimize or maximize it, change its size, etc.

A full list of functions available to :code:`ts::Window` is provided here

.. doxygenclass:: ts::Window
    :members:

-------------------------------------------

Creating our Own Renderables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For something to be able to be rendered by a :code:`ts::Window`, all the object needs to do is publicly inherit from
:code:`ts::Renderable`, then implement the following function

.. doxygenfunction:: ts::Renderable::render

This function takes as its first argument a :code:`ts::RenderTarget`. Using

.. doxygenfunction:: ts::RenderTarget::get_renderer

We can expose the native SDL rendering context, which we can modify as we like. For more information on how to interact
with :code:`SDL_Renderer`, see `here <https://wiki.libsdl.org/SDL_Renderer>`_.

