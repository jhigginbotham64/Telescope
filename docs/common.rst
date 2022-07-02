Common
======

Commonly used classes representing spacial position, angle, color, shapes and time.

--------------------

Vectors
*******

When dealing with geometry, an important concept is that of a point in space. For this, telescope offers
:code:`Vector2<T>`, :code:`Vector3<T>` and :code:`Vector4<T>`, which are 2-, 3- and 4-tuples of numbers respectively.

For example, :code:`Vector2f` is best thought of as a point in 2d space.

.. doxygentypedef:: Vector2
.. doxygentypedef:: Vector3
.. doxygentypedef:: Vector4

A number of convenient typedefs for different :code:`T`s are available:

.. doxygentypedef:: Vector2f
.. doxygentypedef:: Vector2i
.. doxygentypedef:: Vector2ui

.. doxygentypedef:: Vector3f
.. doxygentypedef:: Vector3i
.. doxygentypedef:: Vector3ui

.. doxygentypedef:: Vector4f
.. doxygentypedef:: Vector4i
.. doxygentypedef:: Vector4ui

Internally, telescope uses :code:`glm::vec` (from the `glm library <https://github.com/g-truc/glm>`_)
for these classes. This means most vector-vector and vector-matrix operations
are supported out-of-the-box:

.. code-block:: cpp

    Vector2f a = {1, 4}
    Vector2f b = {-1, 2}
    auto sum = a + b;
    std::cout << sum.x << " " << sum.y << std::endl;

.. code-block:: text

    0 6

Where we used `C++s Aggregrate Initialization <https://en.cppreference.com/w/cpp/language/aggregate_initialization>`_
to create the vectors, then accessed their values using their members `x` and `y`.

--------------------

Angles
******

A similarly important concept in geometry is that of an angle. Many libraries create confusion by
forcing users to either measure angles in degrees or in radians. To avoid this, telescope provides a central
:code:`ts::Angle` struct:

.. doxygenstruct:: ts::Angle
    :members:

This class has no public constructors. Instead, we initialize an instance by calling one of:

.. doxygenfunction:: ts::degrees
.. doxygenfunction:: ts::radians

Which take as their arguments a float, interpreted as either degrees or radians.

While most of telescopes functions will ask for an :code:`ts::Angle` as input, we can still convert into
either degree or radians using

.. doxygenfunction:: ts::Angle::as_degrees
.. doxygenfunction:: ts::Angle::as_radians

--------------------

Colors
******

There are many ways to express colors numerically, the systems used in telescope are also the most common:
`RGBA <https://en.wikipedia.org/wiki/RGBA_color_model>`_ and `HSVA <https://en.wikipedia.org/wiki/HSL_and_HSV>`_:

.. doxygenstruct:: ts::RGBA
    :members:

.. doxygenstruct:: ts::HSVA
    :members:

We see that colors are 4-component vectors, where each elements is a 32-bit float in the range [0, 1].

Telescope functions will often have a signature that asks for a color in RGBA format. Because :code:`HSVA.operator RGBA()`
is implicit, we can simply use an object of type :code:`ts::HSVA` as the argument, which is implicitly
converted to :code:`ts::RGBA` for us.

--------------------

Geometric Shapes
****************

Distinct from :code:`ts::Shape`, telescope offer classes describing common geometric shapes. Geometric shapes are
the shapes in the mathematical sense. They cannot be rendered.

Triangle
^^^^^^^^

A triangle is described by the (2d) position of its 3 vertices:

.. doxygenstruct:: ts::Triangle
    :members:

Rectangle
^^^^^^^^

A rectangle is described by the 2d-position of its top-left vertex, along with its size. :code:`ts::Rectangle` are
always axis-aligned:

.. doxygenstruct:: ts::Rectangle
    :members:

Trapezoid
^^^^^^^^

While all rectangles can be expressed as trapezoid, the inverse is not the case. A trapezoid is fully described by the
position of its 4 vertices.

.. doxygenstruct:: ts::Trapezoid
    :members:

Circle
^^^^^^

A circle is fully described by it's center and radius. Unlike :code:`ts::CircleShape`, :code:`ts::Circle` describes a perfect circle,
with maximum "roundness".

.. doxygenstruct:: ts::Circle
    :members:

--------------------

Time
****

Time is a central concept in both physics and real-time rendering, so it is only natural telescope provide a
comprehensive system of how to express time in programming terms.

All time in telescope is at nanosecond precision. If two time stamps are apart by more than one nanosecond,
telescopes will be able to tell the difference.

When measuring duration, a high-resolution, `steady clock` is used. This clocks' ticks do not vary with cpu activity,
display speed or frames per second. This makes them ideal to be used as the lowest common denominator between systems,
1 second of a piece of music is exactly as long as 1 second in the physics simulation, allowing for synchronization.

ts::Time
^^^^^^^^

In telescope, durations are represented by the :code:`ts::Time` class:

.. doxygenclass:: ts::Time
    :members:

This class has no public constructors, instead, we create a :code:`ts::Time` using one of the following functions:

.. doxygenfunction:: ts::minutes
.. doxygenfunction:: ts::seconds
.. doxygenfunction:: ts::milliseconds
.. doxygenfunction:: ts::microseconds
.. doxygenfunction:: ts::nanoseconds

Because we have a central object to measure durations, converting between units is effortless:

.. code-block:: cpp
    :caption: Calculating the Duration of one Frame

    size_t frames_per_second = 60;
    auto fps_duration = ts::seconds(1.f / 60);
    std::cout << "frame duration (ms): " << fps_duration.as_milliseconds() << std::endl;

Furthermore, similarly to :code:`ts::Angle`, all of telescopes functions will ask for a :code:`ts::Time`
object, making it such that users cannot make the error of misremembering which unit the argument expects.

--------------------

ts::Clock
^^^^^^^^^

Often, we want to measure the length of a specific time interval. How long was this button held down? How long has it
been since the player last updated this entity? For cases like these, telescope provides :code:`ts::Clock`, which acts
like a stopwatch.

.. doxygenclass:: ts::Clock
    :members:

When :code:`ts::Clock` is constructed, its timer starts immediately. We can get the currently elapsed time using
:code:`elapsed`; this will not restart the clock. If we want to reset the clock to 0, we call :code:`restart`, which
also returns the time elapsed since either construction or the last :code:`restart`.

.. code-block:: cpp
    :caption: Measuring the average Duration of one loop iteration

    auto clock = ts::Clock();
    auto durations = std::vector<ts::Time>();

    clock.restart();
    for (size_t i = 0; i < 10; ++i)
    {
        // do something time-consuming here
        durations.push_back(clock.restart());
    }

    auto duration_sum_ms = 0;
    for (auto& d : durations)
        duration_sum_ms += d.as_milliseconds();

    std::cout << "Average Loop Duration (ms): " \
              << duration_sum_ms / durations.size() \
              << std::endl;







