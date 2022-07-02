Player Input
============

Accessing the state of the keyboard, mouse, and controller

----------------------

On of the most fundamental property of games is that they are interactive. To allow for this interactivity,
any game engine will need a convenient yet powerful way access and respond to player input.

In telescope, we cannot manually handle input events. Instead, they are processed automatically
(during :code:`ts::start_frame`) by updating the global singleton :code:`ts::InputHandler`. Once updated,
we can safely access the current state of the players input devices from anywhere we want.

Fundamentally, there are two types of inputs: the **state of a key** and the **state of an axis**.
A key state is binary, the key is either up (not pressed) or down (pressed) at any given time. The state
of an axis is described with a floating pointer number, usually in [0, 1]. For example, the triggers of
most controller are analog, we can press them slightly, more, or not at all. Depending on the position of the trigger,
the controllers axis' state of that trigger will contain a float: 0 if not pressed at all, 0.5 if half pressed,
1 if pressed fully, etc.

----------------------


Key or Button Input
^^^^^^^^^^^^^^^^^^^

There are three types of keys or buttons, all of which have a binary state:

Keyboard Keys
*************

Keyboard keys are identified as a value of the enum :code:`ts::KeyboardKey`.

These keys include the space bar, any letter on a keyboard, the enter key, volume up/down, control, etc.

The following are all keyboard keys recognized by telescope:

.. doxygenenum:: ts::KeyboardKey

Note that, while regional variations of keyboard are recognized, their label may not necessarily respond to the key.

--------------------------------

Mouse Buttons
*************

Mouse buttons, including the omnipresent left and right mouse button, are values of the enum :code:`ts::MouseButton`:

.. doxygenenum:: ts::MouseButton

--------------------------------

Controller Buttons
******************

Correctly identifying controller buttons is tricky, because there are many manufacturers. The description of the values of
:code:`ts::ControllerButton` below should cover at least the XBOX, PlayStation and Nintendo Switch controller, as well
as any generic two-axis, 4 face-, 2 shoulder-button controllers.

.. doxygenenum:: ts::ControllerButton

--------------------------------

Querying the Key or Button State
********************************

Now that we have a way to identity keys or buttons, we can access their state through
:code:`ts::InputHandler`

The following member functions relate to keys or buttons, (note that overloads exist for :code:`ts::KeyboardKey`, :code:`ts::ControllerButton`, :code:`ts::MouseButton`)

.. doxygenfunction:: ts::InputHandler::is_down(KeyboardKey)
.. doxygenfunction:: ts::InputHandler::has_state_changed(KeyboardKey)
.. doxygenfunction:: ts::InputHandler::was_pressed(KeyboardKey)
.. doxygenfunction:: ts::InputHandler::was_released(KeyboardKey)

These functions are highly convenient in the context of game development, for example:

.. code-block:: cpp

    if (ts::InputHandler::is_down(ts::SPACE)
        // do action

If this line of code is called once per frame, the action will happen once per frame as long as the button is held down.

In contrast:

.. code-block:: cpp

    if (ts::InputHandler::was_pressed(ts::SPACE)
        // do action

Will trigger the action exactly once, the frame the button is pressed. Afterwards, the player has to first release,
then repress the button to trigger the action again. This makes for a frame-perfect and intuitive input method.

Controller IDs
**************

For controller buttons, each function takes an additional argument, the controllers internal ID:

.. doxygenfunction:: ts::InputHandler::is_down(ControllerButton controller_button, ControllerID id = 0)
.. doxygenfunction:: ts::InputHandler::has_state_changed(ControllerButton controller_button, ControllerID id = 0)
.. doxygenfunction:: ts::InputHandler::was_pressed(ControllerButton controller_button, ControllerID id = 0)
.. doxygenfunction:: ts::InputHandler::was_released(ControllerButton controller_button, ControllerID id = 0)

Usually, the first controller connected will be assigned ID 0, the second ID 1, etc.

.. note::
    In :code:`Telescope.jl`, the lowest controller IDs start at 1 instead, there is no controller with ID 0

--------------------------------

--------------------------------


Analog Input
^^^^^^^^^^^^^^

There are a lot less axis when compared to keys or buttons. Keyboards do not have an axis associated with it.

Mouse: Cursor Position
**********************

The cursor position on the screen is identified by a :code:`Vector2i` (note the signed elements). We access it using:

.. doxygenfunction:: ts::InputHandler::get_cursor_position


Cursor position (0, 0) is the top left of the window that is currently in focus. If the mouse is left of above that
coordinate, the cursor position may be negative in its x or y component respectively. Otherwise, 1 pixel on the monitor
responds to 1 increase in either mouse cursor position component.

Mouse: Scrollwheel
******************

Some mice allow for scrolling in two directions. Because of this, the result of

.. doxygenfunction:: ts::InputHandler::get_scrollwheel

is also a vector, where (0, 0) is no scroll-wheel activity.

--------------------------------

Controller: Joysticks
*********************

To access the state of one of the analog joystick positions, we use

.. doxygenfunction:: ts::InputHandler::get_controller_axis_left
.. doxygenfunction:: ts::InputHandler::get_controller_axis_right

Controller: Triggers
********************

Similarly, to access current positions of a controllers triggers, telescope provides:

.. doxygenfunction:: ts::InputHandler::get_controller_trigger_left
.. doxygenfunction:: ts::InputHandler::get_controller_trigger_right

All functions of :code:`ts::InputHandler` that deal with controller input will take
as their argument the controller ID. This makes it possible to handle the input of multiple
connected controllers at the same time.

--------------------------------------------

ts::InputHandler
^^^^^^^^^^^^^^^^

Following is the complete list of functions available through :code:`ts::InputHandler`. This class has a lot of methods,
hopefully this step-by-step description made this powerful way of querying the input state
more easy to understand.

.. doxygenclass:: ts::InputHandler
    :members:
