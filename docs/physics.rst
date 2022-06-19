Physics
=======

Hitbox Collision & Physics Simulation

----------------------------------------

Telescope natively offers a 2d physics system that allows for comprehensive, realistic simulation and automatic collision handling.
This is made possible by two central classes:

.. doxygenclass:: ts::PhysicsWorld
.. doxygenclass:: ts::CollisionShape

----------------------------------------

Physics World
^^^^^^^^^^^^^

A physics world is best thought of as a 2d room. Objects that are inside the same room interact during the simulation.
Objects inside one room do not know anything about objects in another room. Usually, games will only have a single
physics world, though this is not a requirement.

To create a physics world, we call its only constructor:

.. doxygenfunction:: ts::PhysicsWorld::PhysicsWorld

Users are responsible for this instance staying in memory. If it is deallocated prematurely, the application will trigger
undefined behavior.

Usually we want a physics simulation with gravity. We have to manually set the gravity after the world
is created:

.. doxygenfunction:: ts::PhysicsWorld::set_gravity

The simulation is run only during a specified period called a **step**. We specify the length of each step, in seconds,
though callback of this function does not actually take that amount of time.

.. doxygenfunction:: ts::PhysicsWorld::step

Internally, the physics simulation runs on its own internal clock, :code:`step` "fast-forwards" the simulation by
the specified number of seconds internally, then pauses it until called again.

Note that stepping is independent of any other of telescopes systems. This gives users the freedom to run their simulation
arbitrarily fast or slow.

If we do want the simulation to run in real time (that is, one second passing in real-life means one second will pass
inside the simulation), the following pattern is useful:

.. code-block:: cpp
    :caption: Using the result of ts::start_frame to synchronize the physics simulation with the real-world clock

    auto world = PhysicsWorld();
    // add physics objects here

    // render loop
    while (window.is_open())
    {
        auto time = ts::start_frame();

        // advance simulation
        world.step(time);

        // ...

        ts::end_frame();
    }

:code:`ts::start_frame` returns the time since it was last called. Because it is called once at the start of each frame,
this duration is duration of one render cycle. This makes each step of the simulation take the same amount of time as
one frame, synchronizing both cycles.

----------------------------------

Collision Types
^^^^^^^^^^^^^^^

There are three types of physics object in telescope:

.. doxygenenum:: ts::CollisionType

Objects of any type take part in the simulation, the difference between them is which forces they respond to.

+ :code:`STATIC` objects do not respond to any forces and cannot be moved
+ :code:`KINEMATIC` objects only respond to setting the velocity manually, no other forces will move them.
+ :code:`DYNAMIC` objects are fully simulated

Static and kinematic objects cannot collide with each other. Dynamic objects collide with both static, kinematic
and other dynamic objects.

Static objects are usually unchanging level geometry, such as floors or walls.Kinematic objects are reserved
to things that should act like a static objects most of the time but allow for movement, such as moving platforms, doors, etc.

Dynamic objects are fully simulated, an example would be a bouncing ball or an object falling from great height.

We can modify/access the type of an object after creation:

.. doxygenfunction:: ts::CollisionShape::set_type
.. doxygenfunction:: ts::CollisionShape::get_type

------------------------------------

Collision Shapes
^^^^^^^^^^^^^^^^

Similarly to how all native renderable objects in telescope inherit from :code:`ts::Shape`, all physics objects
inherit from :code:`ts::CollisionShape`.

.. doxygenclass:: ts::CollisionShape

This class is pure virtual, making it impossible to instance. Instead, we will need to instance one of its
implementations.

Collision shapes come in 3 types: lines, circles and polygons.

Collision Shapes: Circles
*************************

:code:`ts::CollisionCircle` represents a circular object in 2d space. Unlike :code:`ts::CircleShape`, it does not have
a set number of vertices. For the purposes of simulation, it acts as a perfect circle.

.. doxygenclass:: ts::CollisionCircle
    :members:

------------------------------------

Collision Shapes: Polygons
**************************

All "filled" shapes (shapes that cannot have another shape within its bounds), that can be expressed as 3 or more vertices,
are represented by :code:`ts::CollisionPolygon`.

For convenience, :code:`ts::CollisionPolygon` offers a number of constructors that take, as their argument, a (geometric)
shape. Because of this, the following pattern can be used:

.. code-block:: cpp
    :caption: Creating a Rectangle Shape with an equally sized Hitbox

    auto shape = ts::RectangleShape(Vector2f(50, 50), Vector2f(300, 300));
    auto hitbox = ts::CollisionPolygon(shape);

    // hitbox now has the same position, shape and size of `shape`

No constructors for :code:`ts::CircleShape` are provided, because they should be simulated using a :code:`ts::CollisionCircle`
instead.

.. note::
    In :code:`Telescope.jl`, in addition to the constructors of :code:`ts.CollisionPolygon`,
    :code:`ts.CollisionRectangle` and :code:`ts.CollisionTriangle` are provided for syntactic convenience,
    though both functions still return a :code:`ts.CollisionPolygon`

A full list of all constructors available to :code:`ts::CollisionPolygon` is provided here:

.. doxygenclass:: ts::CollisionPolygon
    :members:

------------------------------------

Collision Shapes: Lines
***********************

Polygons have 3 or more vertices, lines only have 2. Lines are useful for level geometry as they can be stitched together
to form a shape that is not filled (objects can be both inside and outside of it).

Unlike filled shape, lines can furthermore be made to allow for collision from either both or only one side. The latter
is commonly used to create one-way barriers.

.. doxygenclass:: ts::CollisionLine
    :members:

For one-sided lines, which side of the line behaves like a wall, and which one does not, is based on the relative position
of the second vertex to the firsts. For example, if the second vertex is right of the first vertex (its x-coordinate is
larger than that of the first), objects will be able to pass from the bottom to the top (from positive y to negative y),
but not the other way around. To reverse this, we simply swap the vertices' positions, which rotates the line by 180°
around its centroid.

------------------------------------

Collision Shapes: LineSequence
****************************

A common application of lines is to chain them together to create level geometry. When two lines overlap, a "phantom"
collision may appear when a vertex of one line is inside another. A way to avoid this and to make chaining many lines
less cumbersom, telescope provides `CollisionLineSequence`, which automatically removes the problematic vertices and
makes the line sequence be able to be manipulated as a single object:

.. doxygenclass:: ts::CollisionLineSequence
    :members:

------------------------------------

Manipulating Collision Shapes: Properties
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Unlike renderable shapes, we do not have direct control over the position of collision objects. Instead, we
modify their properties, then run the simulation. For example, if we want to move a non-:code:`ts::STATIC` object from
point a to point b, we need to set its velocity such that it moves into the correct position, then step the
simulation enough times for the object to reach that point. Then, we set its velocity back to 0.

:code:`ts::CollisionShapes` provide a number of properties we can modify:

Density
*******

An objects density, along with its surface area, governs its **mass**. An object with density 0 will
have a mass of 0, regardless of its size. Only objects with a non-zero mass will be affected by gravity.

.. doxygenfunction:: ts::CollisionShape::set_density
.. doxygenfunction:: ts::CollisionShape::get_density

Friction
********

Friction determines the deceleration applied to two objects that are colliding. For example, if a circle
is rolling down a flat surface, the circle will loose speed faster, the higher its or the surfaces friction is.
If both have a friction of 0, the circle will not loose any momentum.

.. doxygenfunction:: ts::CollisionShape::set_friction
.. doxygenfunction:: ts::CollisionShape::get_friction

Restitution
***********

The restitution of an object can be described as its "bounciness". The higher the restitution of
a rubber ball, the higher it will bounce when dropped onto a surface. If both surfaces have a restitution
above 0, the forces will multiply.

.. doxygenfunction:: ts::CollisionShape::set_restitution
.. doxygenfunction:: ts::CollisionShape::get_restitution

Linear Velocity
***************

Linear velocity is a 2d vector that influences how an objects moves in 2d space. For :code:`ts::DYNAMIC`, linear
velocity, along with other forces such as gravity, influences the objects trajectory. For :code:`ts::KINEMATIC`
objects, linear velocity is applied as-is. This makes it the preferred way of moving kinematic objects.

.. doxygenfunction:: ts::CollisionShape::set_linear_velocity
.. doxygenfunction:: ts::CollisionShape::get_linear_velocity


Angular Velocity
****************

Unlike linear velocity, angular velocity is a skala, applied in the clockwise direction. It causes
dynamic and kinematic objects to spin. This makes it useful for influencing an objects rotation.

.. doxygenfunction:: ts::CollisionShape::set_angular_velocity
.. doxygenfunction:: ts::CollisionShape::get_angular_velocity

Being a Bullet
**************

We can declare an object to be a "bullet". This informs the physics simulation that the objects
will be either very small and/or moving at high speeds. "Bullet" objects will be simulated with a
higher degree of accuracy. This prevents them from passing through geometry, potentially missing a collision.
"Bullet" objects are much less performant, so they should only be used if necessary.

.. doxygenfunction:: ts::CollisionShape::set_is_bullet
.. doxygenfunction:: ts::CollisionShape::get_is_bullet

Having Fixed Rotation
*********************

An object that has its rotation fixed will not respond to angular velocity, even if induced by secondary
forces. Otherwise, the object will behave normally.

.. doxygenfunction:: ts::CollisionShape::set_is_rotation_fixed
.. doxygenfunction:: ts::CollisionShape::get_is_rotation_fixed

Being Hidden
************

We can remove an object from the simulation temporarily by setting it as hidden. Care should be taken,
as we cannot query the state of a physics object while it is hidden. This includes things like its position
or bounds. Hiding objects should be reserved for performance optimizations.

.. doxygenfunction:: ts::CollisionShape::set_is_hidden
.. doxygenfunction:: ts::CollisionShape::get_is_hidden

Read-Only Properties
********************

:code:`ts::CollisionShape` furthermore supports querying the following properties of an object, though the value of each
of these can only be influenced by modifying the properties mentioned so far, then advancing the simulation.

.. doxygenfunction:: ts::CollisionShape::get_centroid
.. doxygenfunction:: ts::CollisionShape::get_rotation
.. doxygenfunction:: ts::CollisionShape::get_mass
.. doxygenfunction:: ts::CollisionShape::get_inertia
.. doxygenfunction:: ts::CollisionShape::get_center_of_mass

---------------------------------

Manipulating Collision Shapes: Impulses
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Other than setting the linear or angular velocity directly, we can apply force to dynamic objects in
a multitude of ways. These forces interact with the object according to the laws of the simulation:

.. doxygenfunction:: ts::CollisionShape::apply_force_to
.. doxygenfunction:: ts::CollisionShape::apply_linear_impulse_to
.. doxygenfunction:: ts::CollisionShape::apply_torque

Using impulses often makes for more realistic movement, compared to manually setting the
velocity of an object programmatically.

--------------------------------------

Collision Filters
^^^^^^^^^^^^^^^^^

Other than the properties mentioned above, each individual collision shape instance has two
additional properties, both relating to :code:`ts::CollisionFilterGroup`:

.. doxygenenum:: CollisionFilterGroup

We can declare an object to be in one or more filter groups. Additional, we can restrict which
filter groups the object interacts with.

If an object comes into contact with another object
which is of a filter group the object is ignoring, no collision will happen and the objects won't
affect each other, making them pass through each other.

We can specify which group an object is
part of, and which group an object will not interact with, using:

.. doxygenfunction:: ts::CollisionShape::set_collision_filter

By default, an object is in :code:`ts::CollisionFilterGroup::ALL` groups and will `not` interact
with :code:`ts::CollisionFilterGroup::NONE`, meaning it will collide with any object, regardless of its group membership.

------------------------------------------

Handling Collision Events
^^^^^^^^^^^^^^^^^^^^^^^^^

A common scenario in games is this: if two specific objects collide, we want something to happen, such as
playing a sound or updating an entity.

Rather than asking the user to test overlap of every single pair of shapes, telescope offers
an optional way of iterating all collisions that happened in the last simulation step:

.. doxygenfunction:: ts::PhysicsWorld::next_event

Using this funciton, we obtain an object of type :code:`ts::CollisionEvent`:

.. doxygenstruct:: ts::CollisionEvent
    :members:

This objects contains three fields. Firstly, the collision *type* states whether or not this event
describes two objects starting to collide, or seizing to collide. Secondly, the event contains a pointer
to the two shapes involved in the collision. We can identify a :code:`ts::CollisionShape` by its **id**:

.. doxygenfunction:: ts::CollisionShape::get_id

Which allows us to freely trigger behavior depending on collisions occurring during the simulation. Note that the collision
will resolve, regardless of whether the corresponding event was polled. If ignored, the worlds event queue will
simply be cleared when :code:`ts::PhysicsWorld::step` is called again.


.. code-block:: cpp
    :caption: Example usage of next_event

    world.step(ts::seconds(0.1));

    auto event = ts::CollisionEvent()
    while (world.next_event(&event))
    {
        if (event.shape_a.get_id() == 12 and event.shape_b.get_id() == 14)
            // do something
    }

If the steps duration is long enough, the same objects may collide multiple times, triggering multiple collision
events between the same two shapes.

-----------------------------------------

Geometric Queries
^^^^^^^^^^^^^^^^^

Telescope offers a number of sophisticated geometric queries:

Point-in-Shape
**************

To test whether a specific point is inside the bounds of a collision shape, we use:

.. doxygenfunction:: ts::PhysicsWorld::is_point_in_shape

This function simply returns a yes or no answer in form of a boolean.

Ray Cast
********

We can ask whether a ray (a line with a fixed beginning- and end-point) overlaps
with a shape:

.. doxygenfunction:: ts::PhysicsWorld::ray_cast

This function returns an object of type :code:`ts::RayCastInformation`, which, along with the
yes or no answer of whether overlap is present, also returns the (distance-wise) earliest contact point
between the beginning of the ray and the shape, along with the normal vector at the point of intersection.

.. doxygenstruct:: ts::RayCastInformation
    :members:

Distance between Shapes
***********************

Lastly, we can ask for the shortest distance between two specific shapes:

.. doxygenfunction:: ts::PhysicsWorld::distance_between

Similar to ray-casting, this function returns an information object that contains the distance
between the two closest points of the first and second shape respectively:

.. doxygenstruct:: ts::DistanceInformation
    :members:

--------------------------------

Drawable Collision Shapes
^^^^^^^^^^^^^^^^^^^^^^^^^

Instances of :code:`ts::CollisionShape` are "invisible", they cannot be rendered. To make debugging easier, telescope provides
a number of shapes that are both a :code:`ts::CollisionShape` and a :code:`ts::Shape`, such that
both have the exact same size and boundary. These objects behave exactly like a regular physics object, except
they can also be rendered.

After each :code:`ts::PhysicsWorld::step`, :code:`update` needs to be called on all of these objects. This will
synchronize the position and state of the visible shape with that of its physics-simulation counterpart.

.. doxygenclass:: ts::CollisionTriangleShape
    :members:

.. doxygenclass:: ts::CollisionRectangleShape
    :members:

.. doxygenclass:: ts::CollisionCircleShape
    :members:

.. doxygenclass:: ts::CollisionLineShape
    :members:

.. doxygenclass:: ts::CollisionLineSequence
    :members:

---------------------------------

ts::PhysicsWorld
^^^^^^^^^^^^^^^^

For completeness, here is a full list of functions available through :code:`ts::PhysicsWorld`:

.. doxygenclass:: ts::PhysicsWorld
    :members:

--------------------------------

ts::CollisionShape
^^^^^^^^^^^^^^^^^^

A full list of all member functions of :code:`ts::CollisionShape` is available here:

.. doxygenclass:: ts::CollisionShape
    :members:











