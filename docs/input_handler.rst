Handling Player Input
=====================

Accessing the state of the keyboard, mouse and controllers.

----------------------

On of the most fundamental property of games is that they are interactive. To allow for interactivity,
any game engine will need a very convenient and powerful access user inputs.

In telescope, user cannot manually handle input events. Instead, they are all handled automatically
(during :code:`ts::start_frame`) by updating the global singleton :code:`ts::InputHandler`.

Fundamentally, there are two types of inputs: the **state of a key** and the **state of an axis**.
A key state is binary, the key is either up (not pressed) or down (pressed) at any given time. The state
of an axis is described with a floating pointer number, usually in [0, 1]. For example, the triggers of
most controller are analog, you can press them slightly, or harder. Depending on the position of the trigger,
the controllers axis state of that trigger will contain a float: 0 if not pressed at all, 0.5 if half pressed,
1 if pressed fully, etc.

----------------------


Key or Button Input
^^^^^^^^^^^^^^^^^^^

There are three types of keys or buttons. A key or buttons state is either "up" or "down":

Keyboard Keys
*************

Keyboard keys are identified as a value of the enum :code:`ts::KeyboardKey`.

These keys include the space bar, any letter on a keyboard, the enter key, volume up/down, etc.

The following are all keyboard keys recognized by telescope:

.. doxygenenum:: ts::KeyboardKey

--------------------------------

Mouse Buttons
*************

Mouse buttons, including the omnipresent left and right mouse button, are values of the enum :code:`ts::MouseButton`:

.. doxygenenum:: ts::MouseButton

--------------------------------

Controller Buttons
******************

Controller buttons are a bit tricky because there are so many manufactures. The description of the values of
:code:`ts::ControllerButton` below should cover the most common controller manufactures on the market:

.. doxygenenum:: ts::ControllerButton

--------------------------------

Querying the Key or Button State
********************************

Now that we know what values we use to identify keys or buttons, to access the current state of the button, we use
:code:`ts::InputHandler`, which is a C++ singleton.

The following member functions relate to keys or buttons, (note that overload exist for :code:`ts::KeyboardKey`, :code:`ts::ControllerButton`, :code:`ts::MouseButton`)

.. doxygenfunction:: ts::InputHandler::is_down(KeyboardKey)
.. doxygenfunction:: ts::InputHandler::has_state_changed(KeyboardKey)
.. doxygenfunction:: ts::InputHandler::was_pressed(KeyboardKey)
.. doxygenfunction:: ts::InputHandler::was_released(KeyboardKey)

These functions are highly useful for common applications in games, for example:

.. code-block:: cpp

    if (ts::InputHandler::is_down(ts::SPACE)
        // do action

If this line of code is called once per frame, the action will happen once per frame as long as the button is held down.

In contrast:

.. code-block:: cpp

    if (ts::InputHandler::was_pressed(ts::SPACE)
        // do action

Will trigger the action exactly once, the frame the button is pressed. Afterwards, the player has to first release,
then repress the button to trigger the action again. This makes for a frame-perfect and intuitive input method

--------------------------------

--------------------------------


analog Input
^^^^^^^^^^^^^^

There are a lot less axis when compared to keys or buttons. A keyboard does not have an axis associated with it.

Mouse: Cursor Position
**********************

.. doxygenfunction:: ts::InputHandler::get_cursor_position

This function returns a :code:`Vector2f`. Cursor position (0, 0) is the top left of the currently focused window. This means the
mouse cursor position may be negative if the mouse is outside the focused windows bounds

Mouse: Scrollwheel
******************

.. doxygenfunction:: ts::InputHandler::get_scrollwheel

Scrolling along the x-axis of the screen is rare but possible on some mice.

Controller: Joysticks
*********************

To access the state of one of the analgue joystick positions, we use

.. doxygenfunction:: ts::InputHandler::get_controller_axis_left
.. doxygenfunction:: ts::InputHandler::get_controller_axis_right

Controller: Triggers
********************

To access the controllers triggers, we use:

.. doxygenfunction:: ts::InputHandler::get_controller_trigger_left
.. doxygenfunction:: ts::InputHandler::get_controller_trigger_right

respectively.

--------------------------------------------

ts::InputHandler
^^^^^^^^^^^^^^^^

With this out of the way, here is the complete :code:`ts::InputHandler`:

.. doxygenclass:: ts::InputHandler
    :members:

This class is very powerful so it can be quite overwhelming, hopefully going over things step-by-step will help as
usage of this class offers one of the most convenient ways handling user input.