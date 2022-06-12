Shapes
======

Textured and Untextured Triangles, Rectangles, Circles, Polygons

------------------------------

Shapes are at the center of telescope. Anything rendered that shows up on screen will be a shape, so it is important to
know how to use them.

All shapes inherit from the pure virtual (abstract) class :code:`ts::Shape`.

.. doxygenclass:: ts::Shape

------------------------------

Vertices
^^^^^^^^

In telescope, a shape is a collection of vertex triangles, rendered as a 2d `triangle fan <https://en.wikipedia.org/wiki/Triangle_fan>`_.
Each vertex has 3 properties:

.. doxygenstruct:: ts::Vertex
    :members:

While we rarely need to setup vertices manually, we have direct access to any shapes vertices using the following functions:

.. doxygenfunction:: ts::Shape::set_vertex_position
.. doxygenfunction:: ts::Shape::set_vertex_color
.. doxygenfunction:: ts::Shape::set_vertex_texture_coordinates

By default, each vertex will be white (:code:`RGBA(1, 1, 1, 1)`). Its texture coordinates will be mapped when the shape is created,
such that the top left of the texture (coordinate :code:`{0, 0}`) is aligned with the top left of the shapes bounding box,
and the bottom right of the texture (coordinate :code:`{1, 1}`) is aligned with the bottom left ot the shapes bounding box.

We will learn more about how textures behave when mapped to vertices later in the next section.

------------------------------

Centroid
^^^^^^^^

An important concept in telescope is that of the **centroid**. The centroid can be described as the "center of mass" of
a geometric shape. For example, the centroid of a perfect circle is its center. Similarly, the centroid of an axis-aligned
rectangle will always be at :code:`top_left + size / 2`. For more complex shapes such as asymmetric triangles, trapezoids
and polygons, the **centroid is defined as the average of all vertices spacial position**.

In telescope, all shapes are anchored at their centroid. Setting the position of a triangle to (x, y) means moving it,
such that its centroid is now at coordinate (x, y). This is important to keep in mind.

------------------------------

Shapes: Triangle
^^^^^^^^^^^^^^^^

The simplest shape is :code:`ts::TriangleShape`. Much like the geometric triangle :code:`ts::Triangle`,
:code:`ts::TriangleShape` is fully described by three points in space.

.. doxygenclass:: ts::TriangleShape
    :members:

We see that we can create a triangle from 3 spacial vertex positions, or from its geometric counterpart.

------------------------------

Shapes: Rectangle
^^^^^^^^^^^^^^^^^

A rectangle is a 4-vertex shape. It's geometric equivalent would be a trapezoid, as :code:`ts::RectangleShape` may not
necessarily be axis-aligned.

.. doxygenclass:: ts::RectangleShape
    :members:

Unlike :code:`ts::TriangleShape`, :code:`ts::TriangleShape` offers some additional functionality beyond being able to
set the shapes centroid: we can access or modify the top left coordinate of the shape and its size. This offers some
additional convenience.

------------------------------

Shapes: Circle
^^^^^^^^^^^^^^

As with their geometric counterpart, circles are described fully by their center and radius.

.. doxygenclass:: ts::CircleShape
    :members:

We an modify a circles radius directly, this essentially scales the circle around its center, which is also its centroid.

:code:`ts::CircleShape` s constructor takes an additional argument **n_outer_vertices**:

.. doxygenfunction:: ts::CircleShape::CircleShape(Vector2f center, float radius, size_t n_outer_vertices)

This governs the number of outer vertices the circle has. The higher the number, the "rounder" the circle will appear as.
Usually, a good middle ground are 16 or 32 vertices. However, we can create a rotationally symmetrical triangle by setting
:code:`n_outer_vertices` to 3, and a square by setting it to 4.

-------------------------------

Shapes: Polygon
^^^^^^^^^^^^^^^

Lastly, we have the most flexible type of shape: a polygon. Technically, all shapes mentioned so far are polygons,
however, telescope provides special implementation for the more common types of polygons to increase ease of use.

A polygon is fully described by an arbitrary number of points. We give these points as the argument to
:code:`ts::PolygonShape`s constructor:

.. doxygenfunction:: ts::PolygonShape::PolygonShape(const std::vector<Vector2f>& positions)

Telescope will compute the `convex hull <https://en.wikipedia.org/wiki/Convex_hull>`_ of these points, then construct
a polygon from the vertices of the convex hull. This is a somewhat costly operations, so it is recommended to use the
more specialized shapes, unless impossible.

Similarly, the centroid of a polygon is not the mean of all of its points, but the mean of all the points of its convex
hull.

.. doxygenclass:: ts::PolygonShape
    :members:

-------------------------------

Rotating / Scaling Shapes
^^^^^^^^^^^^^^^^^^^^^^^^^

So far, we have only talked about the functionalities the shape implementations have by themself. However, they actually
inherit a large number of methods from their base class :code:`ts::Shape`.

Other than modifying a shapes vertices properties, which we discussed earlier, we can rotate and scale an entire shape,
using:

.. doxygenfunction:: ts::Shape::rotate
.. doxygenfunction:: ts::Shape::scale

By default, these operations use the shapes centroid as the origin for the transform. If this is undesired, we can change
the origin using

.. doxygenfunction:: ts::Shape::set_origin

Furthermore, we can modify how shapes interact with their textures (if any). Before we can appreciate these functions,
we do have to learn how deal with textures outside of shapes in the next section.












