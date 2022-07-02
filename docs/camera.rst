Camera
======

Controlling the global window transform

-------------------------

Affine Transforms
^^^^^^^^^^^^^^^^^

In math, an affine transform in 2d space is a 3x3 matrix that can be applied to vectors, usually
points. By cleverly manipulating the elements of the matrix, we can use it to express a number of
morphological operations, such as translating (moving) an object, rotating it around a point, scaling or shearing it, etc.

A convenient property affine transforms is, that they can express a series of operations with just one matrix.
For example, we can start with the identity transform (a transform that, when applied to a point, does not modify it),
apply rotation, apply scaling, then apply translation. This only changes the values in the matrix. Then, when we apply
the matrix to a point, it will be rotated, scaled, then translated. This is highly convenient.

In telescope, affine transforms are represented by :code:`ts::Transform`, which gives an easy to understand,
abstract interface for affine transforms. User will not have to deal with the math behind the scenes.

.. doxygenstruct:: ts::Transform
    :members:

In the section on rendering, we saw that the function

.. doxygenfunction:: ts::Window::render

takes, as its second argument, a transform. This transform will be applied to all points of the renderable entity,
usually a shape, however, **it will not actually mutate the vertices values**. The shape will "show up" rotated, moved, etc.
on screen, but its data will not be modified during each render cycle. Because no vertices are modified,
applying a transform during render is incredibly cheap, performance-wise.

------------------------------

ts::Camera
^^^^^^^^^^

A camera, then, is a transform that is applied to all object rendered to a window. Rotating the camera means rotating
every object during the :code:`render` call. Moving the camera means moving all objects.


This aligns with the usual purpose of cameras in games. We can change how we view the world, without actually changing
any geometry. This functionality is implemented in telescope through the :code:`ts::Camera` class:

.. doxygenclass:: ts::Camera

To create a camera, we need to bind it to a window. Only one camera should be bound each window. To construct it, we call

.. doxygenfunction:: ts::Camera::Camera

Afterwards, if we call a function on our camera object, it will automatically modify the windows global transform.

Note that, if the user also specifies a transform during :code:`ts::Window::render`, this transform and the windows global
transform will be combined, then applied to the geometry to be rendered.

Much like with shapes, a camera positions and origin is at its "centroid": the center of the window.
Rotating the camera means rotating it around this center. Moving the camera to position (x, y) means moving
the camera such that (x, y) is in the center of the window.

:code:`ts::Camera` supports the following operations:

.. doxygenclass:: ts::Camera
    :members:

Lastly, a tip: To rotate the camera around a point (rather than the center), first center the camera on that point,
rotate it, then move it back to itss original center. Because of the way affine transforms work, this is equivalent to
rotating it around the point to begin with. The same applies to zooming in / out on a specific area of the screen.


