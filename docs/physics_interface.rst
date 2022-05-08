Internals: Physics Interface
============================

Initializing the State
**********************

.. doxygenfunction:: TS_BtInit
.. doxygenfunction::TS_BtQuit

------------

Adding Physics Objects
**********************

.. doxygenstruct:: TS_PhysicsObject
	:members:

.. doxygenfunction:: TS_BtAddRigidBox

.. doxygenfunction:: TS_BtAddStaticBox

.. doxygenfunction:: TS_BtAddTriggerBox

.. doxygenfunction:: TS_BtRemovePhysicsObject

-----------------


Setting the Position of an Object
*********************************

.. doxygenstruct:: TS_PositionInfo
	:members:

.. doxygenfunction:: TS_BtGetPosition


-----------------

Setting the Velocity of an Object
*********************************

.. doxygenstruct:: TS_VelocityInfo
	:members:

.. doxygenfunction:: TS_BtSetLinearVelocity
.. doxygenfunction:: TS_BtGetLinearVelocity
.. doxygenfunction:: TS_BtSetGravity

-----------------

Collision Detection
*******************

.. doxygenstruct:: TS_CollisionEvent
.. doxygenfunction:: TS_BtStepSimulation
.. doxygenfunction:: TS_BtGetNextCollision

