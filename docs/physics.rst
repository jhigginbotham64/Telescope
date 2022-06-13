Physics
=======

Hitbox Collision & Physics Simulation.

----------------------------------------

Telescope natively supports a 2d physics engine that offers enough flexibility to serve most
games. It automates hitbox collision, which can be notoriously difficult to implement, and
allows for high-level geometric queries such as ray-casting. All this is made possible by two classes:

.. doxygenclass:: ts::PhysicsWorld
.. doxygenclass:: ts::CollisionHandler

Physics World
^^^^^^^^^^^^^

A physics world is best thought of as a 2d room somewhere. Objects that are inside the same room
always interact during the simulation. Objects inside one room do not know anything about another
room. Usually, games will only have a single physics world, though this is not a requirement.

To create a physics world, we use

.. doxygenfunction:: ts::PhysicsWorld::PhysicsWorld

We need to make sure that this instance stays in memory, as it will contain all the physics objects we will create
from this point on.

Advancing the physics simulation is completely detached from the render loop. This allows for arbitrary precision
(in terms of time) and it allows use to, for example, run 3s of the simulation in just one frame (which is usually 1/60
of a second in real time).

To advance the simulation by an arbitrary amount of time, we use

.. doxygenfunction:: ts::PhysicsWorld::step

This function takes the amount of time to be simulated as its argument.

Now that we know how to run the simulation, let's actually add simulated objects.

----------------------------------

Collision Types
^^^^^^^^^^^^^^^

There are three types of physics object in telescope:

.. doxygenenum:: ts::CollisionType


All types of objects take part in the simulation, the difference is in which forces they respond to.

+ :code:`STATIC` objects do not respond to any forces and cannot be moved
+ :code:`KINEMATIC` objects can only be moved manually by the engine, not secondary forces such as gravity affect them.
+ :code:`DYNAMIC` objects are fully simulated, however they can also me manually moved by the engine

Static objects are usually level geometry, such as floors or walls. Kinematic objects are a middle ground, usually
reserved to things that should act like a static object but allow for movement, such as moving platforms, doors, etc.
Dynamic objects are fully simulate, for example a bouncy ball or an object falling from great height.

------------------------------------

Collision Shapes
^^^^^^^^^^^^^^^^

Similarly to how all renderable object inherit :code:`ts::Shape`, all physics objects that take part in the simulation
inherit from :code:`ts::CollisionShape`.

.. doxygenclass:: ts::CollisionShape

This class is once again pure virtual, making it impossible to instance. Instead, we will need to instance one of its
implementations.

Unlike with shapes, there are different types of collision shapes: lines, circles, polygons and wireframes.


Collision Shapes: Circles
*************************

Unlike :code:`ts::CircleShape`, :code:`ts::CollisionCircle` does not have a set number of vertices. In the simulation,
it acts as a perfect circle with maximum smoothness.

.. doxygenclass:: ts::CollisionCircle
    :members:

------------------------------------

Collision Shapes: Polygons
**************************

All "filled" shapes (shapes that cannot have another shape within its bounds) that can be expressed as 3 or more vertices
are represented by :code:`ts::CollisionPolygon`. Rather than there being a CollisionTriangle or CollisionRectangle,
we instance :code:`ts::CollisionPolygon` with 3 or 4 vertices respectively.

For convenience, :code:`ts::CollisionPolygon` offers a number of constructors that takes as their argument a (geometric)
shape. Because of this, the following pattern can be used:

.. code-block:: cpp
    :caption: Creating a Rectangle Shape with an equally sized Hitbox

    auto shape = ts::RectangleShape(Vector2f(50, 50), Vector2f(300, 300));
    auto hitbox = ts::CollisionPolygon(shape);

    // hitbox now has the same position, shape and size of `shape`

No constructors for :code:`ts::CircleShape` are provided, because they should be simulated using a :code:`ts::CollisionCircle`
instead.

.. doxygenclass:: ts::CollisionPolygon
    :members:

------------------------------------

Collision Shapes: Lines
***********************

Polygons have 3 or more vertices, lines only have 2. Lines are useful for level geometry, also, unlike other shapes,
they can be made to allow for collision from one side but not from the other side. A good example for this is a one-way
door. You want the player character to pass through the line from one side but afterwards, the line behaves like
a solid shape from the other side. This property of lines is called being one- or two-sided respectively.

.. doxygenclass:: ts::CollisionLine
    :member:

------------------------------------

For one sided lines, which side of the line behaves like a wall and which doesn't is based on the relative position of
the vertices. For example, if the second vertex is right of the first vertex, objects can pass from top to bottom
through the line, but not from bottom to top. This also means that we can invert the one-sided nature by simply rotating
the line 180°.

Collision Shapes: Wireframes
****************************

Like polygons, wireframes take a number of vertices for whom the shape will compute the convex hull. Unlike polygons,
however, wireframes are not filled. An object can be inside or outside the bounds of the wireframe.
Wireframes can be thought of as a loop of :code:`ts::CollisionLine`. If the vertices provided do not loop back to
each other, a line is inserted automatically, such that they do.

.. doxygenclass:: ts::WireFrame
    :members:

------------------------------------

Manipulating Collision Shapes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Unlike renderable shapes, we do not have direct control over collision objects. All we can do is
modify their properties, then run the simulation. For example, if we want to move a non-:code:`ts::STATIC` object from
point a to point b, we need to set its velocity such that it moves into the correct position, then step the
simulation for enough time to reach that point


