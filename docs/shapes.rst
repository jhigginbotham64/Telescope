Shapes
======

Textured and Untextured Triangles, Rectangles, Circles and Polygons.

------------------------------

Shapes are at the center of telescope. Most of anything rendered to the screen will be a shape, which is any object that
inherits from the pure virtual (abstract) class

.. doxygenclass:: ts::Shape

------------------------------

Vertices
^^^^^^^^

In telescope, a shape is a collection of vertex triangles, rendered as a 2d `triangle fan <https://en.wikipedia.org/wiki/Triangle_fan>`_.
Each vertex has 3 properties:

.. doxygenstruct:: ts::Vertex
    :members:

While we rarely need to setup vertices manually, we do have direct access to any shapes' vertices:

.. doxygenfunction:: ts::Shape::set_vertex_position
.. doxygenfunction:: ts::Shape::set_vertex_color
.. doxygenfunction:: ts::Shape::set_vertex_texture_coordinates

By default, each vertex will be white (:code:`RGBA(1, 1, 1, 1)`). Its texture coordinates will be mapped when the shape is created,
such that the top left of the texture (coordinate :code:`{0, 0}`) is aligned with the top left of the shapes `bounding box <https://en.wikipedia.org/wiki/Minimum_bounding_box>`_,
and the bottom right of the texture (coordinate :code:`{1, 1}`) is aligned with the bottom left ot the shapes bounding box.

------------------------------

Centroid
^^^^^^^^

An important concept in morphology is that of the **centroid**. A centroid can be described as the "center of mass" of
a geometric shape. For example, the centroid of a perfect circle is its center. Similarly, the centroid of an axis-aligned
rectangle will be :code:`top_left + size / 2`. For more complex shapes, such as asymmetric triangles, trapezoids
and polygons, the **centroid is defined as the average of all vertices spacial position**.

In telescope, all shapes are anchored at their centroid. Setting the position of a triangle to (x, y) means moving it,
such that its centroid is now at coordinate (x, y). This is important to keep in mind.

------------------------------

Shapes: Triangle
^^^^^^^^^^^^^^^^

The simplest shape is :code:`ts::TriangleShape`. Much like the geometric triangle :code:`ts::Triangle`,
:code:`ts::TriangleShape` is fully described by the position of its three vertices:

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

Unlike :code:`ts::TriangleShape`, :code:`ts::RectangleShape` offers some additional functionality beyond being able to
set the shapes centroid. We can access or modify the top left coordinate of the rectangle, as well as its size. This makes
:code:`ts::RectangleShape` the preferred shape for many applications.

------------------------------

Shapes: Circle
^^^^^^^^^^^^^^

As with their geometric counterpart, circles are described fully by their center and radius.

.. doxygenclass:: ts::CircleShape
    :members:

We an modify a circles radius directly, this essentially scales the circle around its center, which is also its centroid.

:code:`ts::CircleShape` s constructor takes an additional argument **n_outer_vertices**:

.. doxygenfunction:: ts::CircleShape::CircleShape(Vector2f center, float radius, size_t n_outer_vertices)

To render a perfect circle, we would need an infinite number of vertices around the circles perimeters. This would be
prohibitively slow, so we need to limit the number of these vertices. The more vertices, the smoother the circle.
A typical value for :code:`n_outer_vertices` would be 16 or 32, though we can create rotationally symmetrical triangles,
squares, pentagons, etc. by setting the number of vertices to 3, 4, 5 respectively.

-------------------------------

Shapes: Polygon
^^^^^^^^^^^^^^^

Lastly, we have the most flexible type of shape: a polygon. Technically, all shapes mentioned so far are polygons,
Telescope provides special implementation for the more common types of polygons for convenience.

A polygon is fully described by an arbitrary number of points. We specify these points as arguments to
:code:`ts::PolygonShape`s constructor:

.. doxygenfunction:: ts::PolygonShape::PolygonShape(const std::vector<Vector2f>& positions)

Given this set of points, telescope will compute their `convex hull <https://en.wikipedia.org/wiki/Convex_hull>`_,
then construct a polygon such that its vertices describe the convex hull,. This is a somewhat costly operations,
so it is recommended to use the more specialized shapes or points that already describe the convex hull.

The centroid of a polygon is not the mean of all of its points, but the mean of all the points of its convex
hull.

.. doxygenclass:: ts::PolygonShape
    :members:

-------------------------------

Rotating / Scaling Shapes
^^^^^^^^^^^^^^^^^^^^^^^^^

So far, we have only talked about the functionalities the shape sub-classes have by themself. All classes inheriting
from :code:`ts::Shape` actually also inherit a large number of methods.

Other than modifying a shapes vertices properties, which we discussed earlier, we can rotate and/or scale an entire shape:

.. doxygenfunction:: ts::Shape::rotate
.. doxygenfunction:: ts::Shape::scale

By default, these operations use the shapes centroid as the origin for the rotation / scaling. We can change
the origin using

.. doxygenfunction:: ts::Shape::set_origin

Note that these transformation change the actual properties of the shapes vertices.

Other than modifying the position of vertices like this, we can modify how shapes interact with their textures (if any).
Before we can appreciate these functions, we have to learn how deal with textures in the next section.

-------------------------------

-------------------------------

ts::Shape
^^^^^^^^^

A full list of methods available to any class inheriting from :code:`ts::Shape` is available here:

.. doxygenclass:: ts::Shape
    :members:













