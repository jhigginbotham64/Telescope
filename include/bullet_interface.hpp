//
// Copyright 2022, Joshua Higginbotham
//

#pragma once

#include <telescope.h>

/// \brief initialize the physics state
void TS_BtInit();

/// \brief quit the bullet state
void TS_BtQuit();

/// \brief add a rigid, axis-aligned collision box to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param mass: mass of the box
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
/// \param is_kinematic: should the box be a kinematic object
void TS_BtAddRigidBox(int id, float hx, float hy, float hz, float m, float px, float py, float pz, bool isKinematic = false);

/// \brief add a static, axis-aligned collision box to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
void TS_BtAddStaticBox(int id, float hx, float hy, float hz, float px, float py, float pz);

/// \brief add a box-shaped trigger to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
void TS_BtAddTriggerBox(int id, float hx, float hy, float hz, float px, float py, float pz);

/// \brief remove a physics object from the state
/// \param id: id of the object
void TS_BtRemovePhysicsObject(int id);

/// \brief set the linear velocity of a physics object
/// \param id: id of the object
/// \param velocity_x: velocity along the x-dimension
/// \param velocity_y: velocity along the y-dimension
/// \param velocity_z: velocity along the z-dimension
void TS_BtSetLinearVelocity(int id, float vx, float vy, float vz);

/// \brief get the linear velocity of a physics object
/// \param id: id of the object
/// \returns TS_VelocityInfo object describing the velocity along each dimension
TS_VelocityInfo TS_BtGetLinearVelocity(int id);

/// \brief advance the physics simulation by one step
void TS_BtStepSimulation();

/// \brief query the next collision event
/// \returns TS_CollisionEvent, contains two ids of colliding objects
TS_CollisionEvent TS_BtGetNextCollision();

/// \brief get the position of an object
/// \returns TS_PositionInfo, position in 3d space
TS_PositionInfo TS_BtGetPosition(int id);

/// \brief set the global gravity
/// \param gravity_x: acceleration along the x-dimension
/// \param gravity_y: acceleration along the y-dimension
/// \param gravity_z: acceleration along the z-dimension
void TS_BtSetGravity(float gx, float gy, float gz);

/// \brief set the outer margin of a specific collision object
/// \param id: id of the object
/// \param margin: distance between the surface and the outer margin of the object
void TS_BtSetCollisionMargin(int id, float margin);

/// \brief quit the bullet state
void TS_BtQuit();