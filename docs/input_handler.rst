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
most controller are analogue, you can press them slightly, or harder. Depending on the position of the trigger,
the controllers axis state of that trigger will contain a float: 0 if not pressed at all, 0.5 if half pressed,
1 if pressed fully, etc.

Accessing Key or Button States
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are three types of keys or buttons. A key or buttons state is either "up" or "down":

Keyboard Keys
*************

Keyboard keys are identified as a value of the enum :code:`ts::KeyboardKey`.

These keys include the space bar, any letter on a keyboard, the enter key, volume up/down, etc.

The following are all keyboard keys recognized by telescope:

.. doxygenenum:: ts::KeyboardKey

Mouse Button
************

Mouse buttons, including the omnipresent left and right mouse button, are values of the enum :code:`ts::MouseButton`:

.. doxygenenum:: ts::MouseButton

ControllerButtons
*****************

Controller buttons are a bit tricky because there are so many manufactures. The description of the values of
:code:`ts::ControllerButton` below should cover the most common controller manufactures on the market:

.. doxygenenum:: ts::ControllerButton



ts::InputHandler
^^^^^^^^^^^^^^^^



There are four fundamental states that are accesible through :code:`ts::InputHandler`:

+ the **keyboard state** contains, for each possible keyboard key