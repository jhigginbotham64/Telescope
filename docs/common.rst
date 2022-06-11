Common
======

Vectors
*******

When dealing with geometry, an important concept is that of a point in space. For this, telescope offers
:code:`Vector2<T>`, :code:`Vector3<T>` and :code:`Vector4<T>`, which are 2-, 3- and 4-tuples of numbers respectively.

For example, :code:`Vector2f` is best thought of as a point in 2d space.

.. doxygentypedef:: Vector2
.. doxygentypedef:: Vector3
.. doxygentypedef:: Vector4

A number of convenient typedefs are defined for :code:`T` in :code:`{float, int32_t, size_t}`

.. doxygentypedef:: Vector2f
.. doxygentypedef:: Vector2i
.. doxygentypedef:: Vector2ui

.. doxygentypedef:: Vector3f
.. doxygentypedef:: Vector3i
.. doxygentypedef:: Vector3ui

.. doxygentypedef:: Vector4f
.. doxygentypedef:: Vector4i
.. doxygentypedef:: Vector4ui

Internally, telescope uses :code:`glm::vec` for these classes. This means most vector-vector and vector-matrix operations
are supported out-of-the-box:

.. code-block:: cpp
    Vector2f a = {1, 4}
    Vector2f b = {-1, 2}
    auto sum = a + b;
    std::cout << sum.x << " " << sum.y << std::endl;

Where we used :ref:`C++s Aggregrate Initialization<https://en.cppreference.com/w/cpp/language/aggregate_initialization>`
to create the vectors, then accessed their values using their members `x` and `y`.

Angles
******

A similarly important concept in geometry is that of an angle. Specifically, many libraries induce confusion by
forcing users to either measure angles in degrees or in radians. To avoid this, telescope provides a central
:code:`ts::Angle` struct:

.. doxygenstruct:: ts::Angle
    :members:

This class has no public constructors. Instead, we initialize by calling one of two functions:

.. doxygenfunction:: ts::degrees
.. doxygenfunction:: ts::radians

Which take as their arguments a float, interpreted as either degrees or radians.

While most of telescopes functions will ask for an :code:`ts::Angle` as input, we can still convert the object into
either degree or radians using:

.. doxygenfunction:: ts::as_degrees
.. doxygenfunction:: ts::as_radians

This gives us full flexiblity and avoids confusing about which functions asks for what type of angle measurement.

Colors
******

There are many ways to express colors numerically, the two systems used in telescope are also the most common:
:ref:`RGBA<https://en.wikipedia.org/wiki/RGBA_color_model>` and :ref:`HSVA<https://en.wikipedia.org/wiki/HSL_and_HSV>`

These are expressed in two classes:

.. doxygenstruct:: ts::RGBA
    :members:

.. doxygenstruct:: ts::HSVA
    :members:

All components of both RGBA and HSVA are 32-bit floats in in the range `[0, 1]`.

Telescope functions will often ask for a color in RGBA format. Because :code:`HSVA.operator RGBA()` is implicit,
this conversion will often be done automatically for us. If we want to manually convert HSVA to RGBA (or the other
way around), we can use each color systems member functions: :code:`as_hsv` for :code:`RGBA` and :code:`as_rgb`
for :code:`HSVA`.

Geometric Shapes
****************

Distinct from :code:`ts::Shape`, telescope offers for classes describing common geometric shapes. Geometric shapes are
the abstract concept of shapes, like we would use in pure math. They cannot be rendered.

Triangle
^^^^^^^^

A triangle is described by the 2d-poision of its 3 vertices:

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
with maximum roundness.

.. doxygenstruct:: ts::Circle
    :members