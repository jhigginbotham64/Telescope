Physics
=======

Hitbox Collision & Physics Simulation.

----------------------------------------

Telescope natively offers a 2d physics system, that allows for full, realistic simulation and automatic collision handling.
This is made possible by two central classes:

.. doxygenclass:: ts::PhysicsWorld
.. doxygenclass:: ts::CollisionShape

Physics World
^^^^^^^^^^^^^

A physics world is best thought of as a 2d room. Objects that are inside the same room
 interact during the simulation. Objects inside one room do not know anything about objects in another
room. Usually, games will only have a single physics world, though this is not a requirement.

We create a physics world by callings its only constructor:

.. doxygenfunction:: ts::PhysicsWorld::PhysicsWorld

Users are responsible for this instance staying in memory. If it is deallocated prematurely, the application will trigger
undefined behavior.

Usually we want a physics simulation with gravity. We have to manually set the gravity after the world
is created:

.. doxygenfunction:: ts::PhysicsWorld::set_gravity

The simulation is run only during a specified period called a **step**. We specify the length of each step, in seconds,
though resolution of this function does not actually take that amount of time. Internally, the physics simulation runs
on its clock, step "fast-forwards" the simulation by the specified number of seconds.

We trigger this fast-forward by using

.. doxygenfunction:: ts::PhysicsWorld::step

Note that stepping is independent of render cycle duration. If we want the simulation to run in real time (that is, one
seconds in the simulation takes one second to resolve in the real world), the following pattern is useful:

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

:code:`ts::start_frame` returns the time since it was last called. Because it is called once, at the start of each frame,
this duration is duration of one render cycle.

----------------------------------

Collision Types
^^^^^^^^^^^^^^^

There are three types of physics object in telescope:

.. doxygenenum:: ts::CollisionType

Objects of any type take part in the simulation, the difference between them is, which forces they respond to.

+ :code:`STATIC` objects do not respond to any forces and cannot be moved
+ :code:`KINEMATIC` objects only respond to setting the velocity manually, no other forces will move them.
+ :code:`DYNAMIC` objects are fully simulated

Static and kinematic objects cannot collide with each other, while dynamic objects can collide with bot static, kinematic
and other dynamic objects.

Static objects are usually level geometry, such as floors or walls. Kinematic objects are a middle ground, usually
reserved to things that should act like a static object but allow for movement, such as moving platforms, doors, etc.
Dynamic objects are fully simulate, for example a bouncing ball or an object falling from great height.

.. doxygenfunction:: ts::CollisionShape::set_type
.. doxygenfunction:: ts::CollisionShape::get_type

------------------------------------

Collision Shapes
^^^^^^^^^^^^^^^^

Similarly to all native renderable objects in telescope inherit from :code:`ts::Shape`, all physics objects
inherit from :code:`ts::CollisionShape`.

.. doxygenclass:: ts::CollisionShape

This class is once pure virtual, making it impossible to instance. Instead, we will need to instance one of its
implementations.

Collision shapes come in four types: lines, circles, polygons and wireframes.

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
instead. A full list of all constructors available to :code:`ts::CollisionPolygon` is provided here:

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
    :member:

For one-sided lines, which side of the line behaves like a wall, and which one does not, is based on the relative position
of the second vertex to the first. For example, if the second vertex is right of the first vertex (its x-coordinate is
larger than that of the first), objects will be able to pass from the bottom to the top (from positive y to negative y),
but not the other way around. To reverse this, we simply rotate the line by 180°.

------------------------------------

Collision Shapes: Wireframes
****************************

Like polygons initialized with an arbitrary number of points, wireframes will compute the convex hull from then, then create
it's vertices from this hull.. Unlike polygons, however, **wireframes are not filled**. An object can be inside or outside
the bounds of the wireframe. Wireframes can be thought of as a "loop" of a series :code:`ts::CollisionLine`. If the
vertices provided do not loop back to each other such that the second coordinate of the last line-segment is identical
with the first coordinate of the first line-segment, a line is inserted automatically to complete the loop.

.. doxygenclass:: ts::Wireframe
    :members:

------------------------------------

Manipulating Collision Shapes: Properties
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Unlike renderable shapes, we do not have direct control over collision objects. Instead, we
modify their properties, then run the simulation. For example, if we want to move a non-:code:`ts::STATIC` object from
point a to point b, we need to set its velocity such that it moves into the correct position, then step the
simulation for enough time to reach that point, then set its velocity back to 0.

:code:`ts::CollisionShapes` provide a number of properties we can modify:

Density
*******

An objects density along with its surface area governs its mass. An object with density 0 will
have a mass of 0, regardless of its size.

.. doxygenfunction:: ts::CollisionShape::set_density
.. doxygenfunction:: ts::CollisionShape::get_density

Friction
********

Friction determines the forces applied to two objects that are colliding. For example, if a circle
is rolling down a flat surface, the circle will loose speed faster the higher its or the surfaces friction is.
If both have a friction of 0, the circle will never loose speed.

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
velocity is influences the objects trajectory alonge with other forces such as gravity. For non-:code:`ts::DYNAMIC`
objects, linear velocity is forwarded one-to-one, making the the preferred method of moving kinematic objects.

.. doxygenfunction:: ts::CollisionShape::set_linear_velocity
.. doxygenfunction:: ts::CollisionShape::get_linear_velocity


Angular Velocity
****************

Unlike linear velocity, angular velocity a 1-element vector applied tangentially in the clockwise direction. It causes
dynamic and kinematic objects to spin. This makes it useful for influencing an objects rotation.

.. doxygenfunction:: ts::CollisionShape::set_angular_velocity
.. doxygenfunction:: ts::CollisionShape::get_angular_velocity

Being a Bullet
**************

We can declare an object to be a "bullet". This informs the physics simulation that the objects
will be either very small and/or moving at high speeds. "Bullet" objects will be simulated with a
higher degree of accuracy, to prevent them from pass through geometry or missing a collision.
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

:code:`ts::CollisionShape` furthermore supports querying the following properties of an object, though their value can
only be influenced by the other properties mentioned so far, along with advancing the simulation.

.. doxygenfunction:: ts::CollisionShape::get_centroid
.. doxygenfunction:: ts::CollisionShape::get_rotation
.. doxygenfunction:: ts::CollisionShape::get_mass
.. doxygenfunction:: ts::CollisionShape::get_inertia
.. doxygenfunction:: ts::CollisionShape::get_center_of_mass

---------------------------------

Manipulating Collision Shapes: Impulses
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Other than setting the linear or angular velocity directly, we can apply force to objects in
a multitude of ways. These forces interact with the object according to the simulation:

.. doxygenfunction:: ts::CollisionShape::apply_force_to
.. doxygenfunction:: ts::CollisionShape::apply_linear_impulse_to
.. doxygenfunction:: ts::CollisionShape::apply_torque

These often make for more realistic movement, when comparing them to the programmer manually setting the
velocity of an object each step.

--------------------------------------

Collision Filters
^^^^^^^^^^^^^^^^^

Other than the properties mentioned above, each individual collision shape instance has two
additional properties, both relating to :code:`ts::CollisionFilterGroup`:

.. doxygenenum:: CollisionFilterGroup

We can declare an object to be in one or more filter groups. Additional, we can restrict which
filter groups the object interacts with. If an object comes into contact with another object
which is of a filter group the object is ignoring, no collision will happen and the objects won't
affect each other, seeming to pass through each other. We can specify which group an object is
part of and which group an object will not interact with using:

.. doxygenfunction:: ts::CollisionShape::set_collision_filter

By default, an object is in :code:`ts::CollisionFilterGroup::ALL` groups and will not interact
with :code:`ts::CollisionFilterGroup::NONE`, meaning it will collide with any object, regardless of its group membership.

------------------------------------------

Handling Collision Events
^^^^^^^^^^^^^^^^^^^^^^^^^

A common scenario in games is this: if two specific objects collide, we want something to happen. For example, playing
a sound. Rather than asking the user to test overlap of every single pair of shapes, telescope offers
an optional way of iterating all collisions in each simulation step. By calling:

.. doxygenfunction:: ts::PhysicsWorld::next_event

We obtain an object of type :code:`ts::CollisionEvent`:

.. doxygenstruct:: ts::CollisionEvent

This objects contains three fields. Firstly, the collision *type* states whether or not this event
describes two objects starting to collide, or seizing to collide. Secondly, the event contains a pointer
to the two shapes involved in the collision. We can identify a :code:`ts::CollisionShape` by its **id**:

.. doxygenfunction:: ts::CollisionShape::get_id

Which allows us to freely trigger behavior depending on collisions in the simulation. Note that the collision
will resolve regardless of whether the corresponding event was polled. If ignored, the worlds event queue will
simply be cleared when :code:`ts::PhysicsWorld::step` is called again.

If the steps duration is long enough, the same objects may collide multiple times, triggering multiple collision
events with the same two shapes.

-----------------------------------------

Geometric Queries
^^^^^^^^^^^^^^^^^

Telescope offers a number of sophisticated geometric queries.

Geometric Queries: Point-in-Shape
*********************************

To test whether a specific point is inside the bounds of a collision shape, we can call:

.. doxygenfunction:: ts::PhysicsWorld::is_point_in_shape

This function simply returns a yes or no answer in form of a boolean.

Geometric Queries: Ray Cast
***************************

We can ask whether a ray (a line with a fixed beginning- and end-point) overlaps
with a shape:

.. doxygenfunction:: ts::PhysicsWorld::ray_cast

This function returns an object of type :code:`ts::RayCastInformation`, which, along with the
yes or no answer of whether overlap is present, also returns (distance-wise) earliest contact point
between the beginning of the ray and the shape, along with the normal vector at the point of intersection.

.. doxygenstruct:: ts::RayCastInformation

Geometric Queries: Distance between Shapes
******************************************

Lastly, we can ask for the shortest distance between two specific shapes:

.. doxygenfunction:: ts::PhysicsWorld::distance_between

Similar to ray-casting, this function returns an information object that contains the distance
between the two closest points of the first and second shape respectively:

.. doxygenstruct:: ts::DistanceInformation

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










