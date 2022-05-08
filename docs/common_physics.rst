General Usage: Physics
======================

Telescope allows for high level physics simulations by using `Bullet
<https://github.com/bulletphysics/bullet3>`_.

I'm sorry, idk anything about bullet so you will need to do this introduction.

-----------------

Adding Physics Objects
**********************

.. doxygenfunction:: TS_BtAddRigidBox
.. doxygenfunction:: TS_BtAddStaticBox
.. doxygenfunction:: TS_BtAddTriggerBox

Then, we remove any object by calling `TS_BtRemovePhysicsObject`, using the id we stored earlier

.. doxygenfunction:: TS_BtRemovePhysicsObject

-----------------

Moving Physics Objects
**********************

To move an object, we need to modify it's velocity. This is the amount it moves in 3D spaces per "step", which is what the smallest unit of time in the simulation is called.

To set an objects velocity, we use `TS_BtSetLinearVelocity`

.. doxygenfunction:: TS_BtSetLinearVelocity

For the object to actually move, we need to advance the simulation by at least one step. This is accomplished by using `TS_BtStepSimulation`:

.. doxygenfunction:: TS_BtStepSimulation

-----------------

When an object is moving, there is potentially more than one variable that affects it's position per step: gravity.

Each object has a mass. We choose it during object creation before. We can then set the global amount of gravity (in any direction, not just towards the ground like in real life) by using `TS_BtSetGravity`:

.. doxygenfunction:: TS_BtSetGravity

-----------------

Collision Detection
*******************

When we have multiple objects moving in space, eventually, some will collide. Each step, Telescope will queue a number of `TS_CollisionEvent`, one representing each collision between two objects:

.. doxygenstruct:: TS_CollisionEvent
	:members:

To access the lates collision event, we use `TS_BtGetNextCollision`

.. doxygenfunction:: TS_BtGetNextCollision

This way, we can keep track of all the objects moving around.

-----------------

Each physics objects has a *bounding box*. This is the space it occupies in all 3 dimensions. During simulation, we sometimes want to increase the minimum amount of space the distance between the surface of two objects can be. This is useful to avoid "clipping", where two graphics objects collide.

To do this, we increase an objects margin using `TS_BtSetCollisionMargin`

.. doxygenfunction:: TS_BtSetCollisionMargin

------------------

Playing Sounds
**************

Telescope does not only allow for graphics but also sound. To play a sound, we simply call `TS_PlaySound`:

.. doxygenfunction:: TS_PlaySound



